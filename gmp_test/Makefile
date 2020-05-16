.PHONY: all
all: host ta

.PHONY: host
host:
	$(MAKE) -C host \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		TEEC_EXPORT=$(TEEC_EXPORT) \
		--no-builtin-variables

.PHONY: ta
ta:
	$(MAKE) -C ta \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		PLATFORM=$(PLATFORM) \
		TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)

.PHONY: clean
clean: host-clean ta-clean

.PHONY: host-clean
host-clean:
	$(MAKE) -C host clean

.PHONY: ta-clean
ta-clean:
	$(MAKE) -C ta clean \
		TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)
