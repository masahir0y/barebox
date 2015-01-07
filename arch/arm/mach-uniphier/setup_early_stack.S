/*
 * Copyright (C) 2012-2013 Panasonic Corporation
 *   Author: Masahiro Yamada <yamada.m@jp.panasonic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <linux/linkage.h>
#include <asm/unified.h>
#include <asm/system.h>
#include <mach/sbc-regs.h>
#include <mach/ssc-regs.h>

/*
 *	based on arch/arm/mm/cache-v7.S of Linux 3.19
 *
 *	Corrupted registers: r0-r3, r12
 */
ENTRY(v7_invalidate_l1)
	mov	r0, #0
	mcr	p15, 2, r0, c0, c0, 0	@ CSSELR (Cache Size Selection Register)
	mrc	p15, 1, r0, c0, c0, 0	@ CCSIDR (Cache Size ID Registers)

	ldr	r1, =0x7fff
	and	r2, r1, r0, lsr #13	@ NumSets - 1

	and	r1, r0, #0x7
	add	r1, r1, #4		@ SetShift
	mov	r3, r2, lsl r1

	mov	r2, #1
	mov	r1, r2, lsl r1		@ 1 << SetShift

	ldr	r2, =0x3ff
	and	r2, r2, r0, lsr #3	@ NumWays - 1
	clz	r0, r2			@ WayShift
	mov	r2, r2, lsl r0		@ (NumWays - 1) << WayShift

	mov	r12, #-1
	mov	r0, r12, lsl r0		@ WayFieldMask

0:	orr	r3, r3, r2
1:	mcr	p15, 0, r3, c7, c6, 2	@ DCISW
	adds	r3, r0
	bcs	1b
	bic	r3, r0
	subs	r3, r3, r1
	bcs	0b
	dsb	st
	isb
	mov	pc, lr
ENDPROC(v7_invalidate_l1)

ENTRY(enable_mmu)
	mrc	p15, 0, r0, c2, c0, 2	@ TTBCR (Translation Table Base Control Register)
	bic	r0, r0, #0x37
	orr	r0, r0, #0x20		@ disable TTBR1
	mcr	p15, 0, r0, c2, c0, 2

	ldr	r0, =early_pgtable	@ page table must be 16KB aligned
	mcr	p15, 0, r0, c2, c0, 0   @ TTBR0

	mov	r0, #-1			@ manager for all domains (No permission check)
	mcr	p15, 0, r0, c3, c0, 0   @ DACR (Domain Access Control Register)

	dsb
	isb
	/*
	 * MMU on:
	 * TLBs was already invalidated in "../start.S"
	 * So, we don't need to invalidate it here.
	 */
	mrc	p15, 0, r0, c1, c0, 0	@ SCTLR (System Contrl Register)
	orr	r0, r0, #(CR_C | CR_M)	@ MMU and Dcache enable
	mcr	p15, 0, r0, c1, c0, 0
	mov	pc, lr
ENDPROC(enable_mmu)

/*
 *	Invalidate the whole L2 cache.
 */
ENTRY(l2_invalidate_all)
	ldr	r0, = SSCOQM_S_ALL | SSCOQM_CE | SSCOQM_CM_INV
	ldr	r1, = SSCOQM
	ldr	r2, = SSCOPPQSEF
0:
	str	r0, [r1]		@ issue a command
	ldr	r3, [r2]
	cmp	r3, #0x0		@ check if the command is successfully set
	bne	0b			@ try again if an error occurres

	ldr	r1, = SSCOLPQS
1:
	ldr	r0, [r1]
	cmp	r0, #0x4
	bne	1b			@ wait until the operation is completed
	str	r0, [r1]		@ clear the complete notification flag

	ldr	r0, = SSCOPE_CM_SYNC	@ drain internal buffers
	ldr	r1, = SSCOPE
	str	r0, [r1]		@ issue a command
	ldr	r0, [r1]		@ need a read back to confirm
	bx	lr
ENDPROC(l2_invalidate_all)

ENTRY(l2_cache_enable)
	ldr	r1, = SSCC
	ldr	r0, [r1]
	orr	r0, r0, #SSCC_ON	@ L2 enable
	str	r0, [r1]
	bx	lr
ENDPROC(l2_cache_enable)

#define EARLY_STACK_TOP		0x0ff00000
#define EARLY_STACK_SIZE	(SSC_WAY_SIZE)
#define EARLY_STACK_WAY		8

ENTRY(setup_init_ram)
	/* Touch to zero for the boot way */
0:
	/* set SSCOQM, SSCOQAD, SSCOQSZ, SSCOQWN in this order */
	ldr	r0, = 0x00408006	@ touch to zero with address range
	ldr	r1, = SSCOQM
	str	r0, [r1]
	ldr	r0, = EARLY_STACK_TOP	@ top of stack
	ldr	r1, = SSCOQAD
	str	r0, [r1]
	ldr	r0, = EARLY_STACK_SIZE
	ldr	r1, = SSCOQSZ
	str	r0, [r1]
	ldr	r0, = 1 << EARLY_STACK_WAY
	ldr	r1, = SSCOQWN
	str	r0, [r1]
	ldr	r1, = SSCOPPQSEF
	ldr	r0, [r1]
	cmp	r0, #0		@ check if the command is successfully set
	bne	0b			@ try again if an error occurres

	ldr	r1, = SSCOLPQS
1:
	ldr	r0, [r1]
	cmp	r0, #0x4
	bne	1b			@ wait until the operation is completed
	str	r0, [r1]		@ clear the complete notification flag

	mov	pc, lr
ENDPROC(setup_init_ram)

/*
 * r0-r3, r12: corrupted
 * sp:         set
 */
ENTRY(early_setup_c)
	mov	sp, lr			@ persevere link reg across call

	/*
	 * We must invalidate D cache in Secure mode.
	 * If in Non-secure mode, the only Non-secure entries
	 * are invalidated and Secure entries will remain in D cache.
	 */
	bl	v7_invalidate_l1	@ invalidate dcache

#ifdef CONFIG_HAVE_ARM_SECURE
	/*
	 * We want to enter Non-secure mode as soon as possible,
	 * because we want to use a normal debugger
	 * for the debug of following code.
	 */
	bl	switch_to_nonsecure
	/* Non-secure copy of SCTLR */
	mrc	p15, 0, r0, c1, c0, 0	@ SCTLR (System Contrl Register)
#ifndef CONFIG_SYS_ICACHE_OFF
	orr	r0, r0, #CR_I		@ Icache enable
#endif
	orr	r0, r0, #CR_A		@ Alginment abort enable
	orr	r0, r0, #CR_Z		@ Branch prediction enable
	mcr	p15, 0, r0, c1, c0, 0
#endif

#ifdef CONFIG_UNIPHIER_SMP
	/*
	 * ACTLR (Auxiliary Control Register) for Cortex-A9
	 * bit[9]  Parity on
	 * bit[8]  Alloc in one way
	 * bit[7]  EXCL (Exclusive cache bit)
	 * bit[6]  SMP
	 * bit[3]  Write full line of zeros mode
	 * bit[2]  L1 Prefetch enable
	 * bit[1]  L2 prefetch enable
	 * bit[0]  FW (Cache and TLB maintenance broadcast)
	 */
	mrc	p15, 0, r0, c1, c0, 1	@ ACTLR (Auxiliary Control Register)
	orr	r0, r0, #0x41		@ enable SMP, FW bit
	mcr	p15, 0, r0, c1, c0, 1

	/* branch by CPU ID */
	mrc	p15, 0, r0, c0, c0, 5	@ MPIDR (Multiprocessor Affinity Register)
	and  	r0, r0, #0x3
	cmp	r0, #0x0
	beq	primary_cpu
	ldr	r1, =ROM_BOOT_ROMRSV2
	mov	r0, #0
	str	r0, [r1]
0:	wfe
	ldr	r0, [r1]		@ r0: entry point of U-Boot main for the secondary CPU
	cmp	r0, #0
	beq	0b
	bx	r0			@ secondary CPUs jump to linux
primary_cpu:
	mrc	p15, 4, r1, c15, c0, 0	@ Configuration Base Address Register
	bfc	r1, #0, #13		@ clear bit 12-0
	mov	r0, #-1
	str	r0, [r1, #SCU_INV_ALL]	@ SCU Invalidate All Register
	mov	r0, #1			@ SCU enable
	str	r0, [r1, #SCU_CTRL]	@ SCU Control Register
#endif

	bl	enable_mmu

	bl	l2_invalidate_all
	bl	l2_cache_enable		@ we use L2 as temporary RAM until DDR becomes available

	bl	setup_init_ram		@ RAM area for temporary stack pointer

	mov	lr, sp			@ restore link

					@ 8-byte alignment for ABI compliance
	ldr	sp, = ((EARLY_STACK_TOP + EARLY_STACK_SIZE - 4) >> 3) << 3

	mov	pc, lr			@ back to my caller
ENDPROC(early_setup_c)

/* page table */
#define NR_SECTIONS	4096
#define SECTION_SHIFT	20
#define DEVICE	0x00002002 /* Non-shareable Device */
#define NORMAL	0x0000000e /* Normal Memory Write-Back, No Write-Allocate */

#define STACK_SECTION	((EARLY_STACK_TOP) >> (SECTION_SHIFT))

	.section ".rodata"
	.align 14
early_pgtable:
	section = 0
	.rept NR_SECTIONS
	.if section == STACK_SECTION
	attr = NORMAL
	.else
	attr = DEVICE
	.endif
	.word (section << SECTION_SHIFT) | attr
	section = section + 1
	.endr
end_early_pgtable:
