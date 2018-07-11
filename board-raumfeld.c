#include "atags.h"
#include "board.h"
#include "dtbs.h"
#include "libfdt.h"
#include "print.h"
#include "register.h"
#include "types.h"
#include "string.h"

extern u32 _binary_input_zImage_start;
extern u32 _binary_dtbs_bin_start;

static u32 system_rev;
struct board board;

struct raumfeld_board {
	u32		machid;
	u16		system_rev_upper;
	const char	*compatible;
	void		(*fixup_dtb)(const struct board *);
};

static void raumfeld_fixup_dtb_common(const struct board *board)
{
	int off;

	off = fdt_path_offset(board->dtb, "/");
	if (off < 0) {
		putstr("Unable to locate /!\n");
		return;
	}

	fdt_setprop_inplace_u32(board->dtb, off, "hw-revision", system_rev & 0xff);
}

static void raumfeld_fixup_dtb_controller(const struct board *board)
{
	int off;
	const char *node;

	raumfeld_fixup_dtb_common(board);

        /*
	 * Hardware revision 2 has the backlight regulator controlled
         * by an LT3593, earlier and later devices use PWM for that.
         */

	if (system_rev == 2)
		node = "/backlight-controller";
	else
		node = "/backlight-controller-pwm";

	off = fdt_path_offset(board->dtb, node);
	if (off < 0) {
		putstr("Unable to locate ");
		putstr(node);
		putstr("!\n");
	} else {
		/* override the string "disabled", and pad the string with zero-bytes */
		fdt_setprop_inplace(board->dtb, off, "status", "okay\0\0\0\0", 9);
	}
}

static struct raumfeld_board rboards[] = {
	/* Controller */
	{
		.machid			= 2413,
		.system_rev_upper	= 0,
		.compatible		= "raumfeld,raumfeld-controller-pxa303",
		.fixup_dtb		= &raumfeld_fixup_dtb_controller,
	},

	/* Connector */
	{
		.machid			= 2414,
		.system_rev_upper	= 0,
		.compatible		= "raumfeld,raumfeld-connector-pxa303",
		.fixup_dtb		= &raumfeld_fixup_dtb_common,
	},

	/* Speaker */
	{
		.machid			= 2415,
		.system_rev_upper	= 0,
		.compatible		= "raumfeld,raumfeld-speaker-s-pxa303",
		.fixup_dtb		= &raumfeld_fixup_dtb_common,
	},
	{
		.machid			= 2415,
		.system_rev_upper	= 1,
		.compatible		= "raumfeld,raumfeld-speaker-m-pxa303",
		.fixup_dtb		= &raumfeld_fixup_dtb_common,
	},
	{
		.machid			= 2415,
		.system_rev_upper	= 4,
		.compatible		= "raumfeld,raumfeld-speaker-one-pxa303",
		.fixup_dtb		= &raumfeld_fixup_dtb_common,
	},
	{ 0, 0, NULL }	/* sentinel */
};

static void wait(u32 ticks)
{
	u32 v;

	/* OSCR */
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

static void led_set(u32 index, u32 state)
{
	u32 v = 1 << (index ? 3 : 4);

	if (state)
		writel(v, 0x40e0001c);
	else
		writel(v, 0x40e00028);
}

static void led_panic(void)
{
	int i;

	led_init();

	for (i = 0;; i++) {
		led_set(0, i & 1);
		led_set(1, ~i & 1);
		wait(500000);
	}
}

struct board *match_board(u32 machid, const struct tag *tags)
{
	struct raumfeld_board *rboard = NULL;

	if (machid == 0xffffffff) {
		/*
		 * If we got a device tree passed in from kexec or such, the
		 * machid will be 0xffffffff. In this case, we can just cast
		 * the atags pointer to our dtb and then read the 'compatible'
		 * string from that dtb. We need to look up a board from our
		 * own dtbs that match the same string so the DTB is
		 * up-to-date.
		 */

		const void *dtb = tags;
		const void *val;
	        int off;

	        off = fdt_path_offset(dtb, "/");

		val = fdt_getprop(dtb, off, "hw-revision", NULL);
		if (val)
			system_rev = *(u32 *)val;
		else
			putstr("Error reading /hw-revision from DTB!\n");

		val = fdt_getprop(dtb, off, "compatible", NULL);
		if (val) {
			putstr("Got compatible string from passed DTB: ");
			putstr(val);
			putstr("\n");

			for (rboard = rboards; rboard->compatible; rboard++)
				if (!strncmp(rboard->compatible, val, strlen(rboard->compatible)))
					break;
		} else {
			putstr("Error reading /compatible from DTB!\n");
		}
	} else {
		/*
		 * Otherwise,  walk the atags to determine the system revision
		 * and determine a dtb from the available boards that matches
		 * the mach_id/system_rev combination.
		 */

		const struct tag *t;

		/* walk the atags to determine the system revision */
		for_each_tag(t, tags) {
			switch (t->hdr.tag) {
				case ATAG_REVISION:
					system_rev = t->u.rev.rev;
					break;
			}
		}

		for (rboard = rboards; rboard->machid; rboard++)
			if ((rboard->machid == machid) &&
			    (rboard->system_rev_upper == (system_rev >> 8)))
				break;
	}

	if (!rboard || !rboard->compatible) {
		putstr("ERROR MATCHING BOARD!\n");
		putstr("MACHID: 0x");
		printhex(machid);
		putstr("\n");
		putstr("SYSTEM_REV: 0x");
		printhex(system_rev);
		putstr("\n");
		led_panic(); /* doesn't return */
	}

	board.kernel = &_binary_input_zImage_start;
	board.compatible = rboard->compatible;
	board.fixup_dtb = rboard->fixup_dtb;
	board.dtb = find_dtb(&_binary_dtbs_bin_start, rboard->compatible, &board.dtb_size);

	if (board.dtb == NULL) {
		putstr("NO DTB BLOB FOUND FOR ");
		putstr(rboard->compatible);
		putstr("\n");
		led_panic(); /* doesn't return */
	}

	return &board;
}
