GMP_VERSION						?= 6.2.0

ROOT							?= $(CURDIR)
GMP_ROOT						?= $(ROOT)/gmp-$(GMP_VERSION)
CONFIG_DIR						?= $(ROOT)/config

CC								?= $(CROSS_COMPILE)gcc
LD								?= $(CROSS_COMPILE)ld
AR								?= $(CROSS_COMPILE)ar
NM								?= $(CROSS_COMPILE)nm
OBJCOPY							?= $(CROSS_COMPILE)objcopy
OBJDUMP							?= $(CROSS_COMPILE)objdump
READELF							?= $(CROSS_COMPILE)readelf

CPATH							?= $(TA_DEV_KIT_DIR)/include
LIBRARY_PATH                    ?= $(TA_DEV_KIT_DIR)/lib
CFLAGS							?= "-Os -g3 -fpic -mstrict-align -I$(TA_DEV_KIT_DIR)/include"

.PHONY: all
all: build

.PHONY: install
install:
	$(MAKE) -C $(GMP_ROOT) install

.PHONY: build
build: configure
	$(MAKE) -C $(GMP_ROOT)

.PHONY: configure
configure:
	cd $(GMP_ROOT) && \
	CPATH=$(CPATH) LIBRARY_PATH=$(LIBRARY_PATH) \
	CC=$(CC) LD=$(LD) AR=$(AR) NM=$(NM) OBJCOPY=$(OBJCOPY) OBJDUMP=$(OBJDUMP) READELF=$(READELF) \
	CFLAGS=$(CFLAGS) \
	./configure --disable-shared --disable-assembly --disable-cxx --host=aarch64-linux-gnu --prefix=$(TA_DEV_KIT_DIR) --exec-prefix=$(TA_DEV_KIT_DIR) && \
	cp $(CONFIG_DIR)/config.h $(GMP_ROOT)/.

.PHONY: clean
clean:
	$(MAKE) -C $(GMP_ROOT) distclean

.PHONY: uninstall
uninstall:
	$(MAKE) -C $(GMP_ROOT) uninstall;