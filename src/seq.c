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
//     uint8_t buzzer;
//     while (1)
//     {
//         uart_puts("loopa\r\n");
//         if (buzzer == 1)
//         {
//             PORTB.OUTSET = PIN0_bm;
//         }
//         else
//         {
//             PORTB.OUTCLR = PIN0_bm;
//         }

//         if (PORTA.IN & PIN4_bm)
//         {
//             buzzer = 0;
//         }
//         if (PORTA.IN & PIN5_bm)
//         {
//             buzzer = 1;
//         }
//     }
// }
