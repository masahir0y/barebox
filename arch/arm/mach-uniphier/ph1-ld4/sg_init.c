
#include <debug_ll.h>
#include <linux/sizes.h>
#include <asm/io.h>
#include <mach/init.h>
#include <mach/sg-regs.h>

static inline int sg_init(const struct uniphier_board_param *bd)
{
	u32 tmp = 0;
	unsigned long size_per_word;

	switch (bd->dram_ch0_width) {
	case 16:
		tmp |= SG_MEMCONF_CH0_NUM_1;
		size_per_word = bd->dram_ch0_size;
		break;
	case 32:
		tmp |= SG_MEMCONF_CH0_NUM_2;
		size_per_word = bd->dram_ch0_size >> 1;
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM Ch0 width\n");
		return -1;
	}

	/* Set DDR size */
	switch (size_per_word) {
	case SZ_64M:
		tmp |= SG_MEMCONF_CH0_SZ_64M;
		break;
	case SZ_128M:
		tmp |= SG_MEMCONF_CH0_SZ_128M;
		break;
	case SZ_256M:
		tmp |= SG_MEMCONF_CH0_SZ_256M;
		break;
	case SZ_512M:
		tmp |= SG_MEMCONF_CH0_SZ_512M;
		break;
	case SZ_1G:
		tmp |= SG_MEMCONF_CH0_SZ_1G;
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM Ch0 size\n");
		return -1;
	}

	switch (bd->dram_ch1_width) {
	case 16:
		tmp |= SG_MEMCONF_CH1_NUM_1;
		size_per_word = bd->dram_ch1_size;
		break;
	case 32:
		tmp |= SG_MEMCONF_CH1_NUM_2;
		size_per_word = bd->dram_ch1_size >> 1;
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM Ch1 width\n");
		return -1;
	}

	switch (size_per_word) {
	case SZ_64M:
		tmp |= SG_MEMCONF_CH1_SZ_64M;
		break;
	case SZ_128M:
		tmp |= SG_MEMCONF_CH1_SZ_128M;
		break;
	case SZ_256M:
		tmp |= SG_MEMCONF_CH1_SZ_256M;
		break;
	case SZ_512M:
		tmp |= SG_MEMCONF_CH1_SZ_512M;
		break;
	case SZ_1G:
		tmp |= SG_MEMCONF_CH1_SZ_1G;
		break;
	default:
		puts_ll(__func__);
		puts_ll(" : error: unsupported DRAM Ch1 size\n");
		return -1;
	}

	if (bd->dram_ch0_base + bd->dram_ch0_size < bd->dram_ch1_base)
		tmp |= SG_MEMCONF_SPARSEMEM;

	writel(tmp, SG_MEMCONF);

	/* Input ports must be enabled before deasserting reset of cores */
	tmp = readl(SG_IECTRL);
	tmp |= 0x1;
	writel(tmp, SG_IECTRL);

	return 0;
}
ph1_ld4_init_func(sg_init);
