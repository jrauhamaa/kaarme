# Kaarme

A [snake](https://en.wikipedia.org/wiki/Snake_(video_game_genre)) game running
inside a bootloader & implemented using
[BIOS API](https://en.wikipedia.org/wiki/BIOS_interrupt_call).

## Controls:

* Left arrow & right arrow - turn left and right
* n - new game

## How to run

### With QEMU

`make run`

### On real hardware

1. `make`
2. `sudo dd if=./build/all.bin of=<usb device>`
3. boot from the USB

