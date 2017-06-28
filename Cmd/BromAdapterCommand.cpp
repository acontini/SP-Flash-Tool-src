#include "BromAdapterCommand.h"
#include "../Err/Exception.h"
#include "../Conn/Connection.h"

#include "../Utility/Utils.h"

namespace APCore
{

BromAdapterCommand::BromAdapterCommand(APKey key):
    ICommand(key),
    key_(key),
    is_only_jump(false)
{
}

BromAdapterCommand::~BromAdapterCommand()
{

}

void BromAdapterCommand::update_brom_dl_para(const QSharedPointer<APCore::Connection> &conn)
{
    const BOOT_RESULT *p_result = FlashTool_GetBootResult(conn->FTHandle());

    BBCHIP_TYPE chip_type = p_result->m_bbchip_type;

    QString addr_str = Utils::GetAddrFromIniFile(chip_type);

    U64 addr = Utils::ConvertStrToAddr(addr_str);

    brom_dl_arg_.set_dl_begin_addr(addr);
    brom_dl_arg_.set_jump_addr(addr);
}

void BromAdapterCommand::exec(const QSharedPointer<APCore::Connection> &conn)
{
    conn->ConnectBROM();

    FlashTool_BROM_DL_Arg brom_dl_arg;
    FlashTool_Brom_DL_Result brom_dl_res;

    memset(&brom_dl_arg, 0, sizeof(FlashTool_BROM_DL_Arg));
    memset(&brom_dl_res, 0, sizeof(FlashTool_Brom_DL_Result));

    if(!is_manual_addr)
        update_brom_dl_para(conn);

    brom_dl_arg = brom_dl_arg_.GetBromAdapterDLArg();

    int ret;

    if(is_only_jump)
    {
         LOGD("Start BROM jump...");

         ret = Brom_JumpCmd_Ex(conn->FTHandle(), &brom_dl_arg, true);

         if(ret != S_DONE)
         {
             THROW_BROM_EXCEPTION(ret,0);
         }
    }
    else
    {

        LOGD("Start downloading via BROM...");

        ret = Brom_DownloadData_Ex(conn->FTHandle(), &brom_dl_arg, &brom_dl_res);

        LOGD("BROM Download result(%d)",ret);

        if(ret != S_DONE)
        {
            THROW_BROM_EXCEPTION(ret,0);
        }

        if(brom_dl_arg.is_jump)
        {
            ret = Brom_JumpCmd_Ex(conn->FTHandle(), &brom_dl_arg, true);
        }

        if(ret != S_DONE)
        {
            THROW_BROM_EXCEPTION(ret,0);
        }
    }
}

}
