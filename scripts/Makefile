###
# scripts contains sources for various helper programs used throughout
# barebox for the build process.

hostprogs-always-y					+= bin2c
hostprogs-always-y					+= mkimage
hostprogs-always-y					+= fix_size
hostprogs-always-y					+= bareboxenv
hostprogs-always-y					+= bareboxcrc32
hostprogs-always-y					+= kernel-install
hostprogs-always-$(CONFIG_CRYPTO_RSA_BUILTIN_KEYS)	+= rsatoc
HOSTCFLAGS_rsatoc = `pkg-config --cflags openssl`
HOSTLDLIBS_rsatoc = `pkg-config --libs openssl`
hostprogs-always-$(CONFIG_IMD)				+= bareboximd
hostprogs-always-$(CONFIG_KALLSYMS)			+= kallsyms
hostprogs-always-$(CONFIG_MIPS)				+= mips-relocs
hostprogs-always-$(CONFIG_MVEBU_HOSTTOOLS)		+= kwbimage kwboot mvebuimg
hostprogs-always-$(CONFIG_ARCH_OMAP)			+= omap_signGP mk-omap-image
hostprogs-always-$(CONFIG_ARCH_S5PCxx)			+= s5p_cksum
hostprogs-always-$(CONFIG_ARCH_DAVINCI)			+= mkublheader
HOSTCFLAGS_zynq_mkimage.o = -I$(srctree) -I$(srctree)/arch/arm/mach-zynq/include
hostprogs-always-$(CONFIG_ARCH_ZYNQ)			+= zynq_mkimage
hostprogs-always-$(CONFIG_ARCH_SOCFPGA)			+= socfpga_mkimage
hostprogs-always-$(CONFIG_ARCH_MXS)			+= mxsimage mxsboot
hostprogs-always-$(CONFIG_ARCH_LAYERSCAPE)		+= pblimage
hostprogs-always-$(CONFIG_ARCH_STM32MP)			+= stm32image
hostprogs-always-$(CONFIG_RISCV)			+= prelink-riscv
hostprogs-always-$(CONFIG_ARCH_ROCKCHIP)		+= rkimage
HOSTCFLAGS_rkimage = `pkg-config --cflags openssl`
HOSTLDLIBS_rkimage = `pkg-config --libs openssl`
KBUILD_HOSTCFLAGS += -I$(srctree)/scripts/include/
HOSTLDLIBS_mxsimage  = `pkg-config --libs openssl`
HOSTCFLAGS_omap3-usb-loader.o = `pkg-config --cflags libusb-1.0`
HOSTLDLIBS_omap3-usb-loader  = `pkg-config --libs libusb-1.0`
hostprogs-always-$(CONFIG_OMAP3_USB_LOADER)		+= omap3-usb-loader
HOSTCFLAGS_omap4_usbboot.o = `pkg-config --cflags libusb-1.0`
HOSTLDLIBS_omap4_usbboot = -lpthread `pkg-config --libs libusb-1.0`
hostprogs-always-$(CONFIG_OMAP4_HOSTTOOL_USBBOOT)	+= omap4_usbboot

userprogs-always-$(CONFIG_BAREBOXENV_TARGET)		+= bareboxenv-target
userprogs-always-$(CONFIG_KERNEL_INSTALL_TARGET)	+= kernel-install-target
userprogs-always-$(CONFIG_BAREBOXCRC32_TARGET)		+= bareboxcrc32-target
userprogs-always-$(CONFIG_IMD_TARGET)			+= bareboximd-target
userprogs-always-$(CONFIG_OMAP3_USB_LOADER_TARGET)	+= omap3-usb-loader-target
userprogs-always-$(CONFIG_OMAP4_USBBOOT_TARGET)		+= omap4_usbboot-target
userprogs-always-$(CONFIG_MVEBU_KWBOOT_TARGET)		+= kwboot-target

omap3-usb-loader-target-userccflags += `$(CROSS_PKG_CONFIG) --cflags libusb-1.0`
omap3-usb-loader-target-userldlibs += `$(CROSS_PKG_CONFIG) --libs libusb-1.0`
omap4_usbboot-target-userccflags += `$(CROSS_PKG_CONFIG) --cflags libusb-1.0`
omap4_usbboot-target-userldlibs += -lpthread `$(CROSS_PKG_CONFIG) --libs libusb-1.0`

userccflags += -I $(srctree)/$(src)/include -isystem $(srctree)/scripts/include

subdir-y			+= mod
subdir-y			+= imx
subdir-$(CONFIG_DTC)		+= dtc
subdir-$(CONFIG_ARCH_TEGRA)	+= tegra

# Let clean descend into subdirs
subdir-	+= basic kconfig
