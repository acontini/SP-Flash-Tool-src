#ifndef DADOWNLOADALL_H
#define DADOWNLOADALL_H

#include "ICommand.h"
#include "../Arg/BootRomArg.h"
#include "../Arg/BromDownloadArg.h"
#include <QTime>

namespace APCore
{

class DADownloadAll : public ICommand
{
public:
    DADownloadAll(APKey key);
    virtual ~DADownloadAll();

    virtual void exec(const QSharedPointer<Connection> &conn);

private:
    friend class DADownloadAllSetting;
    BromDownloadArg download_arg;
    QTime download_clock;
};

}

#endif // DADOWNLOADALL_H
