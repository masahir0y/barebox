#ifndef __KERN_LEVELS_H__
#define __KERN_LEVELS_H__

#define KERN_EMERG	""	/* system is unusable */
#define KERN_ALERT	""	/* action must be taken immediately */
#define KERN_CRIT	""	/* critical conditions */
#define KERN_ERR	""	/* error conditions */
#define KERN_WARNING	""	/* warning conditions */
#define KERN_NOTICE	""	/* normal but significant condition */
#define KERN_INFO	""	/* informational */
#define KERN_DEBUG	""	/* debug-level messages */

/*
 * Annotation for a "continued" line of log printout (only done after a
 * line that had no enclosing \n). Only to be used by core/arch code
 * during early bootup (a continued line is not SMP-safe otherwise).
 */
#define KERN_CONT	""

#endif
