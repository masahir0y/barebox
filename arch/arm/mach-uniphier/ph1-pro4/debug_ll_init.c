#include <linux/types.h>
#include <linux/bug.h>
#include <asm/io.h>
#include <mach/debug_ll.h>
#include <mach/sc-regs.h>
#include <mach/sg-regs.h>
#include "set_pinsel.h"

#define UART_CLK	73728000

static inline void debug_ll_init(void)
{
#ifdef CONFIG_DEBUG_LL
	u32 tmp;

	tmp = readl(SC_CLKCTRL);
	tmp |= SC_CLKCTRL_CLK_PERI;
	writel(tmp, SC_CLKCTRL);

	uniphier_debug_uart_init(UART_CLK);

	switch (CONFIG_DEBUG_UNIPHIER_UART_PORT) {
	case 0:
		set_pinsel(127, 0);	/* RXD0 -> RXD0 */
		set_pinsel(128, 0);	/* TXD0 -> TXD0 */
		break;
	case 1:
		set_pinsel(129, 0);	/* RXD1 -> RXD1 */
		set_pinsel(130, 0);	/* TXD1 -> TXD1 */
		break;
	case 2:
		set_pinsel(131, 0);	/* RXD2 -> RXD2 */
		set_pinsel(132, 0);	/* TXD2 -> TXD2 */
		break;
	default:
		BUILD_BUG_ON(CONFIG_DEBUG_UNIPHIER_UART_PORT < 0 ||
			     CONFIG_DEBUG_UNIPHIER_UART_PORT > 2);
		break;
	}

	writel(1, SG_LOADPINCTRL);

	tmp = readl(SG_IECTRL);
	tmp |= 1;
	writel(tmp, SG_IECTRL);
#endif
}
