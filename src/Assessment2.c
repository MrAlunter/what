#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "timer.h"
#include "buzzer.h"
#include "SPI.h"
#include "buttons.h"
#include "adc.h"

typedef enum
{
    playback,
    gameplay,

} gameModes;

typedef enum
{
    wait,
    button1,
    button2,
    button3,
    button4,
    checker,
    holder,
} buttonStates;

typedef enum
{
    maker,
    playwait,
    hold,
} playback_states;

uint8_t score0s[11] =
    {
        0xFF,       // nothing
        0b00001000, // 0
        0b01101011, // 1
        0b01000100, // 2
        0b01000001, // 3
        0b00100011, // 4
        0b00010001, // 5
        0b00010000, // 6
        0b00110100, // 7
        0b00000000, // 8
        0b00000001, // 9
};

uint16_t score1s[11] =
    {
        0xFF,       // nothing
        0b10001000, // 0
        0b11101011, // 1
        0b11000100, // 2
        0b11000001, // 3
        0b10010001, // 4
        0b10100011, // 5
        0b10010000, // 6
        0b10110100, // 7
        0b10000000, // 8
        0b10000001, // 9}
};

uint32_t STATE_LSFR = 0x11271965;
uint32_t STEP = 0;
uint32_t next(void)
{
    uint32_t BIT = STATE_LSFR & 0b01;
    STATE_LSFR >>= 1;

    if (BIT == 1)
    {
        STATE_LSFR ^= 0xE2023CAB;
    }
    STEP = STATE_LSFR & 0b11;
    return STEP;
}

volatile uint32_t current_time;
uint32_t start_time = 0;
uint8_t pb_previous = 0xFF;
uint8_t pb_current = 0xFF;
volatile uint8_t pb_state = 0xFF;

uint8_t sequenceI = 0;   // index of the sequence
uint8_t round_count = 1; // number of steps in the sequence
uint8_t guess = 0;       // index of the guess
uint8_t value = 0;       // current step in the sequence

uint8_t time;        // time between steps in the sequence
uint8_t num;         // Number of the button pressed
uint8_t ledD = 0;    // LED display
uint8_t score1 = 1;  // Score for player 1
uint8_t score10 = 0; // Score for player 10

void main(void)
{
    cli();                      // Disable interrupts
    uart_init();                // Initialise UART (serial port)
    timer_init();               // Initialise timer (for UART)
    buzzer_init();              // Initialise buzzer (PWM)
    buttons_init();             // Initialise buttons (for user input)
    spi_init();                 // Initialise SPI
    spi_write(0xFF);            // Clear SPI (MOSI) buffer
    uart_init();                // Initialize the UART (serial port)
    adc_init();                 // Initialize the ADC
    TCB0.INTCTRL = TCB_CAPT_bm; // Enable interrupts for TCB0
    sei();                      // Enable interrupts
    gameModes gameMode = playback;
    buttonStates buttonState = wait;
    playback_states playback_states = maker;

    uint8_t button_release = 0; // Check if any buttons are released
    uint8_t waiting;            // Check if the user is waiting for the next step
    uint8_t guessMade = 0;      // Check if the user has made a guess
    uint8_t disp_score = 0;     // Check if the score needs to be displayed
    uint8_t finished;           // Check if the round is finished

    while (1)
    {
        uint8_t pb_changed = pb_current ^ pb_previous; // Check if any buttons have changed
        uint8_t pb_falling = pb_changed & pb_previous; // Check if any buttons are falling
        uint8_t pb_rising = pb_changed & pb_current;   // Check if any buttons are rising
        pb_previous = pb_current;                      // Update the previous button state
        pb_current = pb_state;                         // Update the current button state

        switch (gameMode)
        {
        case playback:
        {
            uart_puts("gamemode = playback\n");
            switch (playback_states)
            {
            case maker:
            {
                uart_puts("playback_states = maker\n");
                if (sequenceI < round_count)
                {
                    uart_puts("sequenceI < round_count\n");
                    STATE_LSFR = 0x11271965;
                    for (uint8_t i = 0; i <= sequenceI; i++)
                    {
                        uart_puts("sequenceI < round_count loop\n");
                        uart_puts("next\n");
                        value = next();         // Get the next step
                        uart_putc(value + '0'); // Print the current step
                        switch (value)
                        {
                        case 0:
                            uart_puts("value = 0\n");
                            buttonState = button1;
                            break;
                        case 1:
                            uart_puts("value = 1\n");
                            buttonState = button2;
                            break;
                        case 2:
                            uart_puts("value = 2\n");
                            buttonState = button3;
                            break;
                        case 3:
                            uart_puts("value = 3\n");
                            buttonState = button4;
                            break;
                        }
                        playback_states = playwait;
                    }
                }
                else if (sequenceI >= round_count)
                {
                    uart_puts("sequenceI >= round_count\n");
                    uart_puts("change gameMode = gameplay\n");
                    gameMode = gameplay;
                    buttonState = wait;
                    playback_states = hold;
                    sequenceI = 0;
                }
                break;
            }

            case playwait:
            {
                uart_puts("playwait\n");
                if ((current_time - start_time) >= ((1750 * ADC0.RESULT >> 8) + 250))
                {
                    uart_puts("sequence++\n");
                    sequenceI++;
                    playback_states = maker;
                }
                break;
            }
            break;

            case hold:
            {
                uart_puts("holding\n");
                break;
            }
            break;
                uart_puts("\n");
            }
        }
        case gameplay:
        {
            uart_puts("now gamemode = gameplay\n");
            if (pb_falling & PIN4_bm)
            {
                uart_puts("pb_falling & PIN4_bm\n");
                buttonState = button1;
                button_release = 1;
                guessMade = 1;
                if (pb_falling & pb_rising)
                {
                    button_release = 0;
                }
            }
            if (pb_falling & PIN5_bm)
            {
                uart_puts("pb_falling & PIN5_bm\n");
                buttonState = button2;
                button_release = 1;
                guessMade = 1;
                if (pb_falling & pb_rising)
                {
                    button_release = 0;
                }
            }
            if (pb_falling & PIN6_bm)
            {
                uart_puts("pb_falling & PIN6_bm\n");
                buttonState = button3;
                button_release = 1;
                guessMade = 1;
                if (pb_falling & pb_rising)
                {
                    button_release = 0;
                }
            }
            if (pb_falling & PIN7_bm)
            {
                uart_puts("pb_falling & PIN7_bm\n");
                buttonState = button4;
                button_release = 1;
                guessMade = 1;
                if (pb_falling & pb_rising)
                {
                    button_release = 0;
                }
            }
            break;
            uart_puts("\n");
        }
        break;
        }

        switch (buttonState)
        {
        case holder:
        {
            break;
        }
        case wait:
        {
            if (pb_rising)
            {
                button_release = 0;
            }
            // uart_puts("buttonState = wait\n");
            if (((current_time - start_time) >= (((1750 * ADC0.RESULT >> 8) + 250) >> 1)) && button_release == 0)
            {
                TCA0.SINGLE.PERBUF = 0;
                ledD = 4;
                if ((current_time - start_time) >= ((1750 * ADC0.RESULT >> 8) + 250))
                {
                    // if (disp_score == 1)
                    // {
                    //     ledD = 6;
                    //     disp_score = 0;
                    //     buttonState = wait;
                    // }
                    // if (disp_score == 0)
                    // {
                    //     gameMode = playback;
                    //     buttonState = wait;
                    //     playback_states = maker;
                    // }
                    time = 0;
                    waiting = 0;
                    if (guessMade)
                    {
                        guessMade = 0;
                        buttonState = checker;
                    }
                }
            }
            break;
        }

        case button1:
        {
            ledD = 0;
            uart_puts("buttonState = button0\n");
            start_time = current_time;
            TCA0.SINGLE.PERBUF = 9487;
            TCA0.SINGLE.CMP0BUF = 4743;
            PORTB.OUTSET = PIN0_bm;
            start_time = current_time;
            waiting = 1;
            button_release = 1;
            if (gameMode != gameplay)
            {
                button_release = 0;
            }
            else
            {
                guess = 0;
            }
            buttonState = wait;
            break;
        }

        case button2:
        {
            num = 1;
            ledD = 1;
            uart_puts("buttonState = button1\n");
            start_time = current_time;
            TCA0.SINGLE.PERBUF = 11288;
            TCA0.SINGLE.CMP0BUF = 5644;
            PORTB.OUTSET = PIN0_bm;
            start_time = current_time;
            waiting = 1;
            button_release = 1;
            if (gameMode != gameplay)
            {
                button_release = 0;
            }
            else
            {
                guess = 1;
            }
            buttonState = wait;
            break;
        }

        case button3:
        {
            num = 2;
            ledD = 2;
            uart_puts("buttonState = button2\n");
            start_time = current_time;
            TCA0.SINGLE.PERBUF = 12654;
            TCA0.SINGLE.CMP0BUF = 6327;
            PORTB.OUTSET = PIN0_bm;
            start_time = current_time;
            waiting = 1;
            button_release = 1;
            if (gameMode != gameplay)
            {
                button_release = 0;
            }
            else
            {
                guess = 2;
            }
            buttonState = wait;
            break;
        }

        case button4:
        {
            num = 3;
            ledD = 3;
            uart_puts("buttonState = button3\n");
            start_time = current_time;
            TCA0.SINGLE.PERBUF = 14242;
            TCA0.SINGLE.CMP0BUF = 7121;
            PORTB.OUTSET = PIN0_bm;
            start_time = current_time;
            waiting = 1;
            button_release = 1;
            if (gameMode != gameplay)
            {
                button_release = 0;
            }
            else
            {
                guess = 3;
            }
            buttonState = wait;
            break;
        }

        case checker:
        {
            uart_puts("buttonState = checker\n");
            STATE_LSFR = 0x11271965;
            for (uint8_t i = 0; i <= sequenceI; i++)
            {
                value = next();
                uart_puts("value = ");
                uart_putc(value + '0');
                uart_puts("\n");
            }
            if (guess == value)
            {
                uart_puts("correct\n");
                sequenceI++;
                if (sequenceI >= round_count)
                {
                    uart_puts("round fin\n");
                    round_count++;
                    sequenceI = 0;
                    start_time = current_time;
                    finished = 1;
                    score1++;
                    disp_score = 1;
                    ledD = 5;
                    finished = 1;

                    if (score1 == 10)
                    {
                        score10++;
                        score1 = 0;
                    }
                    gameMode = playback;
                    buttonState = wait;
                    playback_states = maker;
                    break;
                }
                else if (sequenceI < round_count)
                {
                    uart_puts("round cont\n");
                    buttonState = wait;
                }
            }
            else
            {
                uart_puts("incorrect\n");
                score1 = 1;
                disp_score = 1;
                finished = 2;
                score10 = 0;
                sequenceI = 0;
                round_count = 1;
                gameMode = playback;
                buttonState = wait;
                playback_states = playwait;
                break;
            }
            break;
        }
        }
    }
}

ISR(TCB0_INT_vect)
{
    current_time++;

    static uint8_t count0 = 0;
    static uint8_t count1 = 0;

    uint8_t pb_sample = PORTA.IN;
    uint8_t pb_changed = pb_sample ^ pb_state;

    count1 = (count1 ^ count0) & pb_changed;
    count0 = ~count0 & pb_changed;

    pb_state ^= (count1 & count0) | (pb_changed & pb_state);

    static uint8_t side = 0;
    switch (ledD)
    {
    case 0:
    {
        if (side == 0)
        {
            spi_write(0b01111111);
        }
        else
        {
            spi_write(0b10111110);
        }
        break;
    }
    case 1:
    {
        if (side == 0)
        {
            spi_write(0b01111111);
        }
        else
        {
            spi_write(0b11101011);
        }
        break;
    }
    case 2:
    {
        if (side == 0)
        {
            spi_write(0b00111110);
        }
        else
        {
            spi_write(0b11111111);
        }
        break;
    }
    case 3:
    {
        if (side == 0)
        {
            spi_write(0b01101011);
        }
        else
        {
            spi_write(0b11111111);
        }
        break;
    }
    case 4:
    {
        if (side == 0)
        {
            spi_write(0b01111111);
        }
        else
        {
            spi_write(0b11111111);
        }
        break;
    }
    case 5:
    {
        if (side == 0)
        {
            spi_write(0b00000000);
        }
        else
        {
            spi_write(0b10000000);
        }
    }
    case 6:
    {
        if (side == 0)
        {
            spi_write(score0s[score1]);
        }
        else
            spi_write(score1s[score10]);
    }
    }
    side = !side;
    TCB0.INTFLAGS = TCB_CAPT_bm;
}
