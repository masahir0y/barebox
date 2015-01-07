#include <linux/types.h>
#include <linux/bug.h>
#include <asm/io.h>
#include <mach/debug_ll.h>
#include <mach/sg-regs.h>
#include "set_pinsel.h"

#define UART_CLK	36864000

static inline void debug_ll_init(void)
{
#ifdef CONFIG_DEBUG_LL
	u32 tmp;

	uniphier_debug_uart_init(UART_CLK);

	switch (CONFIG_DEBUG_UNIPHIER_UART_PORT) {
	case 0:
		set_pinsel(85, 1);	/* HSDOUT3 -> RXD0 */
		set_pinsel(88, 1);	/* HDDOUT6 -> TXD0 */
		break;
	case 1:
		set_pinsel(69, 23);	/* PCIOWR -> TXD1 */
		set_pinsel(70, 23);	/* PCIORD -> RXD1 */
		break;
	case 2:
		set_pinsel(128, 13);	/* XIRQ6 -> TXD2 */
		set_pinsel(129, 13);	/* XIRQ7 -> RXD2 */
		break;
	default:
		BUILD_BUG_ON(CONFIG_DEBUG_UNIPHIER_UART_PORT < 0 ||
			     CONFIG_DEBUG_UNIPHIER_UART_PORT > 2);
		break;
	}

	tmp = readl(SG_IECTRL);
	tmp |= 1;
	writel(tmp, SG_IECTRL);
#endif
}
