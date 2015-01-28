#include <asm-generic/barebox.lds.h>

SECTIONS
{
	. = ALIGN(64);
	__barebox_initcalls : { INITCALLS }

	. = ALIGN(64);
	__barebox_exitcalls : { EXITCALLS }

	. = ALIGN(64);
	.barebox_magicvar : { BAREBOX_MAGICVARS }

	. = ALIGN(64);
	__barebox_cmd : { BAREBOX_CMDS }
}

INSERT BEFORE .rodata;
