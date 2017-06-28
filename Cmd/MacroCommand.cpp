#include "MacroCommand.h"
#include "../Logger/Log.h"

namespace APCore
{

MacroCommand::MacroCommand(APKey key):
    ICommand(key)
{
}

MacroCommand::~MacroCommand()
{
    m_pszCommandList.clear();
}

void MacroCommand::vAddCommand(const QSharedPointer<ICommand>& cmd)
{
    this->m_pszCommandList.push_back(cmd);
}

void MacroCommand::vClearCommand()
{
    this->m_pszCommandList.clear();
}

void MacroCommand::exec(const QSharedPointer<Connection> &conn)
{
    std::list<QSharedPointer<APCore::ICommand> >::const_iterator it = m_pszCommandList.begin();
    int i = 0;
    for(;it != m_pszCommandList.end(); ++it)
    {
        LOGI("Command (%d)::Execute",i++);
        (*it)->exec(conn);
    }
}

}
