#ifndef DLPLUGFMTALLCOMMAND_H
#define DLPLUGFMTALLCOMMAND_H

#include "ICommand.h"
#include "../Setting/DADownloadAllSetting.h"
#include "../Setting/ComboFormatSetting.h"

#include <QSharedPointer>

namespace APCore
{

class DLPlugFmtAllCommand : public ICommand
{
public:
    DLPlugFmtAllCommand(APKey key);
    virtual ~DLPlugFmtAllCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_download_setting(
            const QSharedPointer<DADownloadAllSetting> &dl_setting)
    {
        this->dl_setting_ = dl_setting;
    }

    void set_combofmt_setting(
            const QSharedPointer<ComboFormatSetting> &combo_fmt_setting)
    {
        this->format_setting_ = combo_fmt_setting;
    }

private:
     void DoDownload(const QSharedPointer<Connection> &conn);
     void DoFormat(const QSharedPointer<Connection> &conn);

private:
    QSharedPointer<APCore::DADownloadAllSetting> dl_setting_;
    QSharedPointer<APCore::ComboFormatSetting> format_setting_;
};

}

#endif // DLPLUGFMTALLCOMMAND_H
