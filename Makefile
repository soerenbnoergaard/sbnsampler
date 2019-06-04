.PHONY: all list app buildroot menuconfig sdcard clean cleanall

# Q = @
Q = 

BUILDROOT_MAKE_ARGS = BR2_EXTERNAL=../external

#
# General
#

all: app

list:
	@make -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'

#
# Application
#

app:
	$(Q)make -C app

clean:
	$(Q)make -C app clean

cleanall:
	$(Q)make -C app cleanall

#
# Buildroot
#

buildroot:
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) clean
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) raspberrypi0_sbn_defconfig
	$(Q)time make -C buildroot $(BUILDROOT_MAKE_ARGS)

menuconfig:
	$(Q)make -C buildroot $(BUILDROOT_MAKE_ARGS) menuconfig

sdcard:
	sudo -k dd bs=4M if=buildroot/output/images/sdcard.img of=/dev/mmcblk0
