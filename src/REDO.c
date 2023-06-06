// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <avr/interrupt.h>
// #include "uart.h"
// #include "timer.h"
// #include "buzzer.h"
// #include "SPI.h"
// #include "buttons.h"
// #include "adc.h"

// typedef enum
// {
//     playback,
//     gameplay,

// } gameModes;

// typedef enum
// {
//     wait,
//     button1,
//     button2,
//     button3,
//     button4
// } buttonStates;

// typedef enum
// {
//     maker,
//     playwait,
// } playback_states;

// uint32_t STATE_LSFR = 0x11271965;
// uint32_t STEP = 0;
// uint32_t next(void)
// {
//     uint32_t BIT = STATE_LSFR & 0b01;
//     STATE_LSFR >>= 1;

//     if (BIT == 1)
//     {
//         STATE_LSFR ^= 0xE2023CAB;
//     }
//     STEP = STATE_LSFR & 0b11;
//     return STEP;
// }

// volatile uint32_t current_time;
// uint32_t start_time = 0;
// uint8_t pb_previous = 0xFF;
// uint8_t pb_current = 0xFF;
// volatile uint8_t pb_state = 0xFF;

// uint8_t sequenceI = 0;   // index of the sequence
// uint8_t round_count = 1; // number of steps in the sequence
// uint8_t guess = 0;       // index of the guess
// uint8_t value = 0;       // current step in the sequence

// uint8_t time;            // time between steps in the sequence
// uint8_t num;             // Number of the button pressed

// void main(void)
// {
//     cli();                      // Disable interrupts
//     uart_init();                // Initialise UART (serial port)
//     timer_init();               // Initialise timer (for UART)
//     buzzer_init();              // Initialise buzzer (PWM)
//     buttons_init();             // Initialise buttons (for user input)
//     spi_init();                 // Initialise SPI
//     spi_write(0xFF);            // Clear SPI (MOSI) buffer
//     uart_init();                // Initialize the UART (serial port)
//     adc_init();                 // Initialize the ADC
//     TCB0.INTCTRL = TCB_CAPT_bm; // Enable interrupts for TCB0
//     sei();                      // Enable interrupts
//     gameModes gameMode = maker;
//     buttonStates buttonState = wait;
//     playback_states playback_states = maker;

//     uint16_t playback_delay; // delay between steps in the sequence
//     uint8_t button_release; // Check if any buttons are released
//     while (1)
//     {
//         uint8_t pb_changed = pb_current ^ pb_previous;        // Check if any buttons have changed
//         uint8_t pb_falling = pb_changed & pb_previous;        // Check if any buttons are falling
//         uint8_t pb_rising = pb_changed & pb_current;          // Check if any buttons are rising
//         pb_previous = pb_current;                             // Update the previous button state
//         pb_current = pb_state;                                // Update the current button state
//         playback_delay = (((1750 * ADC0.RESULT >> 8) + 250)); // Calculate the playback delay
//         switch (gameMode)
//         {
//         case playback:
//         {
//             uart_puts("gamemode = playback\n");
//             switch (playback_states)
//             {
//             case maker:
//             {
//                 uart_puts("playback_states = maker\n");
//                 if (sequenceI < round_count)
//                 {
//                     uart_puts("sequenceI < round_count\n");
//                     uart_puts("next\n");
//                     value = next();         // Get the next step
//                     uart_putc(value + '0'); // Print the current step
//                     switch (value)
//                     {
//                     case 0:
//                         uart_puts("value = 0\n");
//                         buttonState = button1;
//                         break;
//                     case 1:
//                         uart_puts("value = 1\n");
//                         buttonState = button2;
//                         break;
//                     case 2:
//                         uart_puts("value = 2\n");
//                         buttonState = button3;
//                         break;
//                     case 3:
//                         uart_puts("value = 3\n");
//                         buttonState = button4;
//                         break;
//                     }
//                     playback_states = playwait;
//                     start_time = current_time;
//                 }
//                 else
//                 {
//                     uart_puts("sequenceI >= round_count\n");
//                     gameMode = gameplay;
//                     sequenceI = 0;
//                 }
//                 break;
//             }
//             case playwait:
//             {
//                 sequenceI++;
//                 playback_states = maker;
//                 break;
//             }
//             }
//             break;
//             uart_puts("\n");
//         }

//         case gameplay:
//         {
//             uart_puts("gamemode = gameplay\n");
//             if (pb_falling & PIN4_bm)
//             {
//                 uart_puts("pb_falling & PIN4_bm\n");
//                 buttonState = button1;
//                 button_release = 1;
//                 if (pb_falling & pb_rising)
//                 {
//                     button_release = 0;
//                 }
//             }
//             if (pb_falling & PIN5_bm)
//             {
//                 uart_puts("pb_falling & PIN5_bm\n");
//                 buttonState = button2;
//                 button_release = 1;
//                 if (pb_falling & pb_rising)
//                 {
//                     button_release = 0;
//                 }
//             }
//             if (pb_falling & PIN6_bm)
//             {
//                 uart_puts("pb_falling & PIN6_bm\n");
//                 buttonState = button3;
//                 button_release = 1;
//                 if (pb_falling & pb_rising)
//                 {
//                     button_release = 0;
//                 }
//             }
//             if (pb_falling & PIN7_bm)
//             {
//                 uart_puts("pb_falling & PIN7_bm\n");
//                 buttonState = button4;
//                 button_release = 1;
//                 if (pb_falling & pb_rising)
//                 {
//                     button_release = 0;
//                 }
//             }
//             break;
//             uart_puts("\n");
//         }
//         }

//         switch (buttonState)
//         {
//         case wait:
//         {
//             uart_puts("buttonState = wait\n");
//             if (((current_time - start_time) >= (playback_delay >> 1)))
//             {
//                 TCA0.SINGLE.PERBUF = 0;
//                 uart_puts("Finished Waiting buzz off\n"); // Print the current state
//                 if ((current_time - start_time) >= playback_delay)
//                 {
//                     uart_puts("Finished Waiting\n"); // Print the current state
//                     playback_states = playwait;
//                     time = 0;
//                 }
//             }
//             break;
//         }

//         case button1:
//             num = 0;
//             uart_puts("buttonState = button0\n");
//             start_time = current_time;
//             TCA0.SINGLE.PERBUF = 9487;
//             TCA0.SINGLE.CMP0BUF = 4743;
//             PORTB.OUTSET = PIN0_bm;
//             start_time = current_time;
//             time = 1;
//             buttonState = wait;
//             break;

//         case button2:
//             num = 1;
//             uart_puts("buttonState = button1\n");
//             start_time = current_time;
//             TCA0.SINGLE.PERBUF = 11288;
//             TCA0.SINGLE.CMP0BUF = 5644;
//             PORTB.OUTSET = PIN0_bm;
//             start_time = current_time;
//             time = 1;
//             buttonState = wait;
//             break;

//         case button3:
//             num = 2;
//             uart_puts("buttonState = button2\n");
//             start_time = current_time;
//             TCA0.SINGLE.PERBUF = 12654;
//             TCA0.SINGLE.CMP0BUF = 6327;
//             PORTB.OUTSET = PIN0_bm;
//             start_time = current_time;
//             time = 1;
//             buttonState = wait;
//             break;

//         case button4:
//             num = 3;
//             uart_puts("buttonState = button3\n");
//             start_time = current_time;
//             TCA0.SINGLE.PERBUF = 14242;
//             TCA0.SINGLE.CMP0BUF = 7121;
//             PORTB.OUTSET = PIN0_bm;
//             start_time = current_time;
//             time = 1;
//             buttonState = wait;
//             break;
//         }
//     }
// }

// ISR(TCB0_INT_vect)
// {
//     current_time++;

//     static uint8_t count0 = 0;
//     static uint8_t count1 = 0;

//     uint8_t pb_sample = PORTA.IN;
//     uint8_t pb_changed = pb_sample ^ pb_state;

//     count1 = (count1 ^ count0) & pb_changed;
//     count0 = ~count0 & pb_changed;

//     pb_state ^= (count1 & count0) | (pb_changed & pb_state);

//     static uint8_t side = 0;
//     switch (num)
//     {
//     case 0:
//     {
//         if (side == 0)
//         {
//             spi_write(0b01111111);
//         }
//         else
//         {
//             spi_write(0b10111110);
//         }
//     }
//     case 1:
//     {
//         if (side == 0)
//         {
//             spi_write(0b01111111);
//         }
//         else
//         {
//             spi_write(0b11101011);
//         }
//     }
//     case 2:
//     {
//         if (side == 0)
//         {
//             spi_write(0b00111110);
//         }
//         else
//         {
//             spi_write(0b11111111);
//         }
//     }
//     case 3:
//     {
//         if (side == 0)
//         {
//             spi_write(0b01101011);
//         }
//         else
//         {
//             spi_write(0b11111111);
//         }
//     }
//     }
//     side = !side;
// }
