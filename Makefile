.PHONY: all buildroot menuconfig patch flash

# Q = @
Q = 

BUILDROOT_MAKE_ARGS = BR2_EXTERNAL_SBN_TREE_PATH=../buildroot-external

all: buildroot

buildroot:
	$(Q)make $(BUILDROOT_MAKE_ARGS) -C buildroot

menuconfig:
	$(Q)make $(BUILDROOT_MAKE_ARGS) -C buildroot menuconfig

patch:
	$(Q)echo Patching package directory
	$(Q)cp -r buildroot-external/package buildroot/
	$(Q)cp -r buildroot-external/package/rpi-firmware/config.txt buildroot/output/images/rpi-firmware/config.txt

flash:
	$(Q)sudo -k dd if=buildroot/output/images/sdcard.img of=/dev/mmcblk0 bs=4M
