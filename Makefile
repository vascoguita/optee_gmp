GMP_VERSION						?= 6.2.0

ROOT							?= $(CURDIR)
GMP_ROOT						?= $(ROOT)/gmp-$(GMP_VERSION)
CONFIG_DIR						?= $(ROOT)/config

DEVEL_ROOT						?= $(ROOT)/..
TOOLCHAIN_ROOT					?= $(DEVEL_ROOT)/toolchains
AARCH64_PATH					?= $(TOOLCHAIN_ROOT)/aarch64
AARCH64_CROSS_COMPILE			?= $(AARCH64_PATH)/bin/aarch64-linux-gnu-
CROSS_COMPILE					?= $(AARCH64_CROSS_COMPILE)
CC								?= $(CROSS_COMPILE)gcc
LD								?= $(CROSS_COMPILE)ld
AR								?= $(CROSS_COMPILE)ar
NM								?= $(CROSS_COMPILE)nm
OBJCOPY							?= $(CROSS_COMPILE)objcopy
OBJDUMP							?= $(CROSS_COMPILE)objdump
READELF							?= $(CROSS_COMPILE)readelf

OPTEE_OS_ROOT					?= $(DEVEL_ROOT)/optee_os
OPTEE_OS_EXPORT					?= $(OPTEE_OS_ROOT)/out/arm/export-ta_arm64

CPATH							?= $(OPTEE_OS_EXPORT)/include
LIBRARY_PATH                    ?= $(OPTEE_OS_EXPORT)/lib
CFLAGS							?= "-Os -g3 -fpic -mstrict-align"

.PHONY: all
all: install-gmp

.PHONY: install-gmp
install-gmp: make-gmp
	$(MAKE) -C $(GMP_ROOT) install

.PHONY: make-gmp
make-gmp: modify-gmp
	$(MAKE) -C $(GMP_ROOT)

.PHONY: modify-gmp
modify-gmp: configure-gmp
	cp $(CONFIG_DIR)/config.h $(GMP_ROOT)/.

.PHONY: configure-gmp
configure-gmp:
	cd $(GMP_ROOT) && \
	CPATH=$(CPATH) LIBRARY_PATH=$(LIBRARY_PATH) \
	CC=$(CC) LD=$(LD) AR=$(AR) NM=$(NM) OBJCOPY=$(OBJCOPY) OBJDUMP=$(OBJDUMP) READELF=$(READELF) \
	CFLAGS=$(CFLAGS) \
	./configure --disable-shared --disable-assembly --disable-cxx --host=aarch64-linu-gnu --prefix=$(OPTEE_OS_EXPORT) --exec-prefix=$(OPTEE_OS_EXPORT)

.PHONY: clean
clean: uninstall
	$(MAKE) -C $(GMP_ROOT) distclean

.PHONY: uninstall
uninstall:
	$(MAKE) -C $(GMP_ROOT) uninstall