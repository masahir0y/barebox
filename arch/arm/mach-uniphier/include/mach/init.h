#ifndef _MACH_INIT_H
#define _MACH_INIT_H

#include <linux/compiler.h>
#include <linux/stringify.h>

struct uniphier_board_param {
	unsigned long dram_ch0_base;
	unsigned long dram_ch0_size;
	unsigned long dram_ch0_width;
	unsigned long dram_ch1_base;
	unsigned long dram_ch1_size;
	unsigned long dram_ch1_width;
	unsigned long dram_ch2_base;
	unsigned long dram_ch2_size;
	unsigned long dram_ch2_width;
	unsigned int  dram_freq;
};

void uniphier_lowlevel_initcall(
			int (*init_func)(const struct uniphier_board_param *),
			const char *func_name,
			const struct uniphier_board_param *bd);

void early_setup_c(void);

void __noreturn uniphier_entry(void *boarddata);
void __noreturn jump_to_arm_entry(unsigned long membase,
				  unsigned long memsize, void *boarddata);

#define uniphier_init_func(prefix, name)				\
static inline int name(const struct uniphier_board_param *bd);		\
__weak int __##prefix##name(const struct uniphier_board_param *bd)	\
{									\
	return name(bd);						\
}

#define uniphier_init_call(prefix, name, bd)				\
{									\
	int __##prefix##name(const struct uniphier_board_param *);	\
	uniphier_lowlevel_initcall(__##prefix##name,			\
				   __stringify(name), bd);		\
}

#define ph1_ld4_init_func(name)		uniphier_init_func(ph1_ld4_, name)
#define ph1_ld4_init_call(name, bd)	uniphier_init_call(ph1_ld4_, name, bd)
#define ph1_pro4_init_func(name)	uniphier_init_func(ph1_pro4_, name)
#define ph1_pro4_init_call(name, bd)	uniphier_init_call(ph1_pro4_, name, bd)
#define ph1_sld8_init_func(name)	uniphier_init_func(ph1_sld8_, name)
#define ph1_sld8_init_call(name, bd)	uniphier_init_call(ph1_sld8_, name, bd)

#endif /* _MACH_INIT_H */
