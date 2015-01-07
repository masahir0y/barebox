#include <linux/linkage.h>

/*
 * load image from NOR - faster than memcpy()
 *
 * r0: dest address
 * r1: size
 * return 0 for success
 */
ENTRY(nor_load)
	stmfd	sp!, {r4-r10}
	add	r1, r0, r1	@ dest_end
	mov	r2, #0		@ src
1:	ldmia	r2!, {r3-r10}
	stmia	r0!, {r3-r10}
	cmp	r0, r1
	blo	1b
	ldmfd	sp!, {r4-r10}
	mov	r0, #0		@ always suceed
	mov	pc, lr
ENDPROC(nor_load)
