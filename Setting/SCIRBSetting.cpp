#include "SCIRBSetting.h"
#include "./Cmd/SCIRBCommand.h"

namespace APCore
{
SCIRBSetting::SCIRBSetting():
    readback_setting_(new APCore::ReadbackSetting())
{
}

QSharedPointer <APCore::ICommand> SCIRBSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::SCIRBCommand> cmd (new APCore::SCIRBCommand(key));

    cmd->set_readback_setting(readback_setting_);
    cmd->set_readback_folderPath(readback_folder_path_);

    return cmd;
}

void SCIRBSetting::set_storage_type(HW_StorageType_E type)
{
    storage_type_ = type;
    this->readback_setting_->set_storage_type(type);
}

void SCIRBSetting::LoadXML(const XML::Node &)
{

}

void SCIRBSetting::SaveXML(XML::Node &) const
{

}

}
