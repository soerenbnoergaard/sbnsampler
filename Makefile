.PHONY: all buildroot menuconfig flash

# Q = @
Q = 

BUILDROOT_MAKE_ARGS = BR2_EXTERNAL_SBN_TREE_PATH=../buildroot-external

all: buildroot

buildroot:
	$(Q)cp defconfig buildroot/.config
	$(Q)cp config.txt buildroot/package/rpi-firmware/config.txt
	$(Q)make $(BUILDROOT_MAKE_ARGS) -C buildroot

menuconfig:
	$(Q)make $(BUILDROOT_MAKE_ARGS) -C buildroot menuconfig


flash:
	$(Q)sudo dd if=buildroot/output/images/sdcard.img of=/dev/mmcblk0 bs=4M
