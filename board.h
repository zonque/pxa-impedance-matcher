#ifndef _BOARD_H
#define _BOARD_H

#include "types.h"
#include "atags.h"

struct board {
	__u32		machid;
	__u32		system_rev;
	void		*dtb;
	const char	*compatible;
};

struct board *match_board(__u32 machid, const struct tag *);

#endif
