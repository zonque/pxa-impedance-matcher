#ifndef _SERIAL_H
#define _SERIAL_H

#define BIT(nr)         (1UL << (nr))
#define UART_THR	0x00
#define UART_LSR	0x14
#define   LSR_THRE	BIT(5)

void __putch(char);
#endif
