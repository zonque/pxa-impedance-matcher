#include "atags.h"
#include "board.h"
#include "print.h"
#include "register.h"
#include "types.h"

extern __u32 _binary_input_zImage_start;
extern __u32 _binary_input_raumfeld_controller_0_dtb_start;
extern __u32 _binary_input_raumfeld_controller_1_dtb_start;
extern __u32 _binary_input_raumfeld_controller_2_dtb_start;
extern __u32 _binary_input_raumfeld_connector_0_dtb_start;
extern __u32 _binary_input_raumfeld_connector_1_dtb_start;
extern __u32 _binary_input_raumfeld_connector_2_dtb_start;
extern __u32 _binary_input_raumfeld_speaker_0_dtb_start;
extern __u32 _binary_input_raumfeld_speaker_1_dtb_start;
extern __u32 _binary_input_raumfeld_speaker_2_dtb_start;

static struct board boards[] = {
	/* Controller */
	{
		.machid		= 2413,
		.system_rev	= 0,
		.dtb		= &_binary_input_raumfeld_controller_0_dtb_start,
		.compatible	= "Raumfeld Controller, revision 0",
	},
	{
		.machid		= 2413,
		.system_rev	= 1,
		.dtb		= &_binary_input_raumfeld_controller_1_dtb_start,
		.compatible	= "Raumfeld Controller, revision 1",
	},
	{
		.machid		= 2413,
		.system_rev	= 2,
		.dtb		= &_binary_input_raumfeld_controller_2_dtb_start,
		.compatible	= "Raumfeld Controller, revision 2",
	},

	/* Controller */
	{
		.machid		= 2414,
		.system_rev	= 0,
		.dtb		= &_binary_input_raumfeld_connector_0_dtb_start,
		.compatible	= "Raumfeld Connector, revision 0",
	},
	{
		.machid		= 2414,
		.system_rev	= 1,
		.dtb		= &_binary_input_raumfeld_connector_1_dtb_start,
		.compatible	= "Raumfeld Connector, revision 1",
	},
	{
		.machid		= 2414,
		.system_rev	= 2,
		.dtb		= &_binary_input_raumfeld_connector_2_dtb_start,
		.compatible	= "Raumfeld Connector, revision 2",
	},

	/* Speaker */
	{
		.machid		= 2415,
		.system_rev	= 0,
		.dtb		= &_binary_input_raumfeld_speaker_0_dtb_start,
		.compatible	= "Raumfeld Speaker, revision 0",
	},
	{
		.machid		= 2415,
		.system_rev	= 1,
		.dtb		= &_binary_input_raumfeld_speaker_1_dtb_start,
		.compatible	= "Raumfeld Speaker, revision 1",
	},
	{
		.machid		= 2415,
		.system_rev	= 2,
		.dtb		= &_binary_input_raumfeld_speaker_2_dtb_start,
		.compatible	= "Raumfeld Speaker, revision 2",
	},
	{ 0, 0, NULL, NULL }	/* sentinel */
};

static void wait(__u32 ticks)
{
	__u32 v;

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

static void led_set(__u32 index, __u32 state)
{
	__u32 v = 1 << (index ? 3 : 4);

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
struct board *match_board(__u32 machid, const struct tag *tags)
{
	const struct tag *t;
	struct board *board;
	__u32 system_rev = 0;

	/* walk the atags to determine the system revision */
	for_each_tag(t, tags)
		switch (t->hdr.tag) {
			case ATAG_REVISION:
				system_rev = t->u.rev.rev;
				break;
		}


	for (board = boards; board->machid; board++)
		if (board->machid == machid && board->system_rev == system_rev)
			break;

	if (board->compatible == NULL) {
		putstr("ERROR MATCHING BOARD!\n");
		putstr("MACHID: 0x");
		printhex(machid);
		putstr("\n");
		putstr("SYSTEM_REV: 0x");
		printhex(system_rev);
		putstr("\n");
		led_panic(); /* doesn't return */
	}

	return board;
}
