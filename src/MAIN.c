// // code from saturday the 28/05.
// // using btn states machine in playback, cant reacsess loop once exited?????

// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <avr/interrupt.h>
// #include "uart.h"
// #include "timer.h"
// #include "buzzer.h"
// #include "spi.h"
// #include "buttons.h"
// #include "sequence_gen.h"
// #include "adc.h"

// void adc(void)
// {

//     uart_init(); // Initialize the UART (serial port)
// }

// typedef enum // enum for state machine
// {
//     playback,
//     btn1,
//     btn2,
//     btn3,
//     btn4,
//     wait,
//     wait_input,
//     checker,
//     game_over,
//     finished_round,

// } states; // enum for state machine

// uint8_t num;

// volatile uint8_t message = 0;

// void print_rand(uint16_t *value)
// {
//     uint8_t STEP = *value;
//     STEP = STEP & 0b11;
//     switch (STEP)
//     {
//     case (0b00):
//         num = 1;
//         break;
//     case (0b01):
//         num = 2;
//         break;
//     case (0b10):
//         num = 3;
//         break;
//     case (0b11):
//         num = 4;
//         break;
//     }
// }
// uint32_t STATE_LSFR = 0x11271965; // seed for LSFR
// uint32_t next(void)

// {
//     uint32_t BIT = STATE_LSFR & 0b01;
//     STATE_LSFR >>= 1;

//     if (BIT == 1)
//     {
//         STATE_LSFR ^= 0xEC2023CAB;
//     }

//     return STATE_LSFR;
// }
// volatile uint8_t tplay = 0;
// volatile uint32_t current_time = 0;
// uint32_t start_time = 0;
// uint8_t pb_previous = 0xFF;
// uint8_t pb_current = 0xFF;
// volatile uint8_t pb_state = 0xFF;
// uint8_t score10 = 0;
// uint8_t score1 = 0;
// uint8_t disp_score;

// uint8_t score0s[11] =
//     {
//         0xFF,
//         0b00001000, // 0
//         0b01101011, // 1
//         0b01000100, // 2
//         0b01000001, // 3
//         0b00100011, // 4
//         0b00010001, // 5
//         0b00010000, // 6
//         0b00110100, // 7
//         0b00000000, // 8
//         0b00000001, // 9}
// };

// uint16_t score1s[11] =
//     {
//         0xFF,
//         0b10001000, // 0
//         0b11101011, // 1
//         0b11000100, // 2
//         0b11000001, // 3
//         0b10010001, // 4
//         0b00100011, // 5
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
//     cli();           // Disable interrupts
//     uart_init();     // Initialise UART (serial port)
//     timer_init();    // Initialise timer (for UART)
//     buzzer_init();   // Initialise buzzer (PWM)
//     buttons_init();  // Initialise buttons (for user input)
//     spi_init();      // Initialise SPI
//     spi_write(0xFF); // Clear SPI (MOSI) buffer
//     uart_init();     // Initialize the UART (serial port)

//     // PORTB.OUTSET = PIN2_bm; // UART thing :) (I think)
//     TCB0.INTCTRL = TCB_CAPT_bm;                // Enable interrupts for TCB0
//     sei();                                     // Enable interrupts
//     uint8_t round_count = 0;                   // Variable to store the current round
//     states state = playback;                   // Initial state
//     uint8_t game = 0;                          // Variable to store if the game is running or not
//     uint8_t guess;                             // Variable to store the current guess
//     uint8_t counted = 0;                       // Variable to store if the current round has been counted
//     uint8_t finished = 0;                      // Variable to store if the current round has been finished
//     uint8_t sequence_index = 0;                // Variable to store the current index of the sequence
//     uint8_t sequence_finish = 0;               // Variable to store if the sequence has finished playing
//     uint8_t allow_updating_playback_delay = 1; // Variable to store if the playback delay should be updated
//     uint8_t play = 0;                          // Variable to store if the tone should be playing
//     uint16_t playback_delay;                   // Variable to store the playback delay
//     uint8_t game_index = 0;                    // Variable to store the current index of the game
//     while (1)
//     {
//         if (allow_updating_playback_delay)
//         {
//             playback_delay = (1750 * ADC0.RESULT >> 8) + 250; // Calculate the playback delay
//         }
//         // does button press, deboucing
//         uint8_t pb_changed = pb_current ^ pb_previous; // Check if any buttons have changed
//         uint8_t pb_falling = pb_changed & pb_previous; // Check if any buttons are falling
//         pb_previous = pb_current;                      // Update the previous button state
//         pb_current = pb_state;                         // Update the current button state

//         switch (state)
//         {

//         case playback:
//         {
//             for (uint8_t i; i <= (sequence_index + 1); i++)
//             {

//                 uart_puts("Creating next num\n"); // Print the current state
//                 uint16_t random_number = next();  // Generate a random number
//                 print_rand(&random_number);       // Print the random number
//             }
//             if (game == 1)
//             {
//                 state = checker;
//             }
//             // uart_puts("Playing Tone...\n"); // Print the current statesh
//             {
//                 switch (num)
//                 {
//                 case 1:
//                     uart_puts("Playing Tone 1\n"); // Print the current state
//                     state = btn1;
//                     break;
//                 case 2:
//                     uart_puts("Playing Tone 2\n"); // Print the current state
//                     state = btn2;
//                     break;
//                 case 3:
//                     uart_puts("Playing Tone 3\n"); // Print the current state
//                     state = btn3;
//                     break;
//                 case 4:
//                     uart_puts("Playing Tone 4\n"); // Print the current state
//                     state = btn4;
//                     break;
//                 }
//             }

//             break;
//         }
//         case wait_input:
//         {
//             if (counted > round_count)
//             {
//                 state = finished_round;
//                 break;
//             }
//             if (counted <= round_count)
//             {
//                 game = 1;
//                 guess = 0;
//                 if (message == 0)
//                 {
//                     uart_puts("Waiting Input\n");
//                     message = 1;
//                 }
//                 if (pb_falling & PIN4_bm)
//                 {
//                     message = 0;
//                     state = btn1;
//                 }
//                 if (pb_falling & PIN5_bm)
//                 {
//                     message = 0;
//                     state = btn2;
//                 }
//                 if (pb_falling & PIN6_bm)
//                 {
//                     message = 0;
//                     state = btn3;
//                 }
//                 if (pb_falling & PIN7_bm)
//                 {
//                     message = 0;
//                     state = btn4;
//                 }
//                 break;
//             }
//             break;
//         }
//         case btn1:
//         {
//             allow_updating_playback_delay = 0;
//             uart_puts("btn1\n");
//             start_time = current_time;
//             TCA0.SINGLE.PERBUF = 9487;
//             TCA0.SINGLE.CMP0BUF = 4743;
//             PORTB.DIRSET = PIN0_bm;
//             tplay = 1; // set which tone is playing
//             if (game == 1)
//             {
//                 guess = 1;
//             }
//             state = wait;

//             break;
//         }

//         case btn2:
//         {
//             allow_updating_playback_delay = 0;
//             uart_puts("btn2\n");
//             TCA0.SINGLE.PERBUF = 11288;
//             TCA0.SINGLE.CMP0BUF = 5644;
//             PORTB.DIRSET = PIN0_bm;
//             tplay = 2; // set which tone is playing
//             if (game == 1)
//             {
//                 guess = 2;
//             }
//             start_time = current_time;
//             state = wait;
//             break;
//         }

//         case btn3:
//         {
//             allow_updating_playback_delay = 0;
//             uart_puts("btn3\n");
//             TCA0.SINGLE.PERBUF = 12654;
//             TCA0.SINGLE.CMP0BUF = 6327;
//             PORTB.DIRSET = PIN0_bm;
//             tplay = 3; // set which tone is playing
//             if (game == 1)
//             {
//                 guess = 3;
//             }
//             start_time = current_time;
//             state = wait;
//             break;
//         }

//         case btn4:
//         {
//             allow_updating_playback_delay = 0;
//             uart_puts("btn4\n");
//             TCA0.SINGLE.PERBUF = 14242;
//             TCA0.SINGLE.CMP0BUF = 7121;
//             PORTB.DIRSET = PIN0_bm;
//             tplay = 4; // set which tone is playing
//             if (game == 1)
//             {
//                 guess = 4;
//             }
//             start_time = current_time;
//             state = wait;
//             break;
//         }

//         case wait:
//         {
//             if (tplay != 0)
//             {
//                 uart_puts("Waiting with buzzer / LED\n"); // Print the current state
//             }
//             else
//             {
//                 uart_puts("Waiting without buzzer / LED\n"); // Print the current state
//             }
//             if ((current_time - start_time) >= (playback_delay >> 1))
//             {
//                 PORTB.DIRCLR = PIN0_bm;
//                 tplay = 0;
//                 if ((current_time - start_time) >= playback_delay)
//                 {
//                     play = 1;
//                 }
//             }
//             if (play == 1)
//             {
//                 uart_puts("play = 1\n"); // Print the current state
//                 allow_updating_playback_delay = 1;
//                 if (game == 0)
//                 {
//                     sequence_index++;
//                     uart_puts("game = 0 \n");
//                     if (sequence_index > round_count)
//                     {
//                         uart_puts("sequence finished\n");
//                         sequence_finish = 1;
//                         sequence_index = 0;
//                         state = wait_input;
//                         play = 0;
//                     }
//                     else if (sequence_index <= round_count)
//                     {
//                         uart_puts("sequence not finished\n");
//                         play = 0;
//                         state = playback;
//                     }
//                 }
//                 else if (game == 1)
//                 {
//                     if (sequence_finish)
//                     {
//                         play = 0;
//                         message = 0;
//                         state = checker;
//                         allow_updating_playback_delay = 1;
//                     }
//                 }
//                 else if (game == 2)
//                 {
//                     if (finished == 1)
//                     {
//                         play = 0;
//                         uart_puts("Game Over\n"); // Print the game over message
//                         uart_puts("\n");
//                         sequence_finish = 0;
//                         sequence_index = 0;
//                         game = 2;
//                         round_count = 0;
//                         message = 0;

//                         allow_updating_playback_delay = 1;
//                         if (disp_score == 0)
//                         {
//                             score10 = 0;
//                             score1 = 0;
//                             uart_puts('creating the sequence\n');
//                             state = playback;
//                             finished = 0;
//                             game = 0;
//                         }
//                         else if (disp_score == 1)
//                         {
//                             start_time = current_time;
//                             uart_puts("Your final score is: ");
//                             uart_putc(score10 + '0');
//                             uart_putc(score1 + '0');
//                             uart_puts("\n");
//                             tplay = 8;
//                             disp_score = 0;
//                         }
//                     }
//                     else if (finished == 0)
//                     {
//                         if (disp_score == 0)
//                         {
//                             play = 0;
//                             uart_puts("New Round starting...\n"); // Print the game over message
//                             uart_puts("\n");
//                             counted = 0;
//                             sequence_index = 0;
//                             sequence_finish = 0;
//                             game = 0;
//                             round_count++;
//                             message = 0;
//                             state = playback;
//                             allow_updating_playback_delay = 1;
//                         }
//                         else if (disp_score == 1)
//                         {
//                             start_time = current_time;
//                             uart_puts("Your current score is: ");
//                             uart_putc(score10 + '0');
//                             uart_putc(score1 + '0');
//                             uart_puts("\n");
//                             tplay = 8;
//                             disp_score = 0;
//                         }
//                     }
//                 }
//             }

//             break;
//         }

//         case checker:
//         {
//             uart_puts("Check\n");

//             if (counted <= round_count)
//             {
//                 for (uint8_t i; i <= sequence_index; i++)
//                 {
//                     uint16_t random_number = next(); // Generate a random number
//                     print_rand(&random_number);      // Print the random number
//                 }
//                 if (guess != num)
//                 {
//                     sequence_index =
//                         state = game_over;
//                 }
//                 if (guess == num)
//                 {
//                     counted++;
//                     sequence_index++;

//                     state = wait_input;
//                     guess = 0;
//                 }
//             }
//             break;
//         }
//         case game_over:
//         {
//             uart_puts("Game Over\n");                       // Print the game over message
//             uart_puts("Give up, Get the soldering iron\n"); // Print the game over message
//             game = 2;
//             finished = 1;
//             counted = 0;
//             sequence_index = 0;
//             disp_score = 1;
//             round_count = 0;
//             start_time = current_time;
//             tplay = 7;
//             state = wait;
//             break;
//         }

//         case finished_round:
//         {
//             score1++;
//             if (score1 == 10)
//             {
//                 score10++;
//                 score1 = 0;
//             }
//             game = 2;
//             finished = 0;
//             tplay = 6;
//             start_time = current_time;
//             disp_score = 1;
//             uart_puts("congrats you made it\n");
//             uart_puts("\n");
//             state = wait;
//             break;
//         }
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

//     // score display.
//     case 8:
//         if (side == 1)
//         {
//             if (score10 < 1)
//             {
//                 spi_write(score1s[0]);
//             }
//             else
//             {
//                 spi_write(score1s[score10]);
//             }
//         }
//         else
//             spi_write(score0s[score1]);
//     }
//     side = !side;
//     TCB0.INTFLAGS = TCB_CAPT_bm;
// }