#include "types.h"

void writeb(u8 val, u32 addr)
{
	*(volatile u32 *)addr = val;
}

u8 readb(u32 addr)
{
	return *(volatile u32 *)addr;
}

void writel(u32 val, u32 addr)
{
	*(volatile u32 *) addr = val;
}

u32 readl(u32 addr)
{
	return *(volatile u32 *) addr;
}
