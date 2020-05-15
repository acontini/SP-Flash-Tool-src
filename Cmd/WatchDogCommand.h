#ifndef WATCHDOGCOMMAND_H
#define WATCHDOGCOMMAND_H

#include "ICommand.h"
#include "../BootRom/flashtool_api.h"

namespace APCore
{

class WatchDogCommand : public ICommand
{
public:
    WatchDogCommand(APKey key);
    virtual ~WatchDogCommand();

    virtual void exec(const QSharedPointer<APCore::Connection> &conn);

private:
    void ArgFlashToolWatchDlg(FlashTool_EnableWDT_Arg *arg);
};

}

#endif // WATCHDOGCOMMAND_H
