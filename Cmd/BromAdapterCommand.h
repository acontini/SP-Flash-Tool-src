#ifndef BROMADAPTERCOMMAND_H
#define BROMADAPTERCOMMAND_H

#include "ICommand.h"
#include "../Arg/BromAdapterArg.h"
#include "../Setting/BromAdapterSetting.h"

namespace APCore
{

class BromAdapterCommand : public ICommand
{
public:
    BromAdapterCommand(APKey key);

    virtual ~BromAdapterCommand();

    virtual void exec(const QSharedPointer<APCore::Connection> &conn);

    void set_is_only_jump(bool only_jump)
    {
        is_only_jump = only_jump;
    }

    void set_is_manual_addr(bool manual_addr)
    {
        is_manual_addr = manual_addr;
    }

private:
    void update_brom_dl_para(const QSharedPointer<APCore::Connection> &conn);

private:
    friend class BromAdapterSetting;
    APKey key_;
    BromAdapterArg brom_dl_arg_;
    bool is_only_jump;
    bool is_manual_addr;
};

}

#endif // BROMADAPTERCOMMAND_H
