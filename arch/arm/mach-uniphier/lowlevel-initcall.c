#include <debug_ll.h>
#include <printk.h>
#include <mach/init.h>
#include <mach/early_clock.h>

void uniphier_lowlevel_initcall(
			int (*init_func)(const struct uniphier_board_param *),
			const char *func_name,
			const struct uniphier_board_param *bd)
{
	int ret;
	unsigned long time;

	time = early_get_time_us();
	pr_info("[%6lu.%06lu] %s start\n", time / 1000000,
		 time % 1000000, func_name);

	ret = (*init_func)(bd);
	if (ret < 0) {
		puts_ll("error: ");
		puts_ll(func_name);
		puts_ll(" failed\nstop\n");
		while (1)
			;
	}

	time = early_get_time_us();
	pr_info("[%6lu.%06lu] %s end\n", time / 1000000,
		 time % 1000000, func_name);
}
