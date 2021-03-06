#ifndef _BOARD_H
#define _BOARD_H

#include "types.h"
#include "atags.h"

struct board;

struct board {
	void		*dtb;
	size_t		dtb_size;
	void		*kernel;
	const char	*compatible;
	void		(*fixup_dtb)(const struct board *);
};

struct board *match_board(u32 machid, const struct tag *);

#endif
