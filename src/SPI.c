#include <avr/io.h>
#include <avr/interrupt.h>
void spi_init()
{
    cli();
    // ENABLE DISP LATCH-PA1
    PORTA.OUTCLR = PIN1_bm;
    PORTA.DIRSET = PIN1_bm;

    // Display enable
    PORTB.OUTSET = PIN1_bm;
    PORTB.DIRSET = PIN1_bm;

    // SET PORTMUX SO SPI0 IS ON PINS PC0-PC3
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;

    // ENABLE PC0 AND PC2 AS OUTPUTS(SPI CLK AND SPI MOSI)
    PORTC.DIRSET = (PIN0_bm | PIN2_bm);

    //
    SPI0.CTRLA = SPI_MASTER_bm;

    // SPI MODE0, SAMPLE ON RISING EDGE
    SPI0.CTRLB = SPI_MODE_0_gc | SPI_SSD_bm;

    // ENABLE SPI INTERRUPT
    SPI0.INTCTRL = SPI_IE_bm;

    // ENABLE SPI
    SPI0.CTRLA |= SPI_ENABLE_bm;
    sei();
}
void spi_write(uint8_t b)
{
    SPI0.DATA = b;
}

// SPI ISR
ISR(SPI0_INT_vect)
{

    PORTA.OUTSET = PIN1_bm;
    PORTA.OUTCLR = PIN1_bm;
}