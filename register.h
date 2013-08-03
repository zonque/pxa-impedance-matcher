#ifndef _REGISTER_H
#define _REGISTER_H

#include "types.h"

void writel(__u32, __u32);
__u32 readl(__u32);

#define BIT(nr)         (1UL << (nr))

#endif
