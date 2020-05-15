#include "DeviceTestSetting.h"
#include "../Cmd/DeviceTestCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

DeviceTestSetting::DeviceTestSetting()
    : battery_test_(false)
{
}

QSharedPointer<APCore::ICommand> DeviceTestSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::DeviceTestCommand> cmd(new APCore::DeviceTestCommand(key, this));

    return cmd;
}

void DeviceTestSetting::LoadXML(const XML::Node &node)
{
    //TODO
    Q_ASSERT("DeviceTestSetting::LoadXML was not implemented.");

    LOG("The node name is %s.", node.GetName().c_str());
}

void DeviceTestSetting::SaveXML(XML::Node &node) const
{
    //TODO
    Q_ASSERT("DeviceTestSetting::SaveXML was not implemented.");

    LOG("The node name is %s.", node.GetName().c_str());
}

}
