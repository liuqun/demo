#include "tool.h"

/** Guess TSS2_ABI_VERSION numbers
 *
 * ABI version checking method was changed by upstream developers. Here we are
 * trying to guess out the hidden version numbers in the master branch of
 * tpm2-tss. In the future, we may pull those magic version numbers directly
 * from "Tss2_Sys_Initialize.c" of upstream source tree. See:
 * https://github.com/tpm2-software/tpm2-tss/pull/864
 */
const TSS2_ABI_VERSION guess_tss2_abi_version(TSS2_ABI_VERSION *answer) {
#ifndef TSSWG_INTEROP
    const uint32_t TSSWG_INTEROP = 1;
#endif
#ifndef TSS_SAPI_FIRST_FAMILY
    const uint32_t TSS_SAPI_FIRST_FAMILY = 2;
#endif
#ifndef TSS_SAPI_FIRST_LEVEL
    const uint32_t TSS_SAPI_FIRST_LEVEL = 1;
#endif
#ifndef TSS_SAPI_FIRST_VERSION
    const uint32_t TSS_SAPI_FIRST_VERSION = 108;
#endif

    answer->tssCreator = TSSWG_INTEROP;
    answer->tssFamily = TSS_SAPI_FIRST_FAMILY;
    answer->tssLevel = TSS_SAPI_FIRST_LEVEL;
    answer->tssVersion = TSS_SAPI_FIRST_VERSION;

    return (*answer);
}
