#ifndef MACROCOMMAND_H
#define MACROCOMMAND_H
#include "ICommand.h"
#include <list>
namespace APCore
{

class MacroCommand : public ICommand
{
public:
    MacroCommand(APKey key);
    virtual ~MacroCommand();
    virtual void exec(const QSharedPointer<Connection> &conn);
    void vAddCommand(const QSharedPointer<APCore::ICommand>& cmd);
    void vClearCommand();
private:

    std::list<QSharedPointer<APCore::ICommand> > m_pszCommandList;
};
}
#endif // MACROCOMMAND_H
