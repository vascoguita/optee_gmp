#include <tee_internal_api.h>

#include <gmp_test_ta.h>

#include <gmp.h>
#include <stdlib.h>

TEE_Result TA_CreateEntryPoint(void) {
    mpz_t n;
    DMSG("has been called");

    mpz_init(n);

    DMSG("INIT DONE");

    mpz_set_ui(n,0);

    DMSG("SET DONE");

    mpz_clear(n);

    DMSG("CLEAR DONE");

    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void) {
    DMSG("has been called");
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types, TEE_Param params[4], void **sess_ctx) {
    uint32_t exp_param_types;
    
    (void)&params;
	(void)&sess_ctx;
    
    DMSG("has been called");

    exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
                                        TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);
	if(param_types != exp_param_types) {
		return TEE_ERROR_BAD_PARAMETERS;
    }

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ctx) {
    (void)&sess_ctx;

    DMSG("has been called");
}

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd, uint32_t param_types, TEE_Param params[4]) {
    (void)&sess_ctx;
    (void)&params;
    (void)param_types;

    switch(cmd) {
        default:
            return TEE_ERROR_NOT_SUPPORTED;
    }
}