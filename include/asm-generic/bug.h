#ifndef _ASM_GENERIC_BUG_H
#define _ASM_GENERIC_BUG_H

#include <linux/compiler.h>

void __noreturn __bug(const char *file, unsigned int line, const char *func);
void __warn(const char *file, unsigned int line, const char *func);

#define BUG()		__bug(__FILE__, __LINE__, __FUNCTION__)
#define __WARN()	__warn(__FILE__, __LINE__, __FUNCTION__)

#define BUG_ON(condition) do { if (unlikely((condition)!=0)) BUG(); } while(0)

#ifndef WARN_ON
#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN();						\
	unlikely(__ret_warn_on);					\
})
#endif

#ifndef WARN
#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on)) {					\
		__WARN();						\
		puts("WARNING: ");					\
		printf(format);						\
	}								\
	unlikely(__ret_warn_on);					\
})
#endif
#endif
