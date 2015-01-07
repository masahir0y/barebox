#ifndef __MACH_EARLY_CLOCK_H__
#define __MACH_EARLY_CLOCK_H__

unsigned long early_get_time_us(void);
void early_udelay(unsigned long usecs);

#endif /* __MACH_EARLY_CLOCK_H__ */
