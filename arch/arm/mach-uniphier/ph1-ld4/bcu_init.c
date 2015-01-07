#include <asm/io.h>
#include <mach/init.h>
#include <mach/bcu-regs.h>

#define ddr_ch(x) ((x) >= 32 ? 0 : (x) < 0 ? 0x11111111 : 0x11111111 << (x))

static inline int bcu_init(const struct uniphier_board_param *bd)
{
	int shift;

	writel(0x44444444, BCSCR0); /* 0x20000000-0x3fffffff: ASM bus */
	writel(0x11111111, BCSCR2); /* 0x80000000-0x9fffffff: IPPC/IPPD-bus */
	writel(0x11111111, BCSCR3); /* 0xa0000000-0xbfffffff: IPPC/IPPD-bus */
	writel(0x11111111, BCSCR4); /* 0xc0000000-0xdfffffff: IPPC/IPPD-bus */
	writel(0x11111111, BCSCR5); /* 0xe0000000-0Xffffffff: IPPC/IPPD-bus */

	/* Specify DDR channel */
	shift = (bd->dram_ch1_base - bd->dram_ch0_base) / 0x04000000 * 4;
	writel(ddr_ch(shift), BCIPPCCHR2); /* 0x80000000-0x9fffffff */

	shift -= 32;
	writel(ddr_ch(shift), BCIPPCCHR3); /* 0xa0000000-0xbfffffff */

	shift -= 32;
	writel(ddr_ch(shift), BCIPPCCHR4); /* 0xc0000000-0xdfffffff */

	return 0;
}
ph1_ld4_init_func(bcu_init);
