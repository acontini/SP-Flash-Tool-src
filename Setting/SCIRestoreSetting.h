#ifndef SCIRESTORESETTING_H
#define SCIRESTORESETTING_H

#include <QSharedPointer>
#include "ISetting.h"
#include "WriteMemorySetting.h"

namespace APCore
{
class SCIRestoreSetting : public ISetting
{
public:
    SCIRestoreSetting();

    QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &);
    virtual void SaveXML(XML::Node &) const;

    void set_restore_folder_path(const std::string &path){
        restore_folder_path_ = path;
    }

    void set_wm_setting(QSharedPointer<APCore::WriteMemorySetting> setting){
        wm_setting_ = setting;
    }

    virtual void set_storage_type(HW_StorageType_E type);

private:
    QSharedPointer<APCore::WriteMemorySetting> wm_setting_;
    std::string restore_folder_path_;
};
}

#endif // SCIRESTORESETTING_H
