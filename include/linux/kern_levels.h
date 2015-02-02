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

/* integer equivalents of KERN_<LEVEL> */
#define LOGLEVEL_SCHED		-2	/* Deferred messages from sched code
					 * are set to this special level */
#define LOGLEVEL_DEFAULT	-1	/* default (or last) loglevel */
#define LOGLEVEL_EMERG		0	/* system is unusable */
#define LOGLEVEL_ALERT		1	/* action must be taken immediately */
#define LOGLEVEL_CRIT		2	/* critical conditions */
#define LOGLEVEL_ERR		3	/* error conditions */
#define LOGLEVEL_WARNING	4	/* warning conditions */
#define LOGLEVEL_NOTICE		5	/* normal but significant condition */
#define LOGLEVEL_INFO		6	/* informational */
#define LOGLEVEL_DEBUG		7	/* debug-level messages */
#define LOGLEVEL_VDEBUG		8	/* verbose debug messages */

#endif
