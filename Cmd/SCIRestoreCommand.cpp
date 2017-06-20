#include "SCIRestoreCommand.h"
#include "./Utility/FileUtils.h"
#include "./BootRom/mtk_status.h"
#include "./XMLParser/XMLDocument.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

namespace APCore
{
SCIRestoreCommand::SCIRestoreCommand(APKey key):
    ICommand(key),
    key_(key),
    wm_setting_(NULL)
{
}

int SCIRestoreCommand::getRestoreItem()
{
    if(!FileUtils::IsDirectoryExist(restore_folder_path_))
    {
        LOGE("Restore folder %s is not exist!", restore_folder_path_.c_str());
        return FT_FILE_FOLDER_IS_NOT_EXIST;
    }

    std::string config = restore_folder_path_ + C_SEP_STR + "clone_info.xml";
    if(!FileUtils::IsFileExist(config))
    {
        LOGE("Restore config file %s is not exist!", config.c_str());
        return FT_FILE_IS_NOT_EXIST;
    }

    LoadFile(config);

    return S_DONE;
}

void SCIRestoreCommand::LoadFile(const std::string &filename)
{
    XML::Document document(filename);

    const XML::Node rootNode = document.GetRootNode();
    Q_ASSERT(rootNode.GetName() == "clone_info");

    XML::Node child = rootNode.GetFirstChildNode();

    while(!child.IsEmpty())
    {
        CloneInfo *info = new CloneInfo();
        info->LoadXML(child);
        CLONE_ITEM item(info);
        cloneItem_list_.push_back(item);
        child = child.GetNextSibling();
    }
}

int SCIRestoreCommand::DoRestore(const QSharedPointer<Connection> &conn)
{
    APCore::WriteMemorySetting wm_setting(*wm_setting_);

    std::list<CLONE_ITEM>::const_iterator it = cloneItem_list_.begin();

    wm_setting.set_container_length(0);
    wm_setting.set_input_mode(InputMode_FromFile);
    wm_setting.set_addressing_mode(AddressingMode_LogicalAddress);

    while(it != cloneItem_list_.end())
    {
        if(!(*it)->verify(restore_folder_path_))
        {
            LOGW("check sum failed before restore partition %s, start address %08llx",
                 (*it)->partition_name().c_str(),
                 (*it)->start_addr());

            return FT_FLASHTOOL_ERROR_CHKSUM_FAIL;
        }

        wm_setting.set_part_id((*it)->part_id());
        wm_setting.set_address((*it)->start_addr());
        wm_setting.set_input_length((*it)->partition_len());
        wm_setting.set_input((*it)->image_path());
        wm_setting.set_program_mode(ProgramMode_PageOnly);

        DoCommand(&wm_setting, conn);

        ++ it;
    }

    return S_DONE;
}

void SCIRestoreCommand::DoCommand(WriteMemorySetting *setting,
                                  const QSharedPointer<Connection> &conn)
{
    setting->CreateCommand(key_)->exec(conn);
}

void SCIRestoreCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(S_DONE != getRestoreItem())
    {
        LOGI("Get restore item failed.");
        THROW_APP_EXCEPTION(FT_GET_RESTORE_ITEM_ERROR, "");
    }

    if(S_DONE != DoRestore(conn))
    {
        LOGI("Restore item failed.");
        THROW_APP_EXCEPTION(FT_RESTORE_ERROR, "");
    }

    LOGI("Restore Succeeded.");
}

}
