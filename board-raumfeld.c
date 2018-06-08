#include "atags.h"
#include "board.h"
#include "dtbs.h"
#include "print.h"
#include "register.h"
#include "types.h"

extern u32 _binary_input_zImage_start;
extern u32 _binary_dtbs_bin_start;

struct board board;

struct raumfeld_board {
	u32		machid;
	u16		system_rev_upper;
	const char	*compatible;
};

static struct raumfeld_board rboards[] = {
	/* Controller */
	{
		.machid			= 2413,
		.system_rev_upper	= 0,
		.compatible		= "raumfeld,raumfeld-controller-pxa303",
	},

	/* Connector */
	{
		.machid			= 2414,
		.system_rev_upper	= 0,
		.compatible		= "raumfeld,raumfeld-connector-pxa303",
	},

	/* Speaker */
	{
		.machid			= 2415,
		.system_rev_upper	= 0,
		.compatible		= "raumfeld,raumfeld-speaker-s-pxa303",
	},
	{
		.machid			= 2415,
		.system_rev_upper	= 1,
		.compatible		= "raumfeld,raumfeld-speaker-m-pxa303",
	},
	{
		.machid			= 2415,
		.system_rev_upper	= 2,
		.compatible		= "raumfeld,raumfeld-speaker-one-pxa303",
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
	const struct tag *t;
	struct raumfeld_board *rboard;
	u32 system_rev = 0;

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

	if (rboard->compatible == NULL) {
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
	board.dtb = find_dtb(&_binary_dtbs_bin_start, rboard->compatible, &board.dtb_size);

	if (board.dtb == NULL) {
		putstr("NO DTB BLOB FOUND FOR ");
		putstr(rboard->compatible);
		putstr("\n");
		led_panic(); /* doesn't return */
	}

	return &board;
}
