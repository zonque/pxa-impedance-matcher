#include "atags.h"
#include "board.h"
#include "dtbs.h"
#include "print.h"
#include "register.h"
#include "types.h"

extern u32 _binary_input_zImage_start;
extern u32 _binary_dtbs_bin_start;

struct board board;

struct bcm63xx_board {
	u32		chip_id;
	const char	*compatible;
};

static struct bcm63xx_board boards[] = {
	{
		.chip_id	= 0x63138,
		.compatible	= "brcm,bcm63138",
	},
	{
		.chip_id	= 0x63148,
		.compatible	= "brcm,bcm63148",
	},
	{ 0, NULL }	/* sentinel */
};

static void panic(void)
{
	while (1)
		;
}

/* Physical address of the INT_REVID register */
#define INT_REVID_REG	0xfffe8000
#define  CHIP_ID_SHIFT	12

static u32 get_chip_id()
{
	return readl(INT_REVID_REG) >> CHIP_ID_SHIFT;
}

struct board *match_board(u32 machid, const struct tag *tags)
{
	struct bcm63xx_board *bcm_board;
	u32 chip_id;

	chip_id = get_chip_id();

	putstr("Found a Broadcom BCM");
	printhex(chip_id);
	putstr(" chip\n");

	for (bcm_board = boards; bcm_board->chip_id; bcm_board++) {
		if (bcm_board->chip_id == chip_id)
			break;
	}

	if (bcm_board->compatible == NULL) {
		putstr("ERROR MATCHING BOARD!\n");
		panic(); /* doesn't return */
	}

	board.kernel = &_binary_input_zImage_start;
	board.compatible = bcm_board->compatible;
	board.dtb = find_dtb(&_binary_dtbs_bin_start, bcm_board->compatible,
			     &board.dtb_size);

	if (board.dtb == NULL) {
		putstr("NO DTB BLOB FOUND FOR ");
		putstr(bcm_board->compatible);
		putstr("\n");
		panic(); /* doesn't return */
	}

	return &board;
}
