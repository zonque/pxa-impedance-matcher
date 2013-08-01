#include "types.h"
#include "register.h"
#include "serial.h"

/*
 * Whole heartedly copied from barebox's mach-mvebu debug code.  Thanks to
 * Thomas Petazzoni for the original code.
 */
void __putch(char c)
{
	/* Wait until there is space in the FIFO */
	while (!(readl(UART_BASE + UART_LSR) & LSR_THRE))
		;

	/* Send the character */
	writel(c, UART_BASE + UART_THR);

	/* Wait to make sure it hits the line */
	while (!(readl(UART_BASE + UART_LSR) & LSR_THRE))
		;
}
