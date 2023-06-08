// #include <stdint.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include "uart.h"

// uint32_t STATE_LSFR = 0x11271965;
// uint32_t STATE_HOLDER[1];
// uint32_t STEP = 0;
// uint32_t next(void)
// {
//     uint32_t BIT = STATE_LSFR & 0b01;
//     STATE_LSFR >>= 1;

//     if (BIT == 1)
//     {
//         STATE_LSFR ^= 0xE2023CAB;
//     }
//     STEP = STATE_LSFR & 0b11;
//     return STEP;
// }
// void main(void)
// {
//     cli();
//     uart_init();
//     sei();

//     while (1)
//     {
//         next();
//         uart_puts("STEP: ");
//         uart_putc(STEP + '0');
//         uart_puts("\n");
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