#include "atags.h"
#include "board.h"
#include "print.h"
#include "types.h"
#include "string.h"

struct board genboard;

struct board *match_board(u32 machid, const struct tag *tags)
{
	const struct tag *t;

	/* walk the atags to get the command line */
	for_each_tag(t, tags)
		switch (t->hdr.tag) {
			case ATAG_CMDLINE:
				getaddrs(&genboard.kernel, &genboard.dtb,
					t->u.cmdline.cmdline);
				break;
		}

	if (genboard.kernel) {
		putstr("Kernel: 0x");
		printhex((u32)genboard.kernel);
	} else
		putstr("Kernel: Appended");
	putstr("\n");

	if (genboard.dtb) {
		putstr("DTB:    0x");
		printhex((u32)genboard.dtb);
	} else
		putstr("** DTB Not Found! **");
	putstr("\n");

	genboard.compatible = NULL;

	return &genboard;
}
