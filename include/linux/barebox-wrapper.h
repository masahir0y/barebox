#ifndef __INCLUDE_LINUX_BAREBOX_WRAPPER_H
#define __INCLUDE_LINUX_BAREBOX_WRAPPER_H

#include <xfuncs.h>

#define kmalloc(len, mode)	malloc(len)
#define kzalloc(len, mode)	xzalloc(len)
#define vmalloc(len)		malloc(len)
#define kfree(ptr)		free(ptr)
#define vzalloc(len)		kzalloc(len, 0)
#define vfree(ptr)		free(ptr)

#define GFP_KERNEL	0

#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_LICENSE(x)
#define MODULE_ALIAS(x)

typedef int   spinlock_t;
#define spin_lock_init(...)
#define spin_lock(...)
#define spin_unlock(...)
static inline void spin_lock_irqsave(spinlock_t *lock, unsigned long flags) {}
static inline void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags) {}

#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)
struct mutex { int i; };

struct rw_semaphore { int i; };

#define init_rwsem(...)			do { } while (0)
#define down_read(...)			do { } while (0)
#define down_write(...)			do { } while (0)
#define down_write_trylock(...)		1
#define up_read(...)			do { } while (0)
#define up_write(...)			do { } while (0)
#define kthread_create(...)	__builtin_return_address(0)
#define kthread_stop(...)	do { } while (0)
#define wake_up_process(...)	do { } while (0)

typedef int	wait_queue_head_t;

#define cond_resched()			do { } while (0)

#define init_waitqueue_head(...)	do { } while (0)

typedef int irqreturn_t;
#define IRQ_NONE 0
#define IRQ_HANDLED 0

/* To ease clk drivers porting from Linux kernel */
#define __clk_get_name(clk)		(clk->name)
#define __clk_lookup			clk_lookup
#define __clk_get_rate			clk_get_rate
#define __clk_get_parent		clk_get_parent

#endif /* __INCLUDE_LINUX_BAREBOX_WRAPPER_H */
