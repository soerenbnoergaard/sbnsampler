.PHONY: all buildroot

# Q = @
Q = 

all: buildroot

buildroot:
	$(Q)cp defconfig buildroot/.config
	$(Q)cp config.txt buildroot/package/rpi-firmware/config.txt
	$(Q)make -C buildroot
