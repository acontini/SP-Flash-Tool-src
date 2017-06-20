#include "SCIDownloadSetting.h"
#include "../Err/Exception.h"
#include "../Cmd/SCIDownloadCommand.h"

namespace APCore
{
SCIDownloadSetting::SCIDownloadSetting():
    fmt_setting_(new APCore::ComboFormatSetting()),
    dl_setting_(new APCore::DADownloadAllSetting())
{
}

QSharedPointer<APCore::ICommand> SCIDownloadSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::SCIDownloadCommand> cmd(new APCore::SCIDownloadCommand(key));

    fmt_setting_->format_setting()->set_auto_format_flag(FormatSetting::Format_ALL);
    fmt_setting_->set_is_combo_dl(true);
   /* fmt_setting_->set_auto_format(true);
    fmt_setting_->set_physical_fmt(true);
    fmt_setting_->set_auto_format_flag(FormatSetting::Format_ALL);
    fmt_setting_->set_erase_flag(NUTL_ERASE);
    fmt_setting_->set_validation(false);
*/
    cmd->set_format_setting(fmt_setting_);
    cmd->set_download_setting(dl_setting_);

    return cmd;
}

void SCIDownloadSetting::set_storage_type(HW_StorageType_E type)
{
    storage_type_ = type;
    this->fmt_setting_->set_storage_type(type);
    this->dl_setting_->set_storage_type(type);

}

void SCIDownloadSetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);
}

void SCIDownloadSetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);
}
}
