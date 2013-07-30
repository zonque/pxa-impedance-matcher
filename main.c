#include "types.h"
#include "atags.h"
#include "print.h"
#include "board.h"

extern __u32 _binary_input_zImage_start;

void main(__u32 dummy, __u32 machid, const struct tag *tags)
{
	struct board *board;
	void (*start_kernel)(__u32 dummy, __u32 machid, void *dtb) =
		(void *) &_binary_input_zImage_start;

	putstr("++ Impedance Matcher (3rd stage loader) ++\n");

	/*
	 * error resp. is board-specific, so won't return on error
	 * eg - raumfeld boards blink a led indefinitely
	 */
	board = match_board(machid, tags);

	putstr("Detected board: ");
	putstr(board->name);
	putstr("\n");

	putstr("Booting into Linux kernel ...\n");
	start_kernel(0, 0xffffffff, board->dtb);
}
