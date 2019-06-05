# SBN Sampler

## Build steps:

    git submodule update --init
    make buildroot
    make sdcard

## Running the application:

Generate `sbnsampler`

    make app

Connect to the Raspberry Pi through Minicom on the host. Follow these steps to transfer the application from the host to the target:

1. On target: `rz`
2. On host: `Ctrl-a s -> zmodem ->` navigate to `app/sbnsampler` `-> space -> enter` (repeat for all `.wav` files.
3. On target: `./sbnsampler`

## Notes and TODOs

### Getting MIDI to work

Test midi:

    modprobe snd-seq
    aconnect -li

I think this will be needed for merging MIDI streams.

    modprobe snd-virmidi

## Links:

- <https://embeddedartistry.com/blog/2017/5/4/raspberry-pi-compute-module-3-getting-started-with-buildroot>
- <https://cellux.github.io/articles/diy-linux-with-buildroot-part-1/>
- <https://agentoss.wordpress.com/2011/03/02/how-to-build-a-tiny-linux-mp3-player-system-using-buildroot/>
- <https://blog.inf.re/buildroot-cheatsheet.html>
- <https://learn.pimoroni.com/tutorial/phat/raspberry-pi-phat-dac-install>
- <https://www.balena.io/blog/the-making-of-boombeastic/>
- <https://forums.pimoroni.com/t/raspberry-pi-zero-w-buildroot-pimoroni-phat-dac/6093>
