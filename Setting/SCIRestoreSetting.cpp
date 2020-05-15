#include "SCIRestoreSetting.h"
#include "./Cmd/SCIRestoreCommand.h"
namespace APCore
{

SCIRestoreSetting::SCIRestoreSetting()
{
}

QSharedPointer<APCore::ICommand> SCIRestoreSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::SCIRestoreCommand> cmd(new SCIRestoreCommand(key));

    cmd->set_restore_folder_path(restore_folder_path_);
    cmd->set_wm_setting(wm_setting_);

    return cmd;
}

void SCIRestoreSetting::set_storage_type(HW_StorageType_E type)
{
    storage_type_ = type;
    this->wm_setting_->set_storage_type(type);
}

void SCIRestoreSetting::LoadXML(const XML::Node &){

}

void SCIRestoreSetting::SaveXML(XML::Node &) const{

}

}
