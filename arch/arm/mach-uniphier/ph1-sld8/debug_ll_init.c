#include <linux/types.h>
#include <linux/bug.h>
#include <asm/io.h>
#include <mach/debug_ll.h>
#include <mach/sg-regs.h>
#include "set_pinsel.h"

#define UART_CLK	80000000

static inline void debug_ll_init(void)
{
#ifdef CONFIG_DEBUG_LL
	u32 tmp;

	uniphier_debug_uart_init(UART_CLK);

	switch (CONFIG_DEBUG_UNIPHIER_UART_PORT) {
	case 0:
		set_pinsel(70, 3);	/* HDDOUT0 -> TXD0 */
		set_pinsel(71, 3);	/* HSDOUT1 -> RXD0 */
		break;
	case 1:
		set_pinsel(114, 0);	/* TXD1 -> TXD1 */
		set_pinsel(115, 0);	/* RXD1 -> RXD1 */
		break;
	case 2:
		set_pinsel(112, 1);	/* SBO1 -> TXD2 */
		set_pinsel(113, 1);	/* SBI1 -> RXD2 */
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
