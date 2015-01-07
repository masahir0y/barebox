/*
 * Copyright (C) 2011-2014 Panasonic Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <debug_ll.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/sizes.h>
#include <asm/io.h>
#include <mach/init.h>
#include <mach/umc-regs.h>
#include <mach/ddrphy-regs.h>

#include "ddrphy_init.c"

static void umc_start_ssif(void __iomem *ssif_base)
{
	writel(0x00000001, ssif_base + 0x0000b004);
	writel(0xffffffff, ssif_base + 0x0000c004);
	writel(0x07ffffff, ssif_base + 0x0000c008);
	writel(0x00000001, ssif_base + 0x0000b000);
	writel(0x00000001, ssif_base + 0x0000c000);

	writel(0x03010100, ssif_base + UMC_HDMCHSEL);
	writel(0x03010101, ssif_base + UMC_MDMCHSEL);
	writel(0x03010100, ssif_base + UMC_DVCCHSEL);
	writel(0x03010100, ssif_base + UMC_DMDCHSEL);

	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_FETCH);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_COMQUE0);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_COMWC0);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_COMRC0);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_COMQUE1);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_COMWC1);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_COMRC1);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_WC);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_RC);
	writel(0x00000000, ssif_base + UMC_CLKEN_SSIF_DST);
	writel(0x00000000, ssif_base + 0x0000c044);		/* DCGIV_SSIF_REG */

	writel(0x00000001, ssif_base + UMC_CPURST);
	writel(0x00000001, ssif_base + UMC_IDSRST);
	writel(0x00000001, ssif_base + UMC_IXMRST);
	writel(0x00000001, ssif_base + UMC_HDMRST);
	writel(0x00000001, ssif_base + UMC_MDMRST);
	writel(0x00000001, ssif_base + UMC_HDDRST);
	writel(0x00000001, ssif_base + UMC_MDDRST);
	writel(0x00000001, ssif_base + UMC_SIORST);
	writel(0x00000001, ssif_base + UMC_GIORST);
	writel(0x00000001, ssif_base + UMC_HD2RST);
	writel(0x00000001, ssif_base + UMC_VIORST);
	writel(0x00000001, ssif_base + UMC_DVCRST);
	writel(0x00000001, ssif_base + UMC_RGLRST);
	writel(0x00000001, ssif_base + UMC_VPERST);
	writel(0x00000001, ssif_base + UMC_AIORST);
	writel(0x00000001, ssif_base + UMC_DMDRST);
}

static void umc_dramcont_init(void __iomem *dramcont, void __iomem *ca_base,
			      int size, int freq)
{
	writel(0x66bb0f17, dramcont + UMC_CMDCTLA);
	writel(0x18c6aa44, dramcont + UMC_CMDCTLB);
	writel(0x5101387f, dramcont + UMC_INITCTLA);
	writel(0x43030d3f, dramcont + UMC_INITCTLB);
	writel(0x00ff00ff, dramcont + UMC_INITCTLC);
	writel(0x00000d71, dramcont + UMC_DRMMR0);
	writel(0x00000006, dramcont + UMC_DRMMR1);
	writel(0x00000298, dramcont + UMC_DRMMR2);
	writel(0x00000000, dramcont + UMC_DRMMR3);
	writel(0x003f0617, dramcont + UMC_SPCCTLA);
	writel(0x00ff0008, dramcont + UMC_SPCCTLB);
	writel(0x000c00ae, dramcont + UMC_RDATACTL_D0);
	writel(0x000c00ae, dramcont + UMC_RDATACTL_D1);
	writel(0x04060802, dramcont + UMC_WDATACTL_D0);
	writel(0x04060802, dramcont + UMC_WDATACTL_D1);
	writel(0x04a02000, dramcont + UMC_DATASET);
	writel(0x00000000, ca_base + 0x2300);
	writel(0x00400020, dramcont + UMC_DCCGCTL);
	writel(0x0000000f, dramcont + 0x7000);
	writel(0x0000000f, dramcont + 0x8000);
	writel(0x000000c3, dramcont + 0x8004);
	writel(0x00000071, dramcont + 0x8008);
	writel(0x00000004, dramcont + UMC_FLOWCTLG);
	writel(0x00000000, dramcont + 0x0060);
	writel(0x80000201, ca_base + 0xc20);
	writel(0x0801e01e, dramcont + UMC_FLOWCTLA);
	writel(0x00200000, dramcont + UMC_FLOWCTLB);
	writel(0x00004444, dramcont + UMC_FLOWCTLC);
	writel(0x200a0a00, dramcont + UMC_SPCSETB);
	writel(0x00010000, dramcont + UMC_SPCSETD);
	writel(0x80000020, dramcont + UMC_DFICUPDCTLA);
}

static int umc_init(const struct uniphier_board_param *bd)
{
	void __iomem *ssif_base = (void __iomem *)UMC_SSIF_BASE;
	void __iomem *ca_base0 = (void __iomem *)UMC_CA_BASE(0);
	void __iomem *ca_base1 = (void __iomem *)UMC_CA_BASE(1);
	void __iomem *dramcont0 = (void __iomem *)UMC_DRAMCONT_BASE(0);
	void __iomem *dramcont1 = (void __iomem *)UMC_DRAMCONT_BASE(1);
	void __iomem *phy0_0 = (void __iomem *)DDRPHY_BASE(0, 0);
	void __iomem *phy0_1 = (void __iomem *)DDRPHY_BASE(0, 1);
	void __iomem *phy1_0 = (void __iomem *)DDRPHY_BASE(1, 0);
	void __iomem *phy1_1 = (void __iomem *)DDRPHY_BASE(1, 1);

	switch (bd->dram_freq) {
	case 1600:
		/* supported */
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM frequency\n");
		return -1;
	}

	switch (bd->dram_ch0_size) {
	case SZ_256M:
		if (bd->dram_ch0_width != 16) {
			puts_ll(__func__);
			puts_ll(" : error: unsupported DRAM Ch0 width\n");
			return -1;
		}
		break;
	case SZ_512M:
		if (bd->dram_ch0_width != 32) {
			puts_ll(__func__);
			puts_ll(" : error: unsupported DRAM Ch0 width\n");
			return -1;
		}
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM Ch0 size\n");
		return -1;
	}

	switch (bd->dram_ch1_size) {
	case SZ_256M:
		if (bd->dram_ch1_width != 16) {
			puts_ll(__func__);
			puts_ll(" : error: unsupported DRAM Ch1 width\n");
			return -1;
		}
		break;
	case SZ_512M:
		if (bd->dram_ch1_width != 32) {
			puts_ll(__func__);
			puts_ll(" : error: unsupported DRAM Ch1 width\n");
			return -1;
		}
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM Ch1 size\n");
		return -1;
	}

	umc_dram_init_start(dramcont0);
	umc_dram_init_start(dramcont1);
	umc_dram_init_poll(dramcont0);
	umc_dram_init_poll(dramcont1);

	writel(0x00000101, dramcont0 + UMC_DIOCTLA);

	ddrphy_init(phy0_0, bd->dram_freq, bd->dram_ch0_size);

	ddrphy_prepare_training(phy0_0, 0);
	ddrphy_training(phy0_0);

	writel(0x00000103, dramcont0 + UMC_DIOCTLA);

	ddrphy_init(phy0_1, bd->dram_freq, bd->dram_ch0_size);

	ddrphy_prepare_training(phy0_1, 1);
	ddrphy_training(phy0_1);

	writel(0x00000101, dramcont1 + UMC_DIOCTLA);

	ddrphy_init(phy1_0, bd->dram_freq, bd->dram_ch1_size);

	ddrphy_prepare_training(phy1_0, 0);
	ddrphy_training(phy1_0);

	writel(0x00000103, dramcont1 + UMC_DIOCTLA);

	ddrphy_init(phy1_1, bd->dram_freq, bd->dram_ch1_size);

	ddrphy_prepare_training(phy1_1, 1);
	ddrphy_training(phy1_1);

	umc_dramcont_init(dramcont0, ca_base0, bd->dram_ch0_size, bd->dram_freq);
	umc_dramcont_init(dramcont1, ca_base1, bd->dram_ch1_size, bd->dram_freq);

	umc_start_ssif(ssif_base);

	return 0;
}
ph1_pro4_init_func(umc_init);
