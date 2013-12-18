#ifndef _REGISTER_H
#define _REGISTER_H

#include "types.h"

void writeb(u8, u32);
u8 readb(u32);

void writel(u32, u32);
u32 readl(u32);

#define BIT(nr)         (1UL << (nr))

#endif
