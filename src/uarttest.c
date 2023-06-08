// #include <stdint.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include "uart.h"
// void main(void)
// {
//     cli();
//     uart_init();
//     sei();

//     while (1)
//     {
//         ;
//     }
// }

// ISR(USART0_RXC_vect)
// {
//     uint8_t rx_data = USART0.RXDATAL;
//     switch (rx_data)
//     {
//     case 'p':
//         uart_puts("your mom gay lmao\n");
//         break;
//     case 's':
//         uart_puts("your mom not gay lmao");
//         break;
//     case ',':
//         uart_puts('octave up');
//         break;
//     case '.':
//         uart_puts('octave down');
//         break;
//     }
// }