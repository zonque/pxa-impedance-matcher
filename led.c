#include "types.h"
#include "led.h"

static inline void writel(__u32 val, __u32 addr)
{
	*(volatile __u32 *) addr = val;
}

static inline __u32 readl(__u32 addr)
{
	return *(volatile __u32 *) addr;
}

static void wait(__u32 ticks)
{
	__u32 v;

	writel(0, 0x40A00010);

	do {
		v = readl(0x40A00010);
	} while (ticks > v);
}

static void led_init(void)
{
	writel(0, 0x40e10420);		/* GPIO35 */
	writel(0, 0x40e10424);		/* GPIO36 */
	writel(0x18, 0x40e00010);	/* GPDR1 */
}

static void led_set(__u32 index, __u32 state)
{
	__u32 v = 1 << (index ? 3 : 4);

	if (state)
		writel(v, 0x40e0001c);
	else
		writel(v, 0x40e00028);
}

void led_panic(void)
{
	int i;

	led_init();

	for (i = 0;; i++) {
		led_set(0, i & 1);
		led_set(1, ~i & 1);
		wait(500000);
	}
}
