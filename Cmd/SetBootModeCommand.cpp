#include "SetBootModeCommand.h"
#include "../Conn/Connection.h"
#include "../BootRom/flashtoolex_api.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"

namespace APCore
{

SetBootModeCommand::SetBootModeCommand(APKey key):
    ICommand(key),
    boot_mode_(0),
    com_type_(2),
    com_id_(0x01)
{
}

SetBootModeCommand::~SetBootModeCommand()
{
}

void SetBootModeCommand::exec(const QSharedPointer<APCore::Connection> &conn)
{
    conn->ConnectDA();

    LOGI("call flashtool_set_boot_flag with boot_mode: %d, com_type: %d, com_id: %d", boot_mode_, com_type_, com_id_);
    int ret = flashtool_set_boot_flag((HSESSION)conn->FTHandle(), boot_mode_, com_type_, com_id_);
    if(ret != STATUS_OK)
    {
        LOGE("flashtool_set_boot_flag() failed! ret: 0x%x", ret);
        THROW_BROM_EXCEPTION(ret, 0);
    }
    LOGI("flashtool_set_boot_flag() Succeeded.");
}


}
