#ifndef SETBOOTMODECOMMAND_H
#define SETBOOTMODECOMMAND_H

#include "ICommand.h"

namespace APCore
{

class CheckRPMBCommand : public ICommand
{
public:
    CheckRPMBCommand(APKey key);
    virtual ~CheckRPMBCommand();

    virtual void exec(const QSharedPointer<APCore::Connection> &conn);
};

}

#endif // SETBOOTMODECOMMAND_H
