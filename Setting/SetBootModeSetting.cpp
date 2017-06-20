#include "SetBootModeSetting.h"
#include "../Cmd/SetBootModeCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

SetBootModeSetting::SetBootModeSetting()
{

}

QSharedPointer<APCore::ICommand> SetBootModeSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::SetBootModeCommand> cmd(new SetBootModeCommand(key));

    cmd->setBootMode(boot_mode_);
    cmd->setComType(com_type_);
    cmd->setComId(com_id_);

    return cmd;
}

void SetBootModeSetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);
}

void SetBootModeSetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);
}

}
