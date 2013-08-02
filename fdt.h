#ifndef _FDT_H
#define _FDT_H

/* Copied from barebox, thanks! */

#include "types.h"

#ifndef __ASSEMBLY__

struct fdt_header {
	__u32 magic;			 /* magic word FDT_MAGIC */
	__u32 totalsize;		 /* total size of DT block */
	__u32 off_dt_struct;		 /* offset to structure */
	__u32 off_dt_strings;	 /* offset to strings */
	__u32 off_mem_rsvmap;	 /* offset to memory reserve map */
	__u32 version;		 /* format version */
	__u32 last_comp_version;	 /* last compatible version */

	/* version 2 fields below */
	__u32 boot_cpuid_phys;	 /* Which physical CPU id we're
					    booting on */
	/* version 3 fields below */
	__u32 size_dt_strings;	 /* size of the strings block */

	/* version 17 fields below */
	__u32 size_dt_struct;	 /* size of the structure block */
} __attribute__((packed));

struct fdt_reserve_entry {
	__u64 address;
	__u64 size;
};

struct fdt_node_header {
	__u32 tag;
	char name[0];
};

struct fdt_property {
	__u32 tag;
	__u32 len;
	__u32 nameoff;
	char data[0];
};

#endif /* !__ASSEMBLY */

#define FDT_MAGIC	0xd00dfeed	/* 4: version, 4: total size */
#define FDT_TAGSIZE	sizeof(__u32)

#define FDT_BEGIN_NODE	0x1		/* Start node: full name */
#define FDT_END_NODE	0x2		/* End node */
#define FDT_PROP	0x3		/* Property: name off,
					   size, content */
#define FDT_NOP		0x4		/* nop */
#define FDT_END		0x9

#define FDT_V1_SIZE	(7*sizeof(__u32))
#define FDT_V2_SIZE	(FDT_V1_SIZE + sizeof(__u32))
#define FDT_V3_SIZE	(FDT_V2_SIZE + sizeof(__u32))
#define FDT_V16_SIZE	FDT_V3_SIZE
#define FDT_V17_SIZE	(FDT_V16_SIZE + sizeof(__u32))

#endif /* _FDT_H */
