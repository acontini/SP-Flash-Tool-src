#include "CheckRPMBSetting.h"
#include "../Cmd/CheckRPMBCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

CheckRPMBSetting::CheckRPMBSetting()
{

}

QSharedPointer<APCore::ICommand> CheckRPMBSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::CheckRPMBCommand> cmd(new CheckRPMBCommand(key));
    return cmd;
}
void CheckRPMBSetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);
}

void CheckRPMBSetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);
}

}
