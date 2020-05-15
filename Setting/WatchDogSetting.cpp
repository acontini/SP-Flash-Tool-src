#include "WatchDogSetting.h"
#include "../Cmd/WatchDogCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

WatchDogSetting::WatchDogSetting()
{

}

QSharedPointer<APCore::ICommand> WatchDogSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::WatchDogCommand> cmd(new WatchDogCommand(key));

    return cmd;
}

void WatchDogSetting::LoadXML(const XML::Node &node)
{
    LOG("%0xI64x", &node);
}

void WatchDogSetting::SaveXML(XML::Node &node) const
{
    LOG("%0xI64x", &node);
}

}
