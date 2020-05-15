#include "FirmwareUpgradeSetting.h"

#include "../Utility/FileUtils.h"
#include "../Err/ErrorLookup.h"
#include "../Err/Exception.h"
#include "../Logger/Log.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{
FirmwareUpgradeSetting::FirmwareUpgradeSetting():
    format_setting_(new APCore::FormatSetting()),
    download_setting_(new APCore::DADownloadAllSetting()),
    readback_setting_(new APCore::ReadbackSetting()),
    wm_setting_(new APCore::WriteMemorySetting()),
    scene_(DOWNLOAD_ONLY)
{
}

QSharedPointer <APCore::ICommand> FirmwareUpgradeSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::FirmwareUpgradeCommand> cmd(new APCore::FirmwareUpgradeCommand(key));
    ConfigOperationByScene(cmd, scene_);

    return cmd;
}

void FirmwareUpgradeSetting::ConfigOperationByScene(
        QSharedPointer<APCore::FirmwareUpgradeCommand> cmd,
        Download_Scene &scene)
{
    format_setting_->set_validation(false);
    format_setting_->set_physical_fmt(false);
    format_setting_->set_auto_format(false);

    if(scene == FIRMWARE_UPGRADE)
    {
        cmd->set_format_setting(format_setting_);
        cmd->set_readback_setting(readback_setting_);
        cmd->set_download_setting(download_setting_);
        cmd->set_writememory_setting(wm_setting_);
    }
    else
    {
        if(scene == FORMAT_ALL_DOWNLOAD)
        {
             format_setting_->set_auto_format(true);
             format_setting_->set_physical_fmt(true);
             format_setting_->set_auto_format_flag(FormatSetting::Format_ALL);
             format_setting_->set_erase_flag(NUTL_ERASE);
        }

        cmd->set_format_setting(format_setting_);
        cmd->set_download_setting(download_setting_);
    }
    cmd->set_download_scene(scene);
}

void FirmwareUpgradeSetting::set_storage_type(HW_StorageType_E type)
{
    storage_type_ = type;
    this->readback_setting_->set_storage_type(type);
    this->format_setting_->set_storage_type(type);
    this->download_setting_->set_storage_type(type);
    this->wm_setting_->set_storage_type(type);


}

void FirmwareUpgradeSetting::LoadXML(const XML::Node &node)
{
    XML::Node child_node = node.GetFirstChildNode();

    while (!child_node.IsEmpty())
    {
        if(child_node.GetName() == "readback")
        {
            this->readback_setting_->set_storage_type(storage_type_);
            this->readback_setting_->LoadXML(child_node);
        }
        else if(child_node.GetName()  == "format")
        {
            this->format_setting_->set_storage_type(storage_type_);
            this->format_setting_->LoadXML(child_node);
        }
        else if(child_node.GetName()  == "da-download-all")
        {
            this->download_setting_->set_storage_type(storage_type_);
            this->download_setting_->LoadXML(child_node);
        }
        else if(child_node.GetName() == "write-memory")
        {
            this->wm_setting_->set_storage_type(storage_type_);
            this->wm_setting_->LoadXML(child_node);
        }
        else if(child_node.GetName() == "scene")
        {
            if(child_node.GetText() == "FORMAT_ALL_DOWNLOAD")
            {
                scene_ = FORMAT_ALL_DOWNLOAD;
            }
            else if(child_node.GetText() == "FIRMWARE_UPGRADE")
            {
                scene_ = FIRMWARE_UPGRADE;
            }
            else if(child_node.GetText() == "DOWNLOAD_ONLY")
            {
                scene_ = DOWNLOAD_ONLY;
            }
        }
        child_node = child_node.GetNextSibling();
    }
}
void FirmwareUpgradeSetting::SaveXML(XML::Node &node) const
{
    XML::Node root_node(node.AppendChildNode("firmware-upgrade"));

    if(scene_ == FORMAT_ALL_DOWNLOAD)
    {
        root_node.AppendChildNode("scene", "FORMAT_ALL_DOWNLOAD");
    }
    if(scene_ == FIRMWARE_UPGRADE)
    {
        root_node.AppendChildNode("scene", "FIRMWARE_UPGRADE");
    }
    if(scene_ == DOWNLOAD_ONLY)
    {
        root_node.AppendChildNode("scene", "DOWNLOAD_ONLY");
    }

    this->readback_setting_->SaveXML(root_node);
    this->format_setting_->SaveXML(root_node);
    this->download_setting_->SaveXML(root_node);
    this->wm_setting_->SaveXML(root_node);
}
}
