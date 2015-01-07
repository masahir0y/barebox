/*
 * Copyright (C) 2011-2014 Panasonic Corporation
 *   Author: Masahiro Yamada <yamada.m@jp.panasonic.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _MACH_BOOT_DEVICE_H_
#define _MACH_BOOT_DEVICE_H_

#define BOOT_DEVICE_NOR		1
#define BOOT_DEVICE_NAND	2
#define BOOT_DEVICE_EMMC	3
#define BOOT_DEVICE_USB		4
#define BOOT_DEVICE_NONE	255

int nor_load(unsigned long dest, unsigned long size);
int nand_load(unsigned long dest, unsigned long size);
int emmc_load(unsigned long dest, unsigned long size);
int usb_load(unsigned long dest, unsigned long size);

int get_boot_device(void);

#endif /* _MACH_BOOT_DEVICE_H_ */
