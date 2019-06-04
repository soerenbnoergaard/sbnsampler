.PHONY: all buildroot menuconfig sdcard clean cleanall

# Q = @
Q = 

BUILDROOT_MAKE_ARGS = BR2_EXTERNAL=../external

all:
	@make -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'

buildroot:
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) clean
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) raspberrypi0_sbn_defconfig
	$(Q)time make -C buildroot $(BUILDROOT_MAKE_ARGS)

menuconfig:
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) menuconfig

sdcard:
	sudo -k dd bs=4M if=buildroot/output/images/sdcard.img of=/dev/mmcblk0

clean:
	$(Q)echo Doing nothing

cleanall:
	$(Q)echo Doing nothing
