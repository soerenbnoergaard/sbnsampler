# SBN Sampler

## Build steps

    git submodule update --init
    make buildroot
    make sdcard

## Running the application

Generate `sbnsampler`

    make app

Connect to the Raspberry Pi through Minicom on the host. Follow these steps to transfer the application from the host to the target:

1. On target: `rz`
2. On host: `Ctrl-a s -> zmodem ->` navigate to `app/sbnsampler` `-> space -> enter` (repeat for all `.wav` files.
3. On target: `./sbnsampler`

## Start application on boot

Add the following content to the file `/etc/init.d/S99sbnsampler`:

    HERE=$(pwd)

    case "$1" in
      start)
            printf "Starting sbnsampler: "
            cd /root/
            ./sbnsampler &
            cd $HERE
            [ $? = 0 ] && echo "OK" || echo "FAIL"
            ;;
      stop)
            printf "Stopping sbnsampler: "
            killall sbnsampler
            [ $? = 0 ] && echo "OK" || echo "FAIL"
            ;;
      restart|reload)
            "$0" stop
            "$0" start
            ;;
      *)
            echo "Usage: $0 {start|stop|restart}"
            exit 1
    esac

    exit $?

## Copy files from USB drive

This is much faster than transfering files with zmodem:

    mount /dev/sda /mnt
    cp /mnt/SOURCE ~/DESTINATION
    umount /mnt

## Good sample sources

Some files included in the `samples.csv` file cannot be uploaded to this repository as they are not mine to give away. Instead, consult some of the links below to get good samples.

- [Vintage Synth Sounds on Bandcamp](https://vintagesynthsamples.bandcamp.com/)
- [Sonic Bomm Mellotron Samples](https://sonicbloom.net/en/free-sb-mellotron-samples/)
- [Samples From Mars](https://samplesfrommars.com/)

## Notes and TODOs

Connecting several midi inputs:

    modprobe snd-virmidi
    aconnect 'microKONTROL':1 'Virtual Raw MIDI 0-0':0
    aconnect 'microKONTROL':2 'Virtual Raw MIDI 0-0':0

This should link midi inputs to a combined virtual midi port.


## Alsa slides

    https://elinux.org/images/8/82/Elc2011_lorriaux.pdf

## Links:

- <https://embeddedartistry.com/blog/2017/5/4/raspberry-pi-compute-module-3-getting-started-with-buildroot>
- <https://cellux.github.io/articles/diy-linux-with-buildroot-part-1/>
- <https://agentoss.wordpress.com/2011/03/02/how-to-build-a-tiny-linux-mp3-player-system-using-buildroot/>
- <https://blog.inf.re/buildroot-cheatsheet.html>
- <https://learn.pimoroni.com/tutorial/phat/raspberry-pi-phat-dac-install>
- <https://www.balena.io/blog/the-making-of-boombeastic/>
- <https://forums.pimoroni.com/t/raspberry-pi-zero-w-buildroot-pimoroni-phat-dac/6093>
