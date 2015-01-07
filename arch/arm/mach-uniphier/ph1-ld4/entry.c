/*
 * Copyright (C) 2015 Panasonic Corporation
 *   Author: Masahiro Yamada <yamada.m@jp.panasonic.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <debug_ll.h>
#include <linux/sizes.h>
#include <asm/barebox-arm.h>
#include <mach/init.h>
#include "debug_ll_init.c"

static inline void lowlevel_soc_init(const struct uniphier_board_param *bd)
{
	debug_ll_init();

	early_setup_c();

	ph1_ld4_init_call(bcu_init, bd);
	ph1_ld4_init_call(sbc_init, bd);
	ph1_ld4_init_call(sg_init, bd);
	ph1_ld4_init_call(pll_init, bd);
	ph1_ld4_init_call(clkrst_init, bd);
	ph1_ld4_init_call(umc_init, bd);
	ph1_ld4_init_call(enable_dpll_ssc, bd);

	puts_ll("barebox start ...\n");
}

static const struct uniphier_board_param ph1_ld4_ref_2g2g_param = {
	.dram_ch0_base  = 0x80000000,
	.dram_ch0_size  = 0x10000000,
	.dram_ch0_width = 16,
	.dram_ch1_base  = 0x90000000,
	.dram_ch1_size  = 0x10000000,
	.dram_ch1_width = 16,
	.dram_freq      = 1600,
};

extern char __dtb_uniphier_ph1_ld4_ref_start[];

ENTRY_FUNCTION(start_uniphier_ph1_ld4_ref_2g2g, r0, r1, r2)
{
	lowlevel_soc_init(&ph1_ld4_ref_2g2g_param);

	uniphier_entry(__dtb_uniphier_ph1_ld4_ref_start);
}

static const struct uniphier_board_param ph1_ld4_ref_1g1g_param = {
	.dram_ch0_base  = 0x80000000,
	.dram_ch0_size  = 0x08000000,
	.dram_ch0_width = 16,
	.dram_ch1_base  = 0x90000000,
	.dram_ch1_size  = 0x08000000,
	.dram_ch1_width = 16,
	.dram_freq      = 1600,
};

ENTRY_FUNCTION(start_uniphier_ph1_ld4_ref_1g1g, r0, r1, r2)
{
	lowlevel_soc_init(&ph1_ld4_ref_1g1g_param);

	uniphier_entry(__dtb_uniphier_ph1_ld4_ref_start);
}
