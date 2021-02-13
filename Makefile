.PHONY: run default

default: all.bin

%.asm: *.inc
	touch $@

%.bin: %.asm
	nasm -f bin $< -o $@

all.bin: bootloader.bin kaarme.bin
	cat bootloader.bin kaarme.bin > all.bin

run: all.bin
	qemu-system-i386 -drive format=raw,file=all.bin

