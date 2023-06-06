// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <avr/interrupt.h>
// // #include "uart.h"
// // #include "timer.h"
// // #include "buzzer.h"
// // #include "SPI.h"
// // #include "buttons.h"
// // #include "adc.h"
// void buttons_init(void)
// {
//     // Enable pull-up resistors for PBs
//     PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
//     PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
//     PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
//     PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
// }

// void port_init(void)
// {
//     // BUZZER (PIN0), USART0 TXD (PIN2)
//     PORTB.DIRSET = PIN0_bm | PIN2_bm;
// }

// void pwm_init(void)
// {
//     // Enable output override on PB0.
//     TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;

//     // PWM at 440Hz - initially OFF (0% duty cycle)
//     TCA0.SINGLE.PER = 7576;
//     TCA0.SINGLE.CMP0 = 0;

//     // Enable TCA0
//     TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
// }

// void timers_init(void)
// {
//     // 1ms interrupt for elapsed time
//     TCB0.CCMP = 3333;
//     TCB0.INTCTRL = TCB_CAPT_bm;
//     TCB0.CTRLA = TCB_ENABLE_bm;

//     // 4ms interrupt for PB sampling
//     TCB1.CCMP = 3333 * 4;
//     TCB1.INTCTRL = TCB_CAPT_bm;
//     TCB1.CTRLA = TCB_ENABLE_bm;
// }

// void uart_init(void)
// {
//     // 9600 baud
//     USART0.BAUD = 1389;

//     // Enable receive complete interrupt
//     USART0.CTRLA = USART_RXCIE_bm;
//     USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
// }
// uint8_t num = 0;

// typedef enum
// {
//     CUNT,
//     FUCK,
//     DOG,
// } State;

// void main(void)
// {
//     cli();
//     buttons_init();
//     port_init();
//     pwm_init();
//     timers_init();
//     uart_init();
//     sei();
//     while (1)
//     {
//         if (num == 1)
//         {
//             uart_puts("maybe not\r\n");
//         }
//         else
//         {
//             uart_puts("your mom gay :(\r\n");
//         }
//     }
// }

// ISR(USART0_RXC_vect)
// {
//     uint8_t rx_data = USART0.RXDATAL;
//     switch (rx_data)
//     {
//     case 'p':
//         State = CUNT;
//         break;
//     case 's':
//         State = DOG;
//         break;
//     case ',':
//         State = FUCK;
//         break;
//     case '.':
//         uart_puts("octave lower");
//         break;
//     }
// }
