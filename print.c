#include "types.h"
#include "print.h"

static inline void nop(int n)
{
	while (n--)
		asm("nop");
}

static void __putch(char c)
{
	*(volatile __u32 *) 0x40100000 = c;
	nop(100);
}

void putch(char c)
{
	__putch(c);
	if (c == '\n')
		__putch('\r');
}

static const char hexdigets[] =
	{ '0', '1', '2', '3', '4', '5', '6', '7',
	  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void printhex(__u32 u)
{
	int i;

	for (i = 0; i < 8; i++) {
		__u32 c = u >> (28 - (i * 4));
		putch(hexdigets[c & 0xf]);
	}
}

void putstr(const char *s)
{
	while (*s)
		putch(*s++);
}

