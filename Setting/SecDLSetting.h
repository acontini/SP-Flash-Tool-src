#ifndef SECDLSETTING_H
#define SECDLSETTING_H

#include "ISetting.h"
#include "DADownloadAllSetting.h"
#include "FormatSetting.h"
#include "../Cmd/ICommand.h"
#include <QSharedPointer>

namespace APCore
{
class SecDLSetting:public ISetting
{
public:
    SecDLSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    void set_download_setting(QSharedPointer<APCore::DADownloadAllSetting> setting)
    {
        this->download_setting_ = setting;
    }

    void set_format_setting(QSharedPointer<APCore::FormatSetting> setting)
    {
        this->format_setting_ = setting;
    }

    virtual void set_storage_type(HW_StorageType_E type);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

private:
    QSharedPointer<APCore::DADownloadAllSetting> download_setting_;
    QSharedPointer<APCore::FormatSetting> format_setting_;

};
}

#endif // SECDLSETTING_H
