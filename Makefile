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
TA_DEV_KIT_DIR					?= $(OPTEE_OS_ROOT)/out/arm/export-ta_arm64

CPATH							?= $(TA_DEV_KIT_DIR)/include
LIBRARY_PATH                    ?= $(TA_DEV_KIT_DIR)/lib
CFLAGS							?= "-Os -g3 -fpic -mstrict-align -I$(TA_DEV_KIT_DIR)/include"

TEST_ROOT						?= $(ROOT)/gmp_test
PLATFORM						?= vexpress-qemu_armv8a
OPTEE_CLIENT_ROOT				?= $(DEVEL_ROOT)/optee_client
TEEC_EXPORT						?= $(OPTEE_CLIENT_ROOT)/out/export/usr


.PHONY: all
all: install-gmp test

.PHONY: test
test: install-gmp
	$(MAKE) -C $(TEST_ROOT) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		TEEC_EXPORT=$(TEEC_EXPORT) \
		PLATFORM=$(PLATFORM) \
		TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)

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
	./configure --disable-shared --disable-assembly --disable-cxx --host=aarch64-linu-gnu --prefix=$(TA_DEV_KIT_DIR) --exec-prefix=$(TA_DEV_KIT_DIR)

.PHONY: clean
clean: uninstall test_clean
	$(MAKE) -C $(GMP_ROOT) distclean

.PHONY: uninstall
uninstall:
	$(MAKE) -C $(GMP_ROOT) uninstall

.PHONY: test_clean
test_clean:
	$(MAKE) -C $(TEST_ROOT) clean \
		TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)