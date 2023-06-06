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

// typedef enum // enum for state machine
// {
//     maker,
//     playback,
//     wait_input,
//     pass,
//     fail,
//     btn1,
//     btn2,
//     btn3,
//     btn4,
//     wait,

// } states; // enum for state machine

// volatile uint8_t message = 0;
// uint8_t num;
// uint32_t STATE_LSFR = 0x11271965; // seed for LSFR
// uint8_t STEP;

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

// volatile uint8_t tplay = 0;
// volatile uint32_t current_time = 0;
// uint32_t start_time = 0;
// uint8_t pb_previous = 0xFF;
// uint8_t pb_current = 0xFF;
// volatile uint8_t pb_state = 0xFF;
// uint8_t score10 = 0;
// uint8_t score1 = 1;
// uint8_t disp_score;

// uint8_t score0s[11] =
//     {
//         0xFF,       // nothing
//         0b00001000, // 0
//         0b01101011, // 1
//         0b01000100, // 2
//         0b01000001, // 3
//         0b00100011, // 4
//         0b00010001, // 5
//         0b00010000, // 6
//         0b00110100, // 7
//         0b00000000, // 8
//         0b00000001, // 9
// };

// uint16_t score1s[11] =
//     {
//         0xFF,       // nothing
//         0b10001000, // 0
//         0b11101011, // 1
//         0b11000100, // 2
//         0b11000001, // 3
//         0b10010001, // 4
//         0b10100011, // 5
//         0b10010000, // 6
//         0b10110100, // 7
//         0b10000000, // 8
//         0b10000001, // 9}
// };

// uint16_t arte[10] = // SPI array for btn lines.
//     {
//         0xFF,       // nothing
//         0b01111111, // nothingthing side 0
//         0b11111111, // nothingthing side 1
//         0b10111110, // btn 1 side 1
//         0b11101011, // btn 2 side 1
//         0b00111110, // btn 3 side 0
//         0b01101011, // btn 4 side 0
// };
// uint16_t gamer[4] =
//     {
//         0b00000000, // all side 0
//         0b10000000, // all side 1
//         0b01110111, // wrong guess side 0
//         0b11110111, // wrong guess side 1
// };

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
//     uint8_t round_count = 1;    // Variable to store the current round
//     states state = maker;       // Initial state
//     uint8_t game = 0;           // Variable to store if the game is running or not
//     uint8_t guess;              // Variable to store the current guess
//     uint8_t sequence_index = 0; // Variable to store the current index of the sequence
//     uint8_t play = 0;           // Variable to store if the tone should be playing
//     uint8_t game_index = 0;     // Variable to store the current index of the game
//     uint8_t finished = 0;       // Variable to store if the game has finished
//     uint8_t total_score;        // Variable to store the total score
//     uint8_t button_release = 1; // Variable to store if the button has been released

//     while (1)
//     {

//         uint8_t pb_changed = pb_current ^ pb_previous; // Check if any buttons have changed
//         uint8_t pb_falling = pb_changed & pb_previous; // Check if any buttons are falling
//         uint8_t pb_rising = pb_changed & pb_current;   // Check if any buttons are rising
//         if (button_release == 0)
//         {
//             if (pb_rising)
//             {
//                 button_release = 1;
//             }
//         }
//         pb_previous = pb_current; // Update the previous button state
//         pb_current = pb_state;    // Update the current button state

//         if (pb_falling & PIN4_bm)
//         {
//             start_time = current_time;
//             TCA0.SINGLE.PERBUF = 9487;
//             TCA0.SINGLE.CMP0BUF = 4743;
//             PORTB.OUTSET = PIN0_bm;
//             start_time = current_time;
//             // time = 1;
//         }
//         if ((current_time - start_time) > 500)
//         {
//             TCA0.SINGLE.CMP0BUF = 0;
//         }
//     }
// }

// //         if (current_time - start_time > 500)
// //         {
// //             TCA0.SINGLE.CMP0BUF = 0;
// //         }

// //         STATE_LSFR = 0x11271965;  // seed for LSFR
// //         switch (state)
// //         {
// //         case maker:
// //             uart_puts("Maker\n"); // Print the current state
// //             if (game == 0)
// //             {
// //                 for (uint8_t i = 0; i <= sequence_index; i++)
// //                 {
// //                     uart_puts("SL\n");
// //                     next(); // Generate a random number
// //                     uart_puts("STEP = ");
// //                     uart_putc(STEP + '0');
// //                     uart_puts("\n");

// //                     switch (STEP)
// //                     {
// //                     case (0):
// //                     {
// //                         uart_puts("num gen = 1");
// //                         num = 1;
// //                         break;
// //                     }
// //                     case (1):
// //                     {
// //                         uart_puts("num gen = 2");
// //                         num = 2;
// //                         break;
// //                     }
// //                     case (2):
// //                     {
// //                         uart_puts("num gen = 3");
// //                         num = 3;
// //                         break;
// //                     }
// //                     case (3):
// //                     {
// //                         uart_puts("num gen = 4");
// //                         num = 4;
// //                         break;
// //                     }
// //                         uart_puts("num gen = ");
// //                         uart_putc(num + '0');
// //                         uart_puts("\n");
// //                     }
// //                     state = playback;
// //                 }
// //             }
// //             else if (game == 1)
// //             {
// //                 for (uint8_t i = 0; i <= game_index; i++)
// //                 {
// //                     uart_puts("GL\n");
// //                     num = next(); // Generate a random number
// //                     uart_putc(num + '0\n');
// //                 }
// //                 if (guess == num)
// //                 {
// //                     uart_puts("guess = num\n");
// //                     game_index++;
// //                     state = pass;
// //                 }
// //                 else if (guess != num)
// //                 {
// //                     uart_puts("guess != num\n");
// //                     state = fail;
// //                 }
// //             }
// //             break;

// //         case playback:
// //         {
// //             uart_puts("Playback\n"); // Print the current state
// //             switch (num)
// //             {
// //             case 1:
// //                 uart_puts("Playing Tone 1\n"); // Print the current state
// //                 state = btn1;
// //                 button_release = 0;
// //                 break;
// //             case 2:
// //                 uart_puts("Playing Tone 2\n"); // Print the current state
// //                 state = btn2;
// //                 button_release = 0;
// //                 break;
// //             case 3:
// //                 uart_puts("Playing Tone 3\n"); // Print the current state
// //                 state = btn3;
// //                 button_release = 0;
// //                 break;
// //             case 4:
// //                 uart_puts("Playing Tone 4\n"); // Print the current state
// //                 state = btn4;
// //                 button_release = 0;
// //                 break;
// //             }
// //             sequence_index++;
// //             break;
// //         }
// //         case btn1:
// //         {
// //             uart_puts("btn1\n"); // Print the current state
// //             start_time = current_time;
// //             TCA0.SINGLE.PERBUF = 9487;
// //             TCA0.SINGLE.CMP0BUF = 4743;
// //             PORTB.OUTSET = PIN0_bm;
// //             tplay = 1; // set which tone is playing
// //             guess = 0;
// //             state = wait;
// //             break;
// //         }
// //         case btn2:
// //         {
// //             uart_puts("btn2\n"); // Print the current state
// //             TCA0.SINGLE.PERBUF = 11288;
// //             TCA0.SINGLE.CMP0BUF = 5644;
// //             PORTB.OUTSET = PIN0_bm;
// //             tplay = 2; // set which tone is playing
// //             start_time = current_time;
// //             state = wait;
// //             guess = 1;
// //             break;
// //         }
// //         case btn3:
// //         {
// //             uart_puts("btn3\n"); // Print the current state
// //             TCA0.SINGLE.PERBUF = 12654;
// //             TCA0.SINGLE.CMP0BUF = 6327;
// //             PORTB.OUTSET = PIN0_bm;
// //             tplay = 3; // set which tone is playing
// //             start_time = current_time;
// //             state = wait;
// //             guess = 2;
// //             break;
// //         }
// //         case btn4:
// //         {
// //             uart_puts("btn4\n"); // Print the current state
// //             TCA0.SINGLE.PERBUF = 14242;
// //             TCA0.SINGLE.CMP0BUF = 7121;
// //             PORTB.OUTSET = PIN0_bm;
// //             tplay = 4; // set which tone is playing
// //             start_time = current_time;
// //             state = wait;
// //             guess = 3;
// //             break;
// //         }
// //         case wait:
// //         {

// //             if (message == 0)
// //             {
// //                 uart_puts("Waiting\n"); // Print the current state
// //                 message = 1;
// //             }

// //             else if (game == 0 || (game == 1 && (finished == (1 || 2))))
// //             {
// //                 button_release = 1;
// //             }

// //             if ((current_time - start_time >= (((1750 * ADC0.RESULT >> 8) + 250)) >> 1) && button_release == 1)
// //             {

// //                 uart_puts("Finished Waiting buzz off\n"); // Print the current state
// //                 TCA0.SINGLE.CMP0BUF = 0;
// //                 tplay = 0;
// //                 if (current_time - start_time >= (((1750 * ADC0.RESULT >> 8) + 250)))
// //                 {
// //                     uart_puts("Finished Waiting\n"); // Print the current state
// //                     play = 1;
// //                 }
// //             }
// //         }
// //             if (play == 1) // if tone is finished playing
// //             {
// //                 button_release = 0;
// //                 play = 0;
// //                 button_release = 0;
// //                 message = 0;
// //                 if (finished == 1)
// //                 {
// //                     uart_puts("ROUND Finished\n"); // Print the current state
// //                     if (disp_score == 1)
// //                     {
// //                         tplay = 8;
// //                         state = wait;
// //                         start_time = current_time;
// //                         disp_score = 0;
// //                     }
// //                     else if (disp_score == 0)
// //                     {

// //                         finished = 0;
// //                         game = 0;
// //                         state = maker;
// //                         sequence_index = 0;
// //                     }

// //                     break;
// //                 }
// //                 if (finished == 2)
// //                 {
// //                     uart_puts("GAME Finished\n"); // Print the current state
// //                     if (disp_score == 1)
// //                     {
// //                         tplay = 8;
// //                         state = wait;
// //                         start_time = current_time;
// //                         disp_score = 0;
// //                         break;
// //                     }
// //                     else if (disp_score == 0)
// //                     {
// //                         finished = 0;
// //                         game = 0;
// //                         state = maker;
// //                         sequence_index = 0;
// //                         game_index = 0;
// //                         round_count = 1;
// //                         break;
// //                     }
// //                     break;
// //                 }
// //                 play = 0;
// //                 if (game == 0) // if game is not running
// //                 {
// //                     play = 0;                          // reset play
// //                     uart_puts("Game Not Running\n");   // Print the current state
// //                     if (sequence_index >= round_count) // if sequence is complete
// //                     {
// //                         uart_puts("Sequence Complete\n"); // Print the current state
// //                         game = 1;                         // start game
// //                         sequence_index = 0;               // reset sequence index
// //                         state = wait_input;               // go to wait input state
// //                     }
// //                     else if (sequence_index < round_count)
// //                     {
// //                         uart_puts("Sequence Incomplete\n"); // Print the current state
// //                         state = maker;
// //                     }
// //                 }
// //                 else if (game == 1) // if game is running
// //                 {
// //                     state = maker;
// //                 }
// //                 break;
// //             }
// //         case wait_input:
// //         {
// //             if (message == 0)
// //             {

// //                 uart_puts("Waiting Input\n");
// //                 message = 1;
// //             }
// //             if (pb_falling & PIN4_bm)
// //             {
// //                 guess = 1;
// //                 message = 0;
// //                 state = btn1;
// //             }
// //             if (pb_falling & PIN5_bm)
// //             {
// //                 guess = 2;
// //                 message = 0;
// //                 state = btn2;
// //             }
// //             if (pb_falling & PIN6_bm)
// //             {
// //                 guess = 3;
// //                 message = 0;
// //                 state = btn3;
// //             }
// //             if (pb_falling & PIN7_bm)
// //             {
// //                 guess = 4;
// //                 message = 0;
// //                 state = btn4;
// //             }
// //             break;
// //         }
// //         case pass:
// //         {
// //             if (game_index < round_count)
// //             {
// //                 uart_puts("Correct Guess not fin\n");
// //                 state = wait_input;
// //             }
// //             else if (game_index >= round_count)
// //             {
// //                 score1++;
// //                 if (score1 == 10)
// //                 {
// //                     score1 = 0;
// //                     score10++;
// //                 }
// //                 total_score = score10 * 10 + score1;
// //                 uart_puts("You Win\n");
// //                 uart_puts("Your Score = ");
// //                 uart_putc(total_score + '0');
// //                 uart_putc("\n");
// //                 round_count++;
// //                 tplay = 6;
// //                 start_time = current_time;
// //                 game_index = 0;
// //                 disp_score = 1;
// //                 finished = 1;
// //                 state = wait;
// //             }
// //         }
// //         break;
// //         case fail:
// //         {
// //             uart_puts("You Fail\n");
// //             uart_puts("num = \n");   // Print the current state
// //             uart_putc(num + '0');    // Print the current state
// //             uart_puts("\n");         // Print the current state
// //             uart_puts("guess = \n"); // Print the current state
// //             uart_putc(guess + '0');  // Print the current state
// //             round_count = 0;         //
// //             sequence_index = 0;
// //             game_index = 0;
// //             score1 = 1;
// //             score10 = 0;
// //             game = 0;
// //             finished = 2;
// //             tplay = 7;
// //             start_time = current_time;
// //             disp_score = 1;
// //             state = wait;
// //         }
// //         }
// //     }
// // }

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
//     switch (tplay)
//     {
//         // button presses.
//     case 0:
//         if (side == 0)
//             spi_write(arte[0]);
//         else
//             spi_write(arte[0]);
//         break;
//     case 1:
//         if (side == 0)
//             spi_write(arte[2]);
//         else
//             spi_write(arte[3]);
//         break;
//     case 2:
//         if (side == 0)
//             spi_write(arte[2]);
//         else
//             spi_write(arte[4]);
//         break;
//     case 3:
//         if (side == 0)
//             spi_write(arte[5]);
//         else
//             spi_write(arte[2]);
//         break;
//     case 4:
//         if (side == 0)
//             spi_write(arte[6]);
//         else
//             spi_write(arte[2]);
//         break;

//         // correct or incorrect guesses
//     case 6:
//         if (side == 0)
//             spi_write(gamer[0]);
//         else
//             spi_write(gamer[1]);
//         break;
//     case 7:
//         if (side == 0)
//             spi_write(gamer[2]);
//         else
//             spi_write(gamer[3]);
//         break;

//     // score display.
//     case 8:
//         if (side == 0)
//             spi_write(score0s[score1]);

//         else if (side != 0)
//         {
//             if (score10 != 0)
//             {
//                 spi_write(score1s[score10]);
//             }
//         }

//         break;
//     }
//     side = !side;
//     TCB0.INTFLAGS = TCB_CAPT_bm;
// }