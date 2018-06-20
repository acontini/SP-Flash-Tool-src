#include "DLOnlyCommand.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/RBHandle.h"

#include "../Flow/storhelper.h"
#include "../Flow/dlproxy.h"
#include "../Flow/defaultrom.h"
#include "../Utility/constString.h"
#include "../Public/RomInfoFactory.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

namespace APCore
{

DLOnlyCommand::DLOnlyCommand(APKey key):
    ICommand(key),
    dl_setting_(NULL),
    key_(key)
{
}

DLOnlyCommand::~DLOnlyCommand()
{

}

bool DLOnlyCommand::CheckPMTLayoutChange(const QSharedPointer<Connection> &conn)
{
    bool ret = false;

    APCore::CoreHandle *core_handle = __POOL_OF_HANDLE(key_);
    if(core_handle == NULL)
    {
        THROW_APP_EXCEPTION(FT_INVALIDE_SESSION_KEY, "");
    }

    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key_);

    CStorHelper storage(conn.data());

    CDLProxy proxy(conn, rb_handle, storage);

    std::list<ROM_INFO> rom_list;

    RomInfoFactory info_factory(key_);
    info_factory.GetRomList(conn, rom_list);

    for(std::list<ROM_INFO>::const_iterator rom_it = rom_list.begin();
        rom_it != rom_list.end(); ++rom_it)
    {
        const PART_INFO *pmt =
                proxy.read_pmt(rom_it->name);

        if (pmt != NULL && pmt_is_changed(pmt, *rom_it))
        {

            if((strstr(rom_it->name, "BMTPOOL")||strstr("BMTPOOL", rom_it->name))
                    && ((rom_it->begin_addr&0xFFFF)== 0))
            {
                continue;
            }
            else
            {
                LOGD("PMT changed for <%s>: addr<0x%llx>-->addr<0x%llx>, len<0x%llx>-->len<0x%llx>",
                        rom_it->name, pmt->begin_addr, rom_it->begin_addr, pmt->image_length, rom_it->partition_size);
                ret = true;
                break;
            }
        }

    }

    return ret;
}

void DLOnlyCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(CheckPMTLayoutChange(conn) == true)
    {
        fw_throw_error(ERROR_ROM_MUST_ENABLE);
    }

    if(NULL != dl_setting_)
        dl_setting_->CreateCommand(conn->ap_key())->exec(conn);
}
}
