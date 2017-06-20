#ifndef DLPLUGFMTALLSETTING_H
#define DLPLUGFMTALLSETTING_H

#include "ISetting.h"
#include "ComboFormatSetting.h"
#include "DADownloadAllSetting.h"
#include "../Cmd/ICommand.h"

#include <QSharedPointer>

namespace APCore
{

class DLPlugFmtAllSetting : public ISetting
{
public:
    DLPlugFmtAllSetting();

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    void set_download_setting(QSharedPointer<APCore::DADownloadAllSetting> setting)
    {
        this->download_setting_ = setting;
    }

    void set_format_setting(QSharedPointer<APCore::ComboFormatSetting> setting)
    {
        this->format_setting_ = setting;
    }

    void set_combo_fmt_flag(bool isComboFmt)
    {
        this->format_setting_->set_is_combo_dl(isComboFmt);
    }

    virtual void set_storage_type(HW_StorageType_E type);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

private:
    QSharedPointer<APCore::DADownloadAllSetting> download_setting_;
    QSharedPointer<APCore::ComboFormatSetting> format_setting_;
};

}

#endif // DLPLUGFMTALLSETTING_H
