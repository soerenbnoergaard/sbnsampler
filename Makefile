.PHONY: all buildroot menuconfig patch flash clean cleanall

# Q = @
Q = 

BUILDROOT_MAKE_ARGS = BR2_EXTERNAL=../external

all: buildroot

buildroot:
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) raspberrypi0_sbn_defconfig
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS)

menuconfig:
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) menuconfig

patch:
	$(Q)echo Patching package directory
	$(Q)cp -r external/package buildroot/
	$(Q)cp -r external/package/rpi-firmware/config.txt buildroot/output/images/rpi-firmware/config.txt

flash:
	$(Q)sudo -k dd bs=4M if=buildroot/output/images/sdcard.img of=/dev/mmcblk0

clean:
	$(Q)echo Doing nothing

cleanall:
	$(Q)make $(BUILDROOT_MAKE_ARGS) -C buildroot clean
