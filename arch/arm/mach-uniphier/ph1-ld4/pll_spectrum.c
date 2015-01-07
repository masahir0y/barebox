/*
 * Copyright (C) 2011-2014 Panasonic Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/io.h>
#include <mach/init.h>
#include <mach/sc-regs.h>

static int enable_dpll_ssc(const struct uniphier_board_param *bd)
{
	u32 tmp;

	tmp = readl(SC_DPLLCTRL);
	tmp |= SC_DPLLCTRL_SSC_EN;
	writel(tmp, SC_DPLLCTRL);

	return 0;
}
ph1_ld4_init_func(enable_dpll_ssc);
