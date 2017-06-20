#ifndef SCIDOWNLOADSETTING_H
#define SCIDOWNLOADSETTING_H

#include "ISetting.h"
#include "FormatSetting.h"
#include "DADownloadAllSetting.h"
#include "ComboFormatSetting.h"

#include "../Logger/Log.h"
#include <QSharedPointer>

namespace APCore
{
class SCIDownloadSetting : public ISetting
{
public:
    SCIDownloadSetting();

    QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node)const;

    void set_format_setting(QSharedPointer<APCore::ComboFormatSetting> setting)
    {
        this->fmt_setting_ = setting;
    }

    void set_download_setting(QSharedPointer<APCore::DADownloadAllSetting> setting)
    {
        this->dl_setting_ = setting;
    }

    void set_combo_fmt_flag(bool isComboFmt)
    {
        this->fmt_setting_->set_is_combo_dl(isComboFmt);
    }

    virtual void set_storage_type(HW_StorageType_E type);

private:
    QSharedPointer<APCore::ComboFormatSetting> fmt_setting_;
    QSharedPointer<APCore::DADownloadAllSetting> dl_setting_;
};
}

#endif // SCIDOWNLOADSETTING_H
