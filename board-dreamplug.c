#include "board.h"
#include "dtbs.h"
#include "print.h"
#include "register.h"
#include "types.h"

/*
 * this code is just a quick PoC for appending multiple dtbs.  I don't have
 * any boards handy with stock, non-DT bootloaders, so I have no machids to
 * trigger off of.
 */

extern __u32 _binary_input_zImage_start;
extern __u32 _binary_dtbs_bin_start;

struct board board;
const char *compat = "globalscale,dreamplug";

struct board *match_board(__u32 machid, const struct tag *tags)
{

	/* testing zImage linked in and dtbs appended */
	board.kernel = &_binary_input_zImage_start;
	board.compatible = compat;
	board.dtb = find_dtb(&_binary_dtbs_bin_start, compat);

	return &board;
}
