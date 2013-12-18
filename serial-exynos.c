#include "types.h"
#include "register.h"
#include "serial.h"

#define EXYNOS_BASE_UART	0x13800000
#define TX_FIFO_FULL_MASK	(1 << 24)

#define UART_BAUDRATE_115200	115200

union br_rest {
	unsigned short	slot;		/* udivslot */
	unsigned char	value;		/* ufracval */
};

struct s5p_uart {
	unsigned int	ulcon;
	unsigned int	ucon;
	unsigned int	ufcon;
	unsigned int	umcon;
	unsigned int	utrstat;
	unsigned int	uerstat;
	unsigned int	ufstat;
	unsigned int	umstat;
	unsigned char	utxh;
	unsigned char	res1[3];
	unsigned char	urxh;
	unsigned char	res2[3];
	unsigned int	ubrdiv;
	union br_rest	rest;
	unsigned char	res3[0xffd0];
};

static inline struct s5p_uart *s5p_get_base_uart(int dev_index)
{
	u32 offset = dev_index * sizeof(struct s5p_uart);
	return (struct s5p_uart *)(EXYNOS_BASE_UART + offset);
}

static inline int s5p_uart_divslot(void)
{
	return 0;
}

static int serial_err_check(const int dev_index, int op)
{
	struct s5p_uart *const uart = s5p_get_base_uart(dev_index);
	unsigned int mask;

	/*
	 * UERSTAT
	 * Break Detect	[3]
	 * Frame Err	[2] : receive operation
	 * Parity Err	[1] : receive operation
	 * Overrun Err	[0] : receive operation
	 */
	if (op)
		mask = 0x8;
	else
		mask = 0xf;

	return readl((unsigned int)&uart->uerstat) & mask;
}

/*
 * Output a single byte to the serial port.
 */
void __putch(char c)
{
	const int dev_index = UART_PORT;

	struct s5p_uart *const uart = s5p_get_base_uart(dev_index);

	/* wait for room in the tx FIFO */
	while ((readl((unsigned int)&uart->ufstat) & TX_FIFO_FULL_MASK)) {
		if (serial_err_check(dev_index, 1))
			return;
	}

	writeb(c, (unsigned int)&uart->utxh);

	/* If \n, also do \r */
	if (c == '\n')
		__putch('\r');
}
