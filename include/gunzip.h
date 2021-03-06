/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef GUNZIP_H
#define GUNZIP_H

int gunzip(unsigned char *inbuf, int len,
	   int(*fill)(void*, unsigned int),
	   int(*flush)(void*, unsigned int),
	   unsigned char *output,
	   int *pos,
	   void(*error_fn)(char *x));
#endif
