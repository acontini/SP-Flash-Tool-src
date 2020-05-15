#include "DLOnlySetting.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{
DLOnlySetting::DLOnlySetting():
    download_setting_(new APCore::DADownloadAllSetting())
{
}

QSharedPointer <APCore::ICommand> DLOnlySetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::DLOnlyCommand> cmd(new APCore::DLOnlyCommand(key));

    cmd->set_download_setting(download_setting_);

    return cmd;
}

void DLOnlySetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);

    XML::Node child_node = node.GetFirstChildNode();

    while(!child_node.IsEmpty())
    {
        if(child_node.GetName()  == "da-download-all")
        {
            this->download_setting_->set_storage_type(storage_type_);
            this->download_setting_->LoadXML(child_node);
        }

        child_node = child_node.GetNextSibling();
    }
}

void DLOnlySetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);

    XML::Node root_node(node.AppendChildNode("download-only"));

    this->download_setting_->SaveXML(root_node);
}
}
