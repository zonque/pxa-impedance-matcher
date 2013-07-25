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

/* board revision */
#define ATAG_REVISION   0x54410007

struct tag_revision {
	__u32 rev;
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_revision	rev;
	} u;
};

#define tag_next(t)     ((struct tag *)((__u32 *)(t) + (t)->hdr.size))
#define for_each_tag(t,base)            \
	for (t = base; t->hdr.size; t = tag_next(t))

#endif /* _ATAG_H */
