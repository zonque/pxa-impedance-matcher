#include "types.h"
#include "atags.h"
#include "print.h"

extern __u32 _binary____linux_arch_arm_boot_zImage_start;
extern __u32 _binary____linux_dtb_start;

void init(__u32 dummy, __u32 machid, const struct tag *tags) __attribute__ ((noreturn));
void init(__u32 dummy, __u32 machid, const struct tag *tags)
{
	__u32 system_rev = 0;
	const struct tag *t;
	void (*start_kernel)(__u32 dummy, __u32 machid, void *dtb) __attribute__ ((noreturn));
	void *zimage = &_binary____linux_arch_arm_boot_zImage_start;
	void *dtb = &_binary____linux_dtb_start;

	putstr("++ Impedance Matcher (3rd stage loader) ++\n");

	/* walk the atags to determine the system revision */
	for_each_tag(t, tags) {
		switch (t->hdr.tag) {
			case ATAG_REVISION:
				system_rev = t->u.rev.rev;
				break;
		}
	}

	putstr("Booting into Linux kernel @0x");
	printhex((__u32) zimage);
	putstr(" with DTB blob @0x");
	printhex((__u32) dtb);
	putstr("\n");

	start_kernel = zimage;
	start_kernel(0, 0xffff, dtb);
}

