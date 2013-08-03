#ifndef _ATAG_H
#define _ATAG_H

#include "types.h"

#define ATAG_NONE       0x00000000

struct tag_header {
	u32 size;
	u32 tag;
};

/* board revision */
#define ATAG_REVISION   0x54410007

struct tag_revision {
	u32 rev;
};

/* command line */
#define ATAG_CMDLINE	0x54410009

struct tag_cmdline {
	char cmdline[1];	/* this is the minimum size */
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_revision	rev;
		struct tag_cmdline	cmdline;
	} u;
};

#define tag_next(t)     ((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define for_each_tag(t,base)            \
	for (t = base; t->hdr.size; t = tag_next(t))

#endif /* _ATAG_H */
