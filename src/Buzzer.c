// Buzzer init
// Includes
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
void buzzer_init(void)
{
    // Configure the buzzer output
    PORTB.DIRSET = PIN0_bm;
    PORTB.OUTCLR = PIN0_bm;
    // CLK_PER select, /1 prescaler (3.33 MHz)
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    // Single-slope PWM mode, WO2 enable (PB5, LED DISP DP)
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    // Enable TCA0
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

// volatile uint32_t current_time = 0;
// uint32_t start_time = 0;
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
//     while (1)
//     {
//         if (current_time < 1500)
//         {
//             uart_puts("timer less than 1500\n");
//         }
//         if (current_time >= 2500)
//         {
//             uart_puts("timer greater than 2500\n");
//         }
//     }
// }

// ISR(TCB0_INT_vect)
// {
//     current_time++;
// }