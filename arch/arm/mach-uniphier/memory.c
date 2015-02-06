#include <init.h>
#include <printk.h>
#include <linux/err.h>
#include <linux/sizes.h>
#include <asm/io.h>
#include <asm/memory.h>
#include <mach/soc_info.h>
#include <mach/sg-regs.h>

static int get_dram_ch1_base(resource_size_t *base)
{
	switch (uniphier_get_soc_type()) {
	case SOC_UNIPHIER_PH1_SLD3:
	case SOC_UNIPHIER_PH1_LD4:
	case SOC_UNIPHIER_PH1_SLD8:
	case SOC_UNIPHIER_PH1_PRO5:
	case SOC_UNIPHIER_PROXSTREAM2:
	case SOC_UNIPHIER_PH1_LD6B:
		*base = 0xc0000000;
		break;
	case SOC_UNIPHIER_PH1_PRO4:
		*base = 0xa0000000;
		break;
	default:
		pr_err("Unknown SoC\n");
		return -EINVAL;
	}

	return 0;
}

static int uniphier_mem_init(void)
{
	const resource_size_t dram_ch0_base = 0x80000000;
	resource_size_t  dram_ch0_size, dram_ch1_base, dram_ch1_size;
	u32 memconf = readl(SG_MEMCONF);
	int sparse_mem = 0;
	int ret;

	switch (memconf & SG_MEMCONF_CH0_SZ_MASK) {
	case SG_MEMCONF_CH0_SZ_64M:
		dram_ch0_size = SZ_64M;
		break;
	case SG_MEMCONF_CH0_SZ_128M:
		dram_ch0_size = SZ_128M;
		break;
	case SG_MEMCONF_CH0_SZ_256M:
		dram_ch0_size = SZ_256M;
		break;
	case SG_MEMCONF_CH0_SZ_512M:
		dram_ch0_size = SZ_512M;
		break;
	case SG_MEMCONF_CH0_SZ_1G:
		dram_ch0_size = SZ_1G;
		break;
	default:
		pr_err("unsupported configuration for DRAM Ch0\n");
		ret = -EINVAL;
		goto error1;
	}

	if ((memconf & SG_MEMCONF_CH0_NUM_MASK) == SG_MEMCONF_CH0_NUM_2)
		dram_ch0_size *= 2;

	switch (memconf & SG_MEMCONF_CH1_SZ_MASK) {
	case SG_MEMCONF_CH1_SZ_64M:
		dram_ch1_size = SZ_64M;
		break;
	case SG_MEMCONF_CH1_SZ_128M:
		dram_ch1_size = SZ_128M;
		break;
	case SG_MEMCONF_CH1_SZ_256M:
		dram_ch1_size = SZ_256M;
		break;
	case SG_MEMCONF_CH1_SZ_512M:
		dram_ch1_size = SZ_512M;
		break;
	case SG_MEMCONF_CH1_SZ_1G:
		dram_ch1_size = SZ_1G;
		break;
	case SG_MEMCONF_CH1_SZ_2G:
		dram_ch1_size = SZ_2G;
		break;
	default:
		pr_err("unsupported configuration for DRAM Ch1\n");
		ret = -EINVAL;
		goto error1;
	}

	if ((memconf & SG_MEMCONF_CH1_NUM_MASK) == SG_MEMCONF_CH1_NUM_2)
		dram_ch1_size *= 2;

	if (memconf & SG_MEMCONF_SPARSEMEM) {
		ret = get_dram_ch1_base(&dram_ch1_base);
		if (ret < 0)
			goto error2;

		if (dram_ch0_base + dram_ch0_size < dram_ch1_base)
			sparse_mem = 1;
		else
			/* truncate the tail of ch0 that overlaps with ch1 */
			dram_ch0_size = dram_ch1_base - dram_ch0_base;
	}			
	
	if (sparse_mem) {
		arm_add_mem_device("ram0", dram_ch0_base, dram_ch0_size);
		arm_add_mem_device("ram1", dram_ch1_base, dram_ch1_size);
	} else {
		arm_add_mem_device("ram0", dram_ch0_base,
				   dram_ch0_size + dram_ch1_size);
	}

	return 0;
error1:
	pr_err("SG_MEMCONF: 0x%08x\n", memconf);
error2:
	return ret;
}
mem_initcall(uniphier_mem_init);
