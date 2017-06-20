#ifndef FW_DEFS_H
#define FW_DEFS_H

#include "../BootRom/flashtool_handle.h"

typedef enum __ERROR_T
{
    ERROR_OK = 8500,
    ERROR_BL_INCONSISTENT,
    ERROR_ROM_MUST_ENABLE,
    ERROR_PMT_UNAVAILABLE,
    ERROR_DATA_LOST,
    ERROR_ILLEGAL_ADDRESS,
    ERROR_UNKNOWN_ROM,
    ERROR_CHKSUM_FAIL,
    ERROR_FILE_NOT_EXIST,
    ERROR_CHK_RAW_FILE_COUNT_DIFF,
    ERROR_CHK_FILE_NOT_EXIST
} ERROR_T;

void fw_throw_error(ERROR_T);
void fw_throw_error(ERROR_T err, const char*);

const char *fw_error_text(ERROR_T);

#endif // FW_DEFS_H
