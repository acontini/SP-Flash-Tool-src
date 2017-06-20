/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#include "../XMLParser/XMLDocument.h"
#include "../XMLParser/XMLDocumentImpl.h"
#include "../Host/Inc/RuntimeMemory.h"

using XML::Document;
using XML::DocumentImpl;
using XML::Node;

Document::Document(const std::string &xmlFile)
    : m_Impl(new DocumentImpl(xmlFile))
{
}

Document::Document(const std::string &xmlVersion,
                   const std::string &encoding,
                   const std::string &rootNodeName)
    : m_Impl(new DocumentImpl(xmlVersion, encoding, rootNodeName))
{
}

Document::~Document()
{
}

void Document::Load(const std::string &xmlFile)
{
    m_Impl->Load(xmlFile);
}

void Document::Save(const std::string &xmlFile)
{
    m_Impl->Save(xmlFile);
}

void Document::Validate(const std::string &schemaFile)
{
    m_Impl->Validate(schemaFile);
}

Node Document::GetRootNode()
{
    return Node(m_Impl->GetRootNode());
}

// FIXME No need Beautify in linux version
void Document::Beautify()
{
//    Node rootNode(GetRootNode());
//    DoBeautify(rootNode, 1);
}

static void AppendTabs(std::string &str, unsigned int numTabs)
{
    for (unsigned int i=0; i<numTabs; ++i)
    {
        str += "\t";
    }
}

void Document::DoBeautify(Node &node, unsigned int depth)
{
    unsigned int numChildNodes = node.GetNumChildNodes();

    if (numChildNodes == 0)
    {
        return;
    }

    std::string spaces("\n");
    AppendTabs(spaces, depth);

    for (unsigned int i=0; i<numChildNodes; ++i)
    {
        const unsigned int childIndex = i * 1 ;
        node.PrependTextBeforeChild(spaces, childIndex);
    }

    Node childNode = node.GetFirstChildNode();

    while (!childNode.IsEmpty())
    {
        DoBeautify(childNode, depth+1);
        childNode = childNode.GetNextSibling();
    }

    spaces = "\n";
    AppendTabs(spaces, depth-1);

    node.AppendText(spaces);
}
