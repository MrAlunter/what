#include <avr/io.h>

#include <stdint.h>

void uart_init(void)
{
    // 9600 baud
    USART0.BAUD = 1389;
    PORTB.DIRSET = PIN2_bm;
    // Enable receive complete interrupt
    USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}

void uart_putc(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
        ; // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

void uart_puts(char *string)
{
    // String stored at string
    // iterates through each character in string and send each one
    // string ends when there is NUL charactor eg' \0'
    while (*string != '\0')
    {
        // Transmit string
        uart_putc(*string);  // Character currently stored at string.
        string = string + 1; // increments string
    }
}