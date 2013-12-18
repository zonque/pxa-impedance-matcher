#include "atags.h"
#include "board.h"
#include "dtbs.h"
#include "print.h"
#include "register.h"
#include "types.h"

extern u32 _binary_input_zImage_start;
extern u32 _binary_dtbs_bin_start;

struct board board;

#define EXYNOS_PRO_ID		0x10000000
#define EXYNOS_PRO_ID_SHIFT	12

struct exynos_board {
	u32		proid;
	u32		system_rev;
	const char	*compatible;
};

static struct exynos_board exboards[] = {
	/* Controller */
	{
		.proid		= 0xe4412,
		.system_rev	= 0,
		.compatible	= "samsung,trats2",
	},
	{
		.proid		= 0x43210,
		.system_rev	= 0,
		.compatible	= "samsung,trats",
	},
	{ 0, 0, NULL }	/* sentinel */
};

static void panic(void)
{
	while (1)
		;
}

static u32 get_proid()
{
	return readl(EXYNOS_PRO_ID) >> EXYNOS_PRO_ID_SHIFT;
}

struct board *match_board(u32 machid, const struct tag *tags)
{
	struct exynos_board *exboard;
	u32 proid;

	proid = get_proid();

	for (exboard = exboards; exboard->proid; exboard++) {
		if (exboard->proid == proid)
			break;
	}

	if (exboard->compatible == NULL) {
		putstr("ERROR MATCHING BOARD!\n");
		panic(); /* doesn't return */
	}

	board.kernel = &_binary_input_zImage_start;
	board.compatible = exboard->compatible;
	board.dtb = find_dtb(&_binary_dtbs_bin_start, exboard->compatible);

	if (board.dtb == NULL) {
		putstr("NO DTB BLOB FOUND FOR ");
		putstr(exboard->compatible);
		putstr("\n");
		panic(); /* doesn't return */
	}

	return &board;
}
