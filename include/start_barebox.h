#ifndef _START_BAREBOX_H
#define _START_BAREBOX_H

#include <linux/linkage.h>
#include <linux/compiler.h>

/* Define the prototype for start_barebox here, rather than cluttering
   up something else. */

extern asmlinkage void __noreturn start_barebox(void);

#endif /* _START_BAREBOX_H */
