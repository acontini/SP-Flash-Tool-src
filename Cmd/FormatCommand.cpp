#include "FormatCommand.h"
#include "../BootRom/flashtool.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/DAHandle.h"
#include "../Conn/Connection.h"
#include "../Flow/storhelper.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../Utility/SizeCalcHelper.h"
#include "../Logger/Log.h"
#include "../Host/Inc/RuntimeMemory.h"

APCore::FormatCommand::FormatCommand(APKey key)
    : ICommand(key), does_auto_fmt(false),
      auto_fmt_flag(FormatSetting::Format_FlagEnd),
      is_normal_fmt_(true)
{
}


APCore::FormatCommand::~FormatCommand()
{
}


void APCore::FormatCommand::exec(const QSharedPointer<Connection> & conn)
{
    conn->ConnectDA();

    if(does_auto_fmt)
    {
        U64 fmt_addr = 0;
        U64 fmt_len = 0;

        QSharedPointer<APCore::AutoFormatArg> auto_fmt_ptr = CreateAutoFormatArg(this->storage_type, conn);

        auto_fmt_ptr.data()->GenAutoFormatRange(conn,this->auto_fmt_flag, this->does_physical_format, fmt_addr, fmt_len);

        format_arg.set_format_begin_addr(fmt_addr);
        format_arg.set_format_length(fmt_len);
    }

    FlashTool_Format_Arg       fmt_arg;
    FlashTool_Format_Result    fmt_res;

    fmt_arg = format_arg.GetBromFormatArg();

    if(!does_auto_fmt && !ValidateFormatParameter(conn, fmt_arg))
    {
        LOGI("Format failed: invalid format parameter.");
        THROW_APP_EXCEPTION(FT_INVALID_FORMAT_ADDR_ERROR, "");
    }

    memset( &fmt_res, 0, sizeof(fmt_res));

    int ret = FlashTool_Format(
                conn->FTHandle(),
                &fmt_arg,
                &fmt_res);
    LOG("Format result(%d)",ret);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    LOGI("Format Succeeded.");
}

bool APCore::FormatCommand::ValidateFormatParameter(
    const QSharedPointer<Connection> &conn, const FlashTool_Format_Arg &fmt_arg)
{
    const FORMAT_CONFIG_T &cfg = fmt_arg.m_format_cfg;

    SizeCalcHelper helper(conn, storage_type, FORMAT_ONLY, cfg.m_AddrType);

    U64 totalSize = helper.size();

    return (totalSize >= cfg.m_format_begin_addr + cfg.m_format_length);
}

QSharedPointer<APCore::AutoFormatArg> APCore::FormatCommand::CreateAutoFormatArg(
    const HW_StorageType_E type, const QSharedPointer<Connection> & conn)
{
    if(HW_STORAGE_NAND == type)
    {
        return QSharedPointer<APCore::AutoFormatArg>(new APCore::NANDFormatArg(GET_DL_HANDLE_T(key_)));
    }
    else if(HW_STORAGE_EMMC == type)
    {
        EMMC_Part_E id  = static_cast<EMMC_Part_E> (part_id);
        if(id >= EMMC_PART_UNKNOWN && id < EMMC_PART_END && !is_normal_fmt_)
            return QSharedPointer<APCore::AutoFormatArg>(new APCore::EMMCFormatArg(conn, true, id, false));

        return QSharedPointer<APCore::AutoFormatArg>(new APCore::EMMCFormatArg(conn));
    }
    else if(HW_STORAGE_SDMMC == type)
    {
        return QSharedPointer<APCore::AutoFormatArg>(new APCore::SDMMCAutoFormatArg(conn));
    }
    else if (HW_STORAGE_NOR == type)
    {
        return QSharedPointer<APCore::AutoFormatArg>(new APCore::NORFormatArg());
    }
    else if (HW_STORAGE_UFS == type)
    {
        UFS_Part_E id = static_cast<UFS_Part_E>(part_id);
        if(id >= UFS_PART_UNKNOWN && id < UFS_PART_END && !is_normal_fmt_)//combo format
            return QSharedPointer<APCore::AutoFormatArg>(new APCore::UFSFormatArg(conn, true, id, false));

        return QSharedPointer<APCore::AutoFormatArg>(new APCore::UFSFormatArg(conn));
    }
    Q_ASSERT(0);
    return QSharedPointer<APCore::AutoFormatArg>();
}

