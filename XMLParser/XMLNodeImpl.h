/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#ifndef _XML_NODE_IMPL_H
#define _XML_NODE_IMPL_H

#include <string>
//#include <MSXML2_TLB.h>
#include "tinyxml.h"

class TiXmlNode;
namespace XML
{
class NodeImpl
{
public:
    explicit NodeImpl(TiXmlNode *xmlNode);
    ~NodeImpl();

public:
    std::string GetName();
    std::string GetText();
    void SetText(const std::string &text);
    void AppendText(const std::string &text);
    void PrependTextBeforeChild(const std::string &text,
                                unsigned int childIndex);

    unsigned int GetNumChildNodes();
    NodeImpl* GetChildNode(unsigned int index);
    NodeImpl* AppendChildNode(const std::string &name);

    NodeImpl* GetFirstChildNode() const;
    NodeImpl* GetNextSibling() const;

    void SetAttribute(const std::string &name,
                      const std::string &value);
    std::string GetAttribute(const std::string &name);

private:
    static NodeImpl *FindEffectiveNode(const TiXmlNode*);
	static const char *GetText(const TiXmlNode*);

private:
    TiXmlNode * m_XMLNode;
};
}

#endif // _XML_NODE_IMPL_H
