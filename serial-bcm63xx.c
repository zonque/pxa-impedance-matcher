#include "types.h"
#include "register.h"
#include "serial.h"

#define UART_IR_REG			0x10
#define UART_IR_STAT(x)			(1 << (x))
#define UART_IR_TXEMPTY			5
#define UART_FIFO_REG			0x14

/* Implementation ripped from arch/mips/bcm63xx/early_printk.c. Thanks to
 * Maxime Bizon for the original code.
 */
static void wait_xfered(void)
{
	unsigned int val;

	/* wait for any previous char to be transmitted */
	do {
		val = readl(UART_BASE + UART_IR_REG);
		if (val & UART_IR_STAT(UART_IR_TXEMPTY))
			break;
	} while (1);
}

void __putch(char c)
{
	wait_xfered();
	writel(c, UART_BASE + UART_FIFO_REG);
	wait_xfered();
}
