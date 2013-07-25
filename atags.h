#ifndef _ATAG_H
#define _ATAG_H

#define ATAG_NONE       0x00000000

struct tag_header {
	__u32 size;
	__u32 tag;
};

#define ATAG_CORE       0x54410001

struct tag_core {
	__u32 flags;	    /* bit 0 = read-only */
	__u32 pagesize;
	__u32 rootdev;
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
	} u;
};

#endif /* _ATAG_H */
