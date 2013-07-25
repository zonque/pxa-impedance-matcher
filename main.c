#include "types.h"
#include "atags.h"

static void putch(char c)
{
	volatile __u32 *reg = (volatile __u32 *) 0x40100000;
	*reg = c;
}

void init(unsigned long dummy, unsigned long machid, const struct tag *tags)
{
	int i;

	while (1) {
		if (machid == 0xffff)
			putch('y');
		else
			putch('n');

		for (i = 0; i < 100000; i++)
			asm("nop");
	}
}

