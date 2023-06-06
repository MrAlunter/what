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

// volatile uint32_t current_time = 0;
// uint32_t start_time = 0;
// uint8_t pb_previous = 0xFF;
// uint8_t pb_current = 0xFF;
// volatile uint8_t pb_state = 0xFF;

// uint8_t sequenceI = 0;   // index of the sequence
// uint8_t round_count = 1; // number of steps in the sequence
// uint8_t guess = 0;       // index of the guess
// uint8_t value = 0;       // current step in the sequence
// uint16_t playback_delay;
// void buzzerOn(STEP)
// {
//     switch (STEP)
//     {
//     case 0:
//         // start_time = current_time;
//         TCA0.SINGLE.PERBUF = 9487;
//         TCA0.SINGLE.CMP0BUF = 4743;
//         PORTB.OUTSET = PIN0_bm;
//         break;
//     case 1:
//         // start_time = current_time;
//         TCA0.SINGLE.PERBUF = 11288;
//         TCA0.SINGLE.CMP0BUF = 5644;
//         PORTB.OUTSET = PIN0_bm;
//         break;
//     case 2:
//         // start_time = current_time;
//         TCA0.SINGLE.PERBUF = 12654;
//         TCA0.SINGLE.CMP0BUF = 6327;
//         PORTB.OUTSET = PIN0_bm;
//         break;
//     case 3:
//         // start_time = current_time;
//         TCA0.SINGLE.PERBUF = 14242;
//         TCA0.SINGLE.CMP0BUF = 7121;
//         PORTB.OUTSET = PIN0_bm;
//         break;
//     case 4:
//         TCA0.SINGLE.PERBUF = 0;
//         TCA0.SINGLE.CMP0BUF = 0;
//         PORTB.OUTCLR = PIN0_bm;
//         break;
//     }
// }

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
//                     value = next();             // Get the next step
//                     uart_putc(value + '0');     // Print the current step
//                     buzzerOn(value);            // Turn on the buzzer
//                     ledOn(value);               // Turn on the LED
//                     start_time = current_time;  // Set the start time
//                     playback_states = playwait; // Wait for the playback delay
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
//                 uart_puts("playback_states = playwait\n");

//                 if ((current_time - start_time) < (playback_delay >> 1))
//                 {
//                     buzzerOn(value);
//                     ledOn(value);
//                 }
//                 else if ((current_time - start_time) < (playback_delay >> 1)){

//                 }
//                     if ((current_time - start_time) > (playback_delay >> 1))
//                     {
//                         uart_puts("buzzer off\n");
//                         buzzerOn(4);
//                         ledOn(4);
//                         if ((current_time - start_time) > (playback_delay))
//                         {
//                             uart_puts("change after wait\n");
//                             playback_states = maker;
//                             sequenceI++;
//                         }
//                     }
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
//             uart_puts("buttonState = wait\n");
//             if (pb_rising)
//             {
//                 button_release = 0;
//             }
//             if (start_time - current_time > (((1750 * ADC0.RESULT >> 8) + 250) >> 1) && button_release == 0)
//             {
//                 buzzerOn(4);
//                 ledOn(4);
//                 if (start_time - current_time > (((1750 * ADC0.RESULT >> 8) + 250)))
//                 {
//                     buttonState = wait;
//                 }
//             }
//             break;

//         case button1:
//             uart_puts("buttonState = button1\n");
//             buzzerOn(0);
//             ledOn(0);
//             buttonState = wait;
//             break;

//         case button2:
//             uart_puts("buttonState = button2\n");
//             buzzerOn(1);
//             ledOn(1);
//             buttonState = wait;
//             break;

//         case button3:
//             uart_puts("buttonState = button3\n");
//             buzzerOn(2);
//             ledOn(2);
//             buttonState = wait;
//             break;

//         case button4:
//             uart_puts("buttonState = button4\n");
//             buzzerOn(3);
//             ledOn(3);
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
// }
