#ifndef SCIDOWNLOADCOMMAND_H
#define SCIDOWNLOADCOMMAND_H

#include "ICommand.h"
#include "../Setting/ISetting.h"
#include "../Arg/BromDownloadArg.h"
#include "../Arg/BromFormatArg.h"
#include "../Setting/FormatSetting.h"
#include "./Setting/ComboFormatSetting.h"
#include "../Setting/DADownloadAllSetting.h"

#include <QSharedPointer>

namespace APCore
{
class Connection;

class SCIDownloadCommand : public ICommand
{
public:
    SCIDownloadCommand(APKey key);
    virtual ~SCIDownloadCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_format_setting(
            QSharedPointer<ComboFormatSetting> &format_setting)
    {
        this->fmt_setting_ = format_setting;
    }

    void set_download_setting(
            const QSharedPointer<DADownloadAllSetting> &dl_setting)
    {
        this->dl_setting_ = dl_setting;
    }

private:
    void DoDownload(const QSharedPointer<Connection> &conn);
    void DoFormat(const QSharedPointer<Connection> &conn);

private:
    QSharedPointer<APCore::ComboFormatSetting> fmt_setting_;
    QSharedPointer<APCore::DADownloadAllSetting> dl_setting_;
};
}

#endif // SCIDOWNLOADCOMMAND_H
