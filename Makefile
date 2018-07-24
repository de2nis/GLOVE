.PHONY: default

NPROCS := $(shell grep -c ^processor /proc/cpuinfo)

default:
	@cd build && make -j $(NPROCS)

install:
	@cd build && make install

uninstall:
	@cd build && cat install_manifest.txt | grep .so | xargs rm

clean:
	@rm -rf build cross_build

cross:
	@cd cross_build && make -j $(NPROCS)

cross_install:
	@cd cross_build && make install
