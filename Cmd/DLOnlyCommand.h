#ifndef SECDLCOMMAND_H
#define SECDLCOMMAND_H

#include "ICommand.h"
#include "../Setting/DADownloadAllSetting.h"
#include "../Arg/BromDownloadArg.h"

#include <QSharedPointer>

namespace APCore
{
class DLOnlyCommand : public ICommand
{
public:
    DLOnlyCommand(APKey key);
    virtual ~DLOnlyCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_download_setting(
            const QSharedPointer<DADownloadAllSetting> &dl_setting)
    {
        this->dl_setting_ = dl_setting;
    }

private:
    bool CheckPMTLayoutChange(const QSharedPointer<Connection> &conn);

private:
    QSharedPointer<APCore::DADownloadAllSetting> dl_setting_;
    APKey key_;
};
}

#endif // SECDLCOMMAND_H
