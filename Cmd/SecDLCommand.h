#ifndef SECDLCOMMAND_H
#define SECDLCOMMAND_H

#include "ICommand.h"
#include "../Setting/DADownloadAllSetting.h"
#include "../Setting/FormatSetting.h"
#include "../Arg/BromDownloadArg.h"

#include <QSharedPointer>

namespace APCore
{
class SecDLCommand : public ICommand
{
public:
    SecDLCommand(APKey key);
    virtual ~SecDLCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_download_setting(
            const QSharedPointer<DADownloadAllSetting> &dl_setting)
    {
        this->dl_setting_ = dl_setting;
    }

    void set_format_setting(
            const QSharedPointer<FormatSetting> &format_setting)
    {
        this->format_setting_ = format_setting;
    }

private:
     const PART_INFO* get_user_data_partition(
                                  const QSharedPointer<Connection> &conn);

     void DoDownload(const QSharedPointer<Connection> &conn);
     void DoFormat(const QSharedPointer<Connection> &conn);

private:
    QSharedPointer<APCore::DADownloadAllSetting> dl_setting_;
    QSharedPointer<APCore::FormatSetting> format_setting_;
    APKey key_;
};

}

#endif // SECDLCOMMAND_H
