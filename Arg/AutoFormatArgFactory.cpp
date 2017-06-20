#include "AutoFormatArgFactory.h"
#include "../Logger/Log.h"
#include "../BootRom/flashtool.h"


namespace APCore
{
QSharedPointer<AutoFormatArg> AutoFormatArgFactory::CreateAutoFormatArg(const HW_StorageType_E& type,
                                                                        const APKey& key,
                                                                        const QSharedPointer<Connection> & conn)
{
    if(HW_STORAGE_NAND == type)
    {
        return QSharedPointer<APCore::AutoFormatArg>(new APCore::NANDFormatArg(GET_DL_HANDLE_T(key)));
    }
    else if(HW_STORAGE_EMMC == type)
    {   return QSharedPointer<APCore::AutoFormatArg>(new APCore::EMMCFormatArg(conn, true, id, false));    }
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
        return QSharedPointer<APCore::AutoFormatArg>(new APCore::UFSFormatArg(conn, true, id, false));
    }
    Q_ASSERT(0);
    return QSharedPointer<APCore::AutoFormatArg>();
}

void APCore::AutoFormatArgFactory::Dump(void)
{
        /*LOG("%s(): auto_fmt_flag(%d), da_report_->m_nand_flash_size(0x%x).",
                        __FUNCTION__, this->auto_fmt_flag_, this->da_report_->m_nand_flash_size);
    LOG("%s(): oper_flag_(%s), dl_handle_(0x%x), flashtool_handle_(0x%x)",
                        __FUNCTION__,
            AddrTypeFlagToString(this->oper_flag_),
                        this->dl_handle_,this->flashtool_handle_);*/
}

}
