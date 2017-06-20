#ifndef DLONLYSETTING_H
#define DLONLYSETTING_H

#include "ISetting.h"
#include "DADownloadAllSetting.h"
#include "../Cmd/DLOnlyCommand.h"
#include <QSharedPointer>

namespace APCore
{
class DLOnlySetting:public ISetting
{
public:
    DLOnlySetting();

    QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    void set_download_setting(QSharedPointer<APCore::DADownloadAllSetting> setting)
    {
        this->download_setting_ = setting;
    }

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

private:
    QSharedPointer<APCore::DADownloadAllSetting> download_setting_;
};
}

#endif // DLONLYSETTING_H
