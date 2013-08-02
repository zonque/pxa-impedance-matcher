#include "types.h"

inline void writel(__u32 val, __u32 addr)
{
	*(volatile __u32 *) addr = val;
}

inline __u32 readl(__u32 addr)
{
	return *(volatile __u32 *) addr;
}
