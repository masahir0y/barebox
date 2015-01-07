#ifndef __SET_PINSEL_H__
#define __SET_PINSEL_H__

#include <mach/sg-regs.h>

static inline void set_pinsel(int n, int value)
{
	sg_set_pinsel(n, value, 8, 4);
}

#endif /* __SET_PINSEL_H__ */
