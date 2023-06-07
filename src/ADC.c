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

void adc_init()
{
    ADC0.CTRLA = ADC_ENABLE_bm;
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    ADC0.CTRLE = 64;
    ADC0.CTRLF = ADC_FREERUN_bm;
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;
}

// uint16_t playback_delay;

// int main(void)
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
//         char buffer[4];
//         uint16_t playback_delay;

//         playback_delay = ((((1750 * ADC0.RESULT >> 8) + 250)));
//         // Convert playback_delay to string
//         sprintf(buffer, "%d\n", playback_delay);

//         // Send the string via UART
//         uart_puts(buffer);
//         ;
//     }
// }