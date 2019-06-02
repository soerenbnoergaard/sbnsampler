.PHONY: all buildroot flash

# Q = @
Q = 

all: buildroot

buildroot:
	$(Q)cp defconfig buildroot/.config
	$(Q)cp config.txt buildroot/package/rpi-firmware/config.txt
	$(Q)make -C buildroot

flash:
	$(Q)sudo dd if=buildroot/output/images/sdcard.img of=/dev/mmcblk0 bs=4M
