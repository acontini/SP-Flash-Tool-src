#include "CertDLSetting.h"
#include "../Cmd/CertDLCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

CertDLSetting::CertDLSetting()
{
}

QSharedPointer<APCore::ICommand> CertDLSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::CertDLCommand> cmd(new CertDLCommand(key));

    return cmd;
}

void CertDLSetting::LoadXML(const XML::Node &node)
{
    XML::Node child_node = node.GetFirstChildNode();

    while (!child_node.IsEmpty())
    {
        LOG("%0xI64x", &child_node);
        child_node = child_node.GetNextSibling();
    }
}

void CertDLSetting::SaveXML(XML::Node &node) const
{
     XML::Node root_node = node.AppendChildNode("cert-download");

     LOG("%0xI64x", &root_node);
}

}
