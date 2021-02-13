.PHONY: default clean

default: all.bin

%.asm: *.inc %/*.inc
	touch $@

%.bin: %.asm
	nasm -f bin $< -o $@

all.bin: bootloader.bin kaarme.bin
	cat bootloader.bin kaarme.bin > all.bin

clean:
	rm -f *.bin
