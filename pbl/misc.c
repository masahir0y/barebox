#include <common.h>
#include <init.h>
#include <linux/bug.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

void __noreturn hang(void)
{
	while (1);
}

void __noreturn panic(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	printf(fmt, args);
	va_end(args);
	while(1);
}

void __noreturn __bug(const char *file, unsigned int line, const char *func)
{
	printk("BUG: failure at %s:%d/%s()!\n", file, line, func);
	panic("BUG!");
}

void __warn(const char *file, unsigned int line, const char *func)
{
	printf("WARNING: at %s:%d/%s()!\n", file, line, func);
}
