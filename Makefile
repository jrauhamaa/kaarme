.PHONY: run binaries clean

binaries:
	if ! [ -d 'build' ]; then mkdir build; fi
	$(MAKE) -C src
	cp src/*.bin build

run: binaries
	qemu-system-i386 -drive format=raw,file=build/all.bin

clean:
	rm -f build/*
	$(MAKE) -C src clean
