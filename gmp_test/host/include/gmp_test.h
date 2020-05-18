#ifndef GMP_TEST_H
#define GMP_TEST_H

#include <stdio.h>
#include <tee_client_api.h>

void prepare_tee_session(TEEC_Context *ctx, TEEC_Session *sess);
void terminate_tee_session(TEEC_Context *ctx, TEEC_Session *sess);
#endif