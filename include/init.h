#ifndef _INIT_H
#define _INIT_H

/*
 * fake define to simplify the linux sync
 */
#define __init
#define __initdata
#define __initconst
#define __exit
#define __exitdata
#define __exit_call

/* For assembly routines */
#define __BARE_INIT	.section ".text_bare_init.text","ax"

#ifndef __ASSEMBLY__
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

#define __define_initcall(level,fn,id) \
	static initcall_t __initcall_##fn##id __attribute__((__used__)) \
	__attribute__((__section__(".initcall." level))) = fn

#define __define_exitcall(level,fn,id) \
	static exitcall_t __exitcall_##fn##id __attribute__((__used__)) \
	__attribute__((__section__(".exitcall." level))) = fn


/*
 * A "pure" initcall has no dependencies on anything else, and purely
 * initializes variables that couldn't be statically initialized.
 *
 * This only exists for built-in code, not for modules.
 */
#define pure_initcall(fn)		__define_initcall("0",fn,0)

#define core_initcall(fn)		__define_initcall("1",fn,1)
#define postcore_initcall(fn)		__define_initcall("2",fn,2)
#define console_initcall(fn)		__define_initcall("3",fn,3)
#define postconsole_initcall(fn)	__define_initcall("4",fn,4)
#define mem_initcall(fn)		__define_initcall("5",fn,5)
#define mmu_initcall(fn)		__define_initcall("6",fn,6)
#define postmmu_initcall(fn)		__define_initcall("7",fn,7)
#define coredevice_initcall(fn)		__define_initcall("8",fn,8)
#define fs_initcall(fn)			__define_initcall("9",fn,9)
#define device_initcall(fn)		__define_initcall("10",fn,10)
#define crypto_initcall(fn)		__define_initcall("11",fn,11)
#define late_initcall(fn)		__define_initcall("12",fn,12)
#define environment_initcall(fn)	__define_initcall("13",fn,13)
#define postenvironment_initcall(fn)	__define_initcall("14",fn,14)

#define early_exitcall(fn)		__define_exitcall("0",fn,0)
#define predevshutdown_exitcall(fn)	__define_exitcall("1",fn,1)
#define devshutdown_exitcall(fn)	__define_exitcall("2",fn,2)
#define postdevshutdown_exitcall(fn)	__define_exitcall("3",fn,3)
#define prearchshutdown_exitcall(fn)	__define_exitcall("4",fn,4)
#define archshutdown_exitcall(fn)	__define_exitcall("5",fn,5)
#define postarchshutdown_exitcall(fn)	__define_exitcall("6",fn,6)

/* section for code used very early when
 * - we're not running from where we linked at
 * - bss not cleared
 * - static variables not initialized
 *
 * Mainly useful for booting from NAND Controllers
 */
#define __bare_init          __section(.text_bare_init.text)

#endif

#endif /* _INIT_H */

