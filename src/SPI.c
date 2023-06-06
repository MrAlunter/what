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
uint8_t led; // LED number

void ledOn(STEP)
{
    switch (STEP)
    {
    case 0:
        led = 0;
    case 1:
        led = 1;
    case 2:
        led = 2;
    case 3:
        led = 3;
    }
}

ISR(TCB1_INT_vect)
{
    static uint8_t side = 0;
    switch (led) // button presses.
    {
    case 0:
        if (side == 0)
            spi_write(0b01111111);
        else
            spi_write(0b10111110);
        break;
    case 1:
        if (side == 0)
            spi_write(0b11101011);
        else
            spi_write(0b01111111);
    case 2:
        if (side == 0)
            spi_write(0b00111110);
        else
            spi_write(0b11111111);
    case 3:
        if (side == 0)
            spi_write(0b01101011);
        else
            spi_write(0b11111111);
    case 4:
        if (side == 0)
            spi_write(0b01111111);
        else
            spi_write(0b11111111);
    }
}

// SPI ISR
ISR(SPI0_INT_vect)
{

    PORTA.OUTSET = PIN1_bm;
    PORTA.OUTCLR = PIN1_bm;
}