#include <linux/linkage.h>
#include <asm/system.h>
#include <mach/ssc-regs.h>

ENTRY(jump_to_arm_entry)
	adr	r4, 1f
	ldm	r4, {r4, r5}
	sub	r4, r0, r4		@ relocation offset
	add	r5, r5, r4		@ 'here' after relocation
	add	r2, r2, r4		@ DTB after relocation
	mov	pc, r5			@ jump to relocated address
here:
	mrc	p15, 0, r4, c1, c0, 0	@ SCTLR (System Contrl Register)
	bic	r4, r4, #(CR_C | CR_M)	@ disable MMU and Dcache
	mcr	p15, 0, r4, c1, c0, 0

	ldr	r5, = SSCC
	ldr	r4, [r5]
	bic	r4, r4, #SSCC_ON	@ disable L2
	str	r4, [r5]

	b	barebox_arm_entry
	
1:
	.long	_stext
	.long	here
ENDPROC(jump_to_arm_entry)
