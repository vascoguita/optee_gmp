# optee_gmp
This repository holds a port of the [GMP](https://gmplib.org) library to [OP-TEE](https://www.op-tee.org).

The current port only covers AArch64.

## Disclaimer
This library was ported for prototyping **only**. It was not extensively tested and it can not be considered secure.
Do **not** use it as part of a production setup.

# Install Instructions
    make CROSS_COMPILE=<path/to/aarch64-linux-gnu-> TA_DEV_KIT_DIR=<optee_os>/out/arm/export-ta_arm64 --no-builtin-variables
    make install