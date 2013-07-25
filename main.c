#include "types.h"
#include "atags.h"

static void putch(char c)
{
	int i;
	volatile __u32 *reg = (volatile __u32 *) 0x40100000;

	*reg = c;

	for (i = 0; i < 100; i++)
		asm("nop");
}

static void putstr(const char *s)
{
	while (*s) {
		putch(*s);
		if (*s == '\n')
			putch('\r');
		s++;
	}
}

extern void *_binary____linux_arch_arm_boot_zImage_start;
extern void *_binary____linux_dtb_start;

void init(__u32 dummy, __u32 machid, const struct tag *tags)
{
	int i;
	__u32 system_rev = 0;
	const struct tag *t;
	void (*start_kernel)(__u32 dummy, __u32 machid, void *dtb);
	void *zimage = _binary____linux_arch_arm_boot_zImage_start;
	void *dtb = _binary____linux_dtb_start;

	putstr("++ Impedance Matcher (3rd stage loader) ++\n");

	/* walk the atags to determine the system revision */
	for_each_tag(t, tags) {
		switch (t->hdr.tag) {
			case ATAG_REVISION:
				system_rev = t->u.rev.rev;
				break;
		}
	}

	putstr("Booting into Linux kernel ...\n");

	start_kernel = zimage;
	start_kernel(dummy, 0xffff, 0);
	//start_kernel(dummy, 0xffff, dtb);
}

