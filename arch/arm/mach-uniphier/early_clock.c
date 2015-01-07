#include <linux/types.h>
#include <linux/compiler.h>
#include <asm/io.h>

/* Coretex-A9 MPCore Global Timer */
#define GLOBAL_TIMER_OFFSET	0x200

/* Global Timer Counter Registers */
struct arm_global_timer {
	u32 counter_l;
	u32 counter_h;
	u32 control;
#define PRESCALER_SHIFT		8
#define TIMER_ENABLE		(1 << 0)
	u32 interrupt_status;
	u32 comparator_value_l;
	u32 comparator_value_h;
	u32 auto_increment;
};

#define TIMER_RATE		1000000			/* 1 tick = 1 usec */
#define PERIPHCLK		(50 * 1000 * 1000)	/* 50 MHz */
#define PRESCALER		((PERIPHCLK) / (TIMER_RATE) - 1)

unsigned long early_get_time_us(void)
{
	struct arm_global_timer __iomem *timer;
	void *periph_base;

	asm("mrc p15, 4, %0, c15, c0, 0" : "=r" (periph_base) : : "memory");

	timer = periph_base + GLOBAL_TIMER_OFFSET;

	/* enable timer */
	writel((PRESCALER << PRESCALER_SHIFT) | TIMER_ENABLE, &timer->control);

	return readl(&timer->counter_l);
}

void early_udelay(unsigned long usecs)
{
	unsigned long timeout;

	timeout = early_get_time_us() + usecs;

	while (early_get_time_us() < timeout)
		;
}
