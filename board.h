#ifndef _BOARD_H
#define _BOARD_H

#include "types.h"
#include "atags.h"

struct board {
	void		*dtb;
	void		*kernel;
	const char	*compatible;
};

struct board *match_board(u32 machid, const struct tag *);

#endif
