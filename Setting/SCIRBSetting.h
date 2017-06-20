#ifndef CLONEDLSETTING_H
#define CLONEDLSETTING_H

#include <QSharedPointer>
#include "ISetting.h"
#include "ReadbackSetting.h"

namespace APCore
{
class SCIRBSetting : public ISetting
{
public:
    SCIRBSetting();

    QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    void set_rb_setting(QSharedPointer<APCore::ReadbackSetting> setting)
    {
        this->readback_setting_ = setting;
    }

    void set_rb_folderPath(const std::string &path){
        readback_folder_path_ = path;
    }

    virtual void set_storage_type(HW_StorageType_E type);
private:
    QSharedPointer<APCore::ReadbackSetting> readback_setting_;
    std::string readback_folder_path_;
};
}

#endif // CLONEDLSETTING_H
