
#include "fw_defs.h"
#include "../Err/Exception.h"
#include "../Logger/Log.h"

#define DEFINE_ERROR(err, text) \
    static const char *tag##err = text

#define ReturnErrorCase(err) \
    case err: return(tag##err)


DEFINE_ERROR(ERROR_BL_INCONSISTENT,"Bootloaders must be selected/deselected together.");
DEFINE_ERROR(ERROR_ROM_MUST_ENABLE,"The Scatter file does not correspond to this device,\nthe partition table is different.\nPlease select a matching Scatter file\nor select \"Firmware upgrade\" to format the device");
DEFINE_ERROR(ERROR_PMT_UNAVAILABLE,"Failed to get PMT info.");
DEFINE_ERROR(ERROR_DATA_LOST,      "Data will be lost.");
DEFINE_ERROR(ERROR_UNKNOWN_ROM,    "Unrecognized ROM detected.");
DEFINE_ERROR(ERROR_ILLEGAL_ADDRESS,"Invalid ROM or PMT address.");
DEFINE_ERROR(ERROR_FILE_NOT_EXIST, "Backup file does not exist.");
DEFINE_ERROR(ERROR_CHKSUM_FAIL,    "Checksum failed.");
DEFINE_ERROR(ERROR_CHK_RAW_FILE_COUNT_DIFF,    "Num of .chk file & num of .raw file mismatch.");
DEFINE_ERROR(ERROR_CHK_FILE_NOT_EXIST,         "Needed .chk file not exist.");


void fw_throw_error(ERROR_T err)
{
    THROW_APP_EXCEPTION(err, fw_error_text(err));
}

void fw_throw_error(ERROR_T err, const char *errMsg)
{
    THROW_APP_EXCEPTION(err, errMsg);
}

const char *fw_error_text(ERROR_T err)
{
    switch (err)
    {
    ReturnErrorCase(ERROR_BL_INCONSISTENT);
    ReturnErrorCase(ERROR_ROM_MUST_ENABLE);
    ReturnErrorCase(ERROR_PMT_UNAVAILABLE);
    ReturnErrorCase(ERROR_DATA_LOST);
    ReturnErrorCase(ERROR_UNKNOWN_ROM);
    ReturnErrorCase(ERROR_ILLEGAL_ADDRESS);
    ReturnErrorCase(ERROR_FILE_NOT_EXIST);
    ReturnErrorCase(ERROR_CHKSUM_FAIL);
    ReturnErrorCase(ERROR_CHK_RAW_FILE_COUNT_DIFF);
    ReturnErrorCase(ERROR_CHK_FILE_NOT_EXIST);

    default:
        LOGD("missing error definition: %d", err);
        return "";
    }
}
