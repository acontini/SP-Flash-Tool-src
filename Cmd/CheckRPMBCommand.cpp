#include "CheckRPMBCommand.h"
#include "../Conn/Connection.h"
#include "../BootRom/flashtoolex_api.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

namespace APCore
{

CheckRPMBCommand::CheckRPMBCommand(APKey key):
    ICommand(key)
{
}

CheckRPMBCommand::~CheckRPMBCommand()
{
}

void CheckRPMBCommand::exec(const QSharedPointer<APCore::Connection> &conn)
{
    conn->ConnectDA();

    LOGI("call flashtool_check_rpmb_written");
    flash_hw_status_e rpmb_status = EMMC_RPMB_KEY_NOT_EXIST;
    int ret = flashtool_get_rpmb_status((HSESSION)conn->FTHandle(), &rpmb_status);
    if(ret != STATUS_OK)
    {
        LOGE("flashtool_get_rpmb_status() failed! ret: 0x%x", ret);
        THROW_BROM_EXCEPTION(ret, 0);
    }
    LOGI("flashtool_get_rpmb_status() , rmpb_written: %d.", rpmb_status);

    //if rpmb_written, NOT download anymore, show one err_msg
    if(rpmb_status == EMMC_RPMB_KEY_EXIST)
    {
        THROW_APP_EXCEPTION(FT_RPMB_WRITTEN_BEFORE_DL, "");
    }
}

}
