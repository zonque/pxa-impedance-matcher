#ifndef _BOARD_H
#define _BOARD_H

struct board {
	__u32		machid;
	__u32		system_rev;
	void		*dtb;
	const char	*name;
};

struct board *match_board(__u32 machid, const struct tag *);

#endif
