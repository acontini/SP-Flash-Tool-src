#ifndef SCIRESTORECOMMAND_H
#define SCIRESTORECOMMAND_H

#include "ICommand.h"
#include "./Setting/WriteMemorySetting.h"
#include "./Utility/CloneInfo.h"

namespace APCore
{
class SCIRestoreCommand : public ICommand
{
public:
    SCIRestoreCommand(APKey key);

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_restore_folder_path(const std::string &path){
        restore_folder_path_ = path;
    }

    void set_wm_setting(const QSharedPointer<APCore::WriteMemorySetting> &setting){
        wm_setting_ = setting;
    }

private:
    int getRestoreItem();
    void LoadFile(const std::string &filename);
    int DoRestore(const QSharedPointer<Connection> &conn);
    void DoCommand(WriteMemorySetting *setting, const QSharedPointer<Connection> &conn);

private:
    APKey key_;
    std::string restore_folder_path_;
    QSharedPointer<APCore::WriteMemorySetting> wm_setting_;
    std::list<CLONE_ITEM> cloneItem_list_;
};

}

#endif // SCIRESTORECOMMAND_H
