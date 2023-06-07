#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

void timer_init(void)
{
    cli();
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                // Set period for 10 ms (16700 clocks @ 1.67 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;      // Enable CAPT interrupt source
    TCB0.CTRLA = TCB_CLKSEL_DIV1_gc | TCB_ENABLE_bm;
    sei();
}
