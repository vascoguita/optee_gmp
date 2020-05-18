#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <gmp_test_ta.h>

#define TA_UUID         TA_GMP_TEST_UUID

#define TA_FLAGS        TA_FLAG_EXEC_DDR

#define TA_STACK_SIZE   (16 * 1024)
#define TA_DATA_SIZE    (128 * 1024)

#define TA_VERSION	"0.1"

#define TA_DESCRIPTION	"TA for testing GMP"

#endif