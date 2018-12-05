#ifndef _TYPES_H
#define _TYPES_H

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned int	uint32_t;
typedef unsigned long	long uint64_t;

#define cpu_to_be32(x) ((u32)(x))
#define cpu_to_be64(x) ((u64)(x))

#define be32_to_cpu(x) (swab32(x))
#define be64_to_cpu(x) (swab64(x))

#define swab32(x) ((u32)(                         \
	(((u32)(x) & (u32)0x000000ffUL) << 24) |            \
	(((u32)(x) & (u32)0x0000ff00UL) <<  8) |            \
	(((u32)(x) & (u32)0x00ff0000UL) >>  8) |            \
	(((u32)(x) & (u32)0xff000000UL) >> 24)))

#define swab64(x) ((u64)(                         \
	(((u64)(x) & (u64)0x00000000000000ffULL) << 56) |   \
	(((u64)(x) & (u64)0x000000000000ff00ULL) << 40) |   \
	(((u64)(x) & (u64)0x0000000000ff0000ULL) << 24) |   \
	(((u64)(x) & (u64)0x00000000ff000000ULL) <<  8) |   \
	(((u64)(x) & (u64)0x000000ff00000000ULL) >>  8) |   \
	(((u64)(x) & (u64)0x0000ff0000000000ULL) >> 24) |   \
	(((u64)(x) & (u64)0x00ff000000000000ULL) >> 40) |   \
	(((u64)(x) & (u64)0xff00000000000000ULL) >> 56)))

#endif /* _TYPES_H */
