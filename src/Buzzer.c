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
