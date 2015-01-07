#include <debug_ll.h>
#include <printk.h>
#include <linux/compiler.h>
#include <linux/sizes.h>
#include <asm/sections.h>
#include <asm/barebox-arm.h>
#include <mach/init.h>
#include <mach/boot-device.h>

static int load_image(unsigned long dest)
{
	int ret;
	unsigned long image_size = *(unsigned long *)(ld_var(_text) + 0x2c);

	pr_info("Load image (dest = 0x%08lx, size = %lu)\n", dest, image_size);

	switch (get_boot_device()) {
#ifdef CONFIG_UNIPHIER_NOR_BOOT
	case BOOT_DEVICE_NOR:
		pr_info("loading image from NOR...\n");
		ret = nor_load(dest, image_size);
		break;
#endif
#ifdef CONFIG_UNIPHIER_NAND_BOOT
	case BOOT_DEVICE_NAND:
		pr_info("loading image from NAND...\n");
		ret = nand_load(dest, image_size);
		break;
#endif
#ifdef CONFIG_UNIPHIER_EMMC_BOOT
	case BOOT_DEVICE_EMMC:
		pr_info("loading image from EMMC...\n");
		ret = emmc_load(dest, image_size);
		break;
#endif
#ifdef CONFIG_UNIPHIER_USB_BOOT
	case BOOT_DEVICE_USB:
		pr_info("loading image from USB...\n");
		ret = usb_load(dest, image_size);
		break;
#endif
	default:
		puts_ll("unsupported boot device\n");
		ret = -1;
		break;
	}

	return ret;
}

void __noreturn uniphier_entry(void *boarddata)
{
	int ret;
	unsigned long membase = 0x88000000;

	ret = load_image(membase);
	if (ret < 0) {
		puts_ll("error: fail to load image\nstop\n");
		while (1)
			;
	}

	jump_to_arm_entry(membase, SZ_16M, boarddata);
}
