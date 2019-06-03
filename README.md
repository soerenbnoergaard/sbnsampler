# SBN Sampler

## Build steps:

    git submodule update --init
    make
    make flash

## (Temporary) manual steps

1. Add the lines below to `/boot/config.txt`:

2. Copy the content of `buildroot/output/build/rpi-firmware-*/boot/overlays/` to `/boot/overlays/`.


`/boot/config.txt`:

    enable_uart=1
    dtparam=i2c_arm=on
    dtparam=spi=on
    dtoverlay=hifiberry-dac

## Notes and TODOs

### Updating config.txt on boot

These may be installed using a custom `post-image.sh` script, I expect, also updating the `config.txt` file. See the procedure here:

- <https://github.com/badaix/snapos/tree/master/buildroot-external>

I think these lines must be added to the config.txt:

    enable_uart=1
    dtoverlay=hifiberry-dac

### Loading the correct modules after boot

I think these will be needed for the project:

    modprobe snd-virmidi

## Links:

- <https://embeddedartistry.com/blog/2017/5/4/raspberry-pi-compute-module-3-getting-started-with-buildroot>
- <https://cellux.github.io/articles/diy-linux-with-buildroot-part-1/>
- <https://agentoss.wordpress.com/2011/03/02/how-to-build-a-tiny-linux-mp3-player-system-using-buildroot/>
- <https://blog.inf.re/buildroot-cheatsheet.html>
- <https://learn.pimoroni.com/tutorial/phat/raspberry-pi-phat-dac-install>
- <https://www.balena.io/blog/the-making-of-boombeastic/>
- <https://forums.pimoroni.com/t/raspberry-pi-zero-w-buildroot-pimoroni-phat-dac/6093>
