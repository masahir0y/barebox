obj-y += board.o
obj-y += clock.o
obj-y += hostfile.o
obj-y += console.o
obj-y += devices.o
obj-y += dtb.o
obj-y += power.o
obj-y += dev-random.o
obj-y += watchdog.o
obj-$(CONFIG_LED) += led.o

extra-y += barebox.lds

extra-y += stickypage.bin

OBJCOPYFLAGS_stickypage.bin = -O binary

%.bin: %.o
	$(call if_changed,objcopy)
