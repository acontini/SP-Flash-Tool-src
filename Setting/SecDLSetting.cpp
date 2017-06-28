#include "SecDLSetting.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Cmd/SecDLCommand.h"

namespace APCore
{

SecDLSetting::SecDLSetting():
    download_setting_(new APCore::DADownloadAllSetting()),
    format_setting_(new APCore::FormatSetting())
{
}

QSharedPointer <APCore::ICommand> SecDLSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::SecDLCommand> cmd(new APCore::SecDLCommand(key));

    format_setting_->set_validation(false);
    format_setting_->set_physical_fmt(false);
    format_setting_->set_auto_format(false);

    cmd->set_download_setting(download_setting_);
    cmd->set_format_setting(format_setting_);

    return cmd;
}

void SecDLSetting::set_storage_type(HW_StorageType_E type)
{
    storage_type_ = type;
    this->format_setting_->set_storage_type(type);
    this->download_setting_->set_storage_type(type);
}

void SecDLSetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);
}

void SecDLSetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);
}

}
