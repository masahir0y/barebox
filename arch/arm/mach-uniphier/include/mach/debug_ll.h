/*
 * Copyright (C) 2011-2014 Panasonic Corporation
 *   Author: Masahiro Yamada <yamada.m@jp.panasonic.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef __MACH_DEBUG_LL_H__
#define __MACH_DEBUG_LL_H__

#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/serial_reg.h>
#include <asm/io.h>

#define DEBUG_UART_PORT		(CONFIG_DEBUG_UNIPHIER_UART_PORT)
#define DEBUG_UART_BASE		(0x54006800 + 0x100 * (DEBUG_UART_PORT))
#define DEBUG_UART_BAUDRATE	(CONFIG_DEBUG_UNIPHIER_UART_BAUDRATE)

#define UART_SHIFT 2
#define UNIPHIER_UART_TX	((UART_TX) << (UART_SHIFT))
#define UNIPHIER_UART_LSR	((UART_LSR) << (UART_SHIFT))
#define UNIPHIER_UART_LCR	0x11
#define UNIPHIER_UART_DLR	0x24

static inline void uniphier_debug_uart_init(unsigned int uartclk)
{
#ifdef CONFIG_DEBUG_LL
	const unsigned int mode_x_div = 16;
	void __iomem *base = (void __iomem *)DEBUG_UART_BASE;
	unsigned int div = DIV_ROUND_CLOSEST(uartclk,
					     mode_x_div * DEBUG_UART_BAUDRATE);

	writeb(UART_LCR_WLEN8, base + UNIPHIER_UART_LCR);
	writew(div, base + UNIPHIER_UART_DLR);
#endif
}

static inline void PUTC_LL(int c)
{
#ifdef CONFIG_DEBUG_LL
	void __iomem *base = (void __iomem *)DEBUG_UART_BASE;

	while (!(readb(base + (UART_LSR << UART_SHIFT)) & UART_LSR_THRE))
		;

	writeb(c, base + (UART_TX << UART_SHIFT));
#endif
}

#endif /* __MACH_DEBUG_LL_H__ */
