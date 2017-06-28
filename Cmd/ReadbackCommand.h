#ifndef READBACKCOMMAND_H
#define READBACKCOMMAND_H

#include "ICommand.h"
#include "../Arg/BromReadbackArg.h"
#include "../Conn/Connection.h"
#include "../Public/AppTypes.h"

namespace APCore
{

class ReadbackCommand : public ICommand
{
public:
    ReadbackCommand(APKey key);
    virtual ~ReadbackCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    BromReadbackArg& readback_arg(){ return readback_arg_;}

private:
    friend class ReadbackSetting;
    BromReadbackArg readback_arg_;
};

}

#endif // READBACKCOMMAND_H
