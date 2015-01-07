#include <common.h>
#include <of.h>
#include <driver.h>

static int uniphier_serial_probe(struct device_d *dev)
{
	return 0;
}

static void uniphier_serial_remove(struct device_d *dev)
{

}

#ifdef CONFIG_OFDEVICE
static struct of_device_id uniphier_serial_of_match[] = {
	{ .compatible = "panasonic,uniphier-uart" },
	{ /* sentinel */ }
};
#endif

static struct driver_d uniphier_serial_driver = {
	.name = "uniphier_serial",
	.probe = uniphier_serial_probe,
	.remove = uniphier_serial_remove,
	.of_compatible = DRV_OF_COMPAT(uniphier_serial_of_match),
};
console_platform_driver(uniphier_serial_driver);
