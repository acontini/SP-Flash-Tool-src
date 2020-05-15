#include "DLPlugFmtAllSetting.h"
#include "../XMLParser/XMLNode.h"
#include "../Cmd/DLPlugFmtAllCommand.h"

namespace APCore
{

DLPlugFmtAllSetting::DLPlugFmtAllSetting():
    download_setting_(new APCore::DADownloadAllSetting()),
    format_setting_(new APCore::ComboFormatSetting())
{
}

QSharedPointer <APCore::ICommand> DLPlugFmtAllSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::DLPlugFmtAllCommand> cmd(new APCore::DLPlugFmtAllCommand(key));

    format_setting_->format_setting()->set_auto_format_flag(FormatSetting::Format_ALL);

    cmd->set_download_setting(download_setting_);
    cmd->set_combofmt_setting(format_setting_);

    return cmd;
}

void DLPlugFmtAllSetting::set_storage_type(HW_StorageType_E type)
{
    storage_type_ = type;
    this->format_setting_->set_storage_type(type);
    this->download_setting_->set_storage_type(type);

}

void DLPlugFmtAllSetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);
    XML::Node child_node = node.GetFirstChildNode();

    while(!child_node.IsEmpty())
    {
        if(child_node.GetName() == "combo-format")
        {
            this->format_setting_->set_storage_type(storage_type_);
            this->format_setting_->LoadXML(child_node);
        }
        else if(child_node.GetName() == "da-download-all")
        {
            this->download_setting_->set_storage_type(storage_type_);
            this->download_setting_->LoadXML(child_node);
        }

        child_node = child_node.GetNextSibling();
    }
}

void DLPlugFmtAllSetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);
    XML::Node  root_node(node.AppendChildNode("format-download"));

    this->format_setting_->SaveXML(root_node);
    this->download_setting_->SaveXML(root_node);
}

}
