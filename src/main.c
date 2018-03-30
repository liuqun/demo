#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include "tool.h"
#include <stdio.h>

typedef enum {
    OPTION_DEFAULT=0,
    OPTION_MSSIM=0,
    OPTION_DEVICE=1,
    OPTION_TABRMD=2,
} option_t;

void foobar(option_t opt)
{
const struct {
    char *lib_name;
    char *func_symbol;
    char *default_opts;
} KNOWN_TCTI_LIST[] = {
    {
        .lib_name="libtss2-tcti-mssim.so",
        .func_symbol="Tss2_Tcti_Mssim_Init",
        .default_opts="tcp://127.0.0.1:2321",
    },
    {
        .lib_name="libtss2-tcti-device.so",
        .func_symbol="Tss2_Tcti_Device_Init",
        .default_opts="/dev/tpm0",
    },
    {
        .lib_name="libtss2-tcti-tabrmd.so",
        .func_symbol="Tss2_Tcti_Tabrmd_Init",
        .default_opts="bus_type=system,bus_name=com.intel.tss2.Tabrmd",
    },
};
    const char *tcti_lib_name = KNOWN_TCTI_LIST[opt].lib_name;
    const char *tcti_func_symbol = KNOWN_TCTI_LIST[opt].func_symbol;
    const char *tcti_opts = KNOWN_TCTI_LIST[opt].default_opts;
    TSS2_TCTI_CONTEXT *tcti_ctx = NULL;
    void *dll_handle;
    dll_handle = dlopen(tcti_lib_name, RTLD_NOW);
//printf("dll_handle = %p\n", dll_handle);
    if (!dll_handle) {
        return;  // TODO: Report an error.
    }
    TSS2_TCTI_INIT_FUNC Tss2_Tcti_Initialize = NULL;
    Tss2_Tcti_Initialize = (TSS2_TCTI_INIT_FUNC) dlsym(dll_handle, tcti_func_symbol);
    size_t size;
    TSS2_RC err;
    size = 0;
    err = Tss2_Tcti_Initialize(NULL, &size, tcti_opts);
    if (err) {
        goto CLEANUP_TCTI_CTX;
    }
    tcti_ctx = malloc(size);
    memset(tcti_ctx, 0x00, size);
    err = Tss2_Tcti_Initialize(tcti_ctx, &size, tcti_opts);
    if (err) {
        goto CLEANUP_TCTI_CTX;
    }
    Tss2_Tcti_Finalize(tcti_ctx);
CLEANUP_TCTI_CTX:
    if (dll_handle) {
        dlclose(dll_handle);
    }
    if (tcti_ctx) {
        free(tcti_ctx);
        tcti_ctx = NULL;
    }
    return;
}

int main()
{
    foobar(OPTION_MSSIM);
    return (0);
}

int fun()
{
    TSS2_RC err;
    size_t size;
    TSS2_SYS_CONTEXT *sys_ctx = NULL;
    TSS2_TCTI_CONTEXT *tcti_ctx = NULL;
    TSS2_ABI_VERSION any;
    memset(&any, 0x00, sizeof(TSS2_ABI_VERSION));
    guess_tss2_abi_version(&any);
    size = Tss2_Sys_GetContextSize(0);
    sys_ctx = malloc(size);
    memset(sys_ctx, 0x00, size);
    err = Tss2_Sys_Initialize(sys_ctx, size, tcti_ctx, &any);
    switch (err) {
        case TSS2_RC_SUCCESS:
            break;
        case TSS2_SYS_RC_BAD_REFERENCE:
            break;
        case TSS2_SYS_RC_ABI_MISMATCH:
            // Try with NULL to suppress ABI version check.
            err = Tss2_Sys_Initialize(sys_ctx, size, tcti_ctx, NULL);
            break;
        default:
            break;
    }
    if (err) {
        goto CLEANUP;
    }
    Tss2_Sys_Finalize(sys_ctx);
    //Tss2_Tcti_Finalize(tcti_ctx);
CLEANUP:
    if (sys_ctx) {
        free(sys_ctx);
        sys_ctx = NULL;
    }
    if (tcti_ctx) {
        free(tcti_ctx);
        tcti_ctx = NULL;
    }
    return ((int) err);
}
