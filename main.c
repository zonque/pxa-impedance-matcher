#include "types.h"
#include "atags.h"
#include "print.h"
#include "board.h"
#include "version.h"


void main(u32 dummy, u32 machid, struct tag *tags)
	__attribute__((naked))
	__attribute__((section(".text_main")));

void main(u32 dummy, u32 machid, struct tag *tags)
{
	struct board *board;
	void (*start_kernel)(u32 dummy, u32 machid, void *dtb);
	void *dtb = tags;

	asm volatile("mov sp, lr");

#ifdef APPEND_KERNEL
	extern u32 _binary_input_zImage_start;
	start_kernel = (void *) &_binary_input_zImage_start;
#endif

	putstr("++ Impedance Matcher ");
	putstr(VERSION);
	putstr(" (3rd stage loader) ++\n\n");

	/*
	 * error resp. is board-specific, so won't return on error
	 * eg - raumfeld boards blink a led indefinitely
	 */
	board = match_board(machid, tags);

	putstr("Detected board: ");
	if (board->compatible)
		putstr(board->compatible);
	else
		putstr("Not given.");
	putstr("\n");

#ifdef LIBFDT
	atags_to_fdt((void *)tags, board->dtb, 0x100000);
#endif

	if (board->kernel)
		start_kernel = board->kernel;

	if (board->dtb) {
#ifdef RELOCATE_DTB
		dtb = RELOCATE_DTB;
		memcpy(dtb, board->dtb, board->dtb_size);
		board->dtb = dtb;
#else
		dtb = board->dtb;
#endif
	}

	if (board->fixup_dtb)
		board->fixup_dtb(board);

	putstr("Booting into Linux kernel ...\n");
	start_kernel(0, 0xffffffff, dtb);
}
