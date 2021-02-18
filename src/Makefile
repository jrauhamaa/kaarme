.PHONY: default clean

default: all.bin

%.asm: utils/*.inc config
	touch $@

%.bin: %.asm
	nasm -f bin $< -o $@

all.bin: bootloader.bin kaarme.bin
	cat bootloader.bin kaarme.bin > all.bin

clean:
	rm -f *.bin
