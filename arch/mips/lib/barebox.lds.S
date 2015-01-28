/*
 * Copyright (C) 2011 Antony Pavlov <antonynpavlov@gmail.com>
 *
 * This file is part of barebox.
 * See file CREDITS for list of people who contributed to this project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#include <asm-generic/barebox.lds.h>

OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = TEXT_BASE;

	. = ALIGN(4);
	.text      :
	{
		_start = .;
		*(.text_entry*)
		_stext = .;
		_text = .;
		__bare_init_start = .;
		*(.text_bare_init*)
		__bare_init_end = .;
		*(.text*)
	}
	BAREBOX_BARE_INIT_SIZE

	PRE_IMAGE

	. = ALIGN(4);
	.rodata : { *(.rodata*) }

	_etext = .;			/* End of text and rodata section */
	_sdata = .;

	. = ALIGN(4);
	.data : { *(.data*) }

	.barebox_imd : { BAREBOX_IMD }

	. = ALIGN(4);
	.got : { *(.got*) }

	. = .;
	.barebox_cmd : { BAREBOX_CMDS }

	.barebox_magicvar : { BAREBOX_MAGICVARS }

	.barebox_initcalls : { INITCALLS }

	.barebox_exitcalls : { EXITCALLS }

	__usymtab : { BAREBOX_SYMS }

	.oftables : { BAREBOX_CLK_TABLE() }

	.dtb : { BAREBOX_DTB() }

	_edata = .;
	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss*) }
	__bss_stop = .;
	_end = .;
}
