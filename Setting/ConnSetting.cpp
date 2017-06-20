#include "ConnSetting.h"
#include <iostream>
#include <QtGlobal>
#include "../Err/Exception.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

#include "ConnBromUARTSetting.h"
#include "ConnBromUSBSetting.h"


namespace APCore
{

ConnSetting::ConnSetting():
    stop_flag_(NULL),
    m_timeout(60*60*1000)
{
}

ConnSetting::~ConnSetting()
{

}

ConnSetting* ConnSetting::FromXML(const XML::Node &node)
{
    Q_ASSERT(node.GetName() == "connection");

    std::string type = node.GetAttribute("type");
    if(type == "BromUART")
    {
        return new ConnBromUARTSetting(node);
    }
    else if(type == "BromUSB")
    {
        return new ConnBromUSBSetting(node);
    }
    else
    {
        Q_ASSERT("Unsupported connection type.");
        return NULL;
    }
}

}
