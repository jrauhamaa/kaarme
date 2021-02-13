.PHONY: run binaries clean

binaries:
	$(MAKE) -C src
	mv src/*.bin build

run: binaries
	qemu-system-i386 -drive format=raw,file=build/all.bin

clean:
	rm -f build/*
