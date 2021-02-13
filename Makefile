.PHONY: run default

default: bootloader.bin

bootloader.asm: *.inc
	touch bootloader.asm

bootloader.bin: bootloader.asm
	nasm -f bin bootloader.asm -o bootloader.bin

run: bootloader.bin
	qemu-system-i386 -drive format=raw,file=bootloader.bin

