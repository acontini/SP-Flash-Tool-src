/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/
//TODO: Qt ignoring the pragma.
//#pragma hdrstop

#include <cassert>
#include <stdexcept>

#include "../XMLParser/XMLNodeImpl.h"
#include "../XMLParser/XMLNode.h"
#include "../XMLParser/tinyxml.h"
#include "../Host/Inc/RuntimeMemory.h"

using XML::NodeImpl;
//using std::runtime_error;

//
// NodeImpl
//
NodeImpl::NodeImpl(TiXmlNode *xmlNode)
    : m_XMLNode(xmlNode)
{
}

NodeImpl::~NodeImpl()
{
}

std::string NodeImpl::GetName()
{
// FIXME need verify
//[zhili.chen][3.16][Add]: Define "TIXML_USE_STL",compiled error.[begin]
#ifdef TIXML_USE_STL
    return m_XMLNode->ValueStr();
#else
    return m_XMLNode->Value();
#endif
//[zhili.chen][3.16][Add]: Define "TIXML_USE_STL",compiled error.[end]
}

const char *NodeImpl::GetText(const TiXmlNode *p)
{
	p = p->FirstChild();

	while (p != NULL)
	{
		if (p->Type() != TiXmlNode::TINYXML_COMMENT)
		{
			const TiXmlText* text = p->ToText();
			return (NULL!=text)?text->Value():NULL;
		}
		p = p->NextSibling();
	}

	return NULL;
}

std::string NodeImpl::GetText()
{
    const char* str;
    switch (m_XMLNode->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        str = GetText(m_XMLNode);
        return (str == 0) ? "" : str;

    case TiXmlNode::TINYXML_TEXT:
        //[zhili.chen][3.16][modify]: Define "TIXML_USE_STL",compiled error.[begin]
        //return m_XMLNode->ValueStr();
#ifdef TIXML_USE_STL
        return m_XMLNode->ValueStr();
#else
        return m_XMLNode->Value();
#endif
        //[zhili.chen][3.16][modify]: Define "TIXML_USE_STL",compiled error.[end]
        // FIXME ignore others
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_TYPECOUNT:
    case TiXmlNode::TINYXML_COMMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
        return "";
    }
    //for compile warning.
    return "";
}

void NodeImpl::SetText(const std::string &text)
{
    TiXmlNode * firstChild;
    switch (m_XMLNode->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        firstChild = m_XMLNode->FirstChild();
        if(firstChild != NULL)
        {
            TiXmlText * xmlText = firstChild->ToText();
            if(xmlText != NULL)
            {
                m_XMLNode->ReplaceChild(xmlText, TiXmlText(text.c_str()));
            }
            return;
        }
        m_XMLNode->LinkEndChild(new TiXmlText(text.c_str()));
        break;

    case TiXmlNode::TINYXML_TEXT:
        m_XMLNode->SetValue(text.c_str());
        break;

        // FIXME ignore others
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_TYPECOUNT:
    case TiXmlNode::TINYXML_COMMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
        break;
    }
    return;
}

// FIXME do not use in linux version.
void NodeImpl::AppendText(const std::string &text)
{
    switch (m_XMLNode->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        m_XMLNode->LinkEndChild(new TiXmlText(text.c_str()));
        break;

    case TiXmlNode::TINYXML_TEXT:
        m_XMLNode->SetValue(text.c_str());
        break;
        // FIXME ignore others
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_TYPECOUNT:
    case TiXmlNode::TINYXML_COMMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
        break;
    }
    return;
}

// FIXME do not use in linux version
void NodeImpl::PrependTextBeforeChild(const std::string &text,
                                      unsigned int childIndex)
{
    assert(childIndex < GetNumChildNodes());

    unsigned int i = 0;
    TiXmlNode *node;
    switch (m_XMLNode->Type())
    {

    case TiXmlNode::TINYXML_ELEMENT:
        m_XMLNode->SetValue(text.c_str());
        for(node = m_XMLNode->FirstChild() ; node != 0 ; i++)
        {
            if(i == childIndex)
                break;
            node = node->NextSibling();
        }
        m_XMLNode->InsertBeforeChild(node, TiXmlText(text.c_str()));
        break;
        // FIXME ignore TEXT
    case TiXmlNode::TINYXML_TEXT:

        // FIXME ignore others
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_TYPECOUNT:
    case TiXmlNode::TINYXML_COMMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
        break;
    }
    return;
}

/*=== very inefficient ===*/
unsigned int NodeImpl::GetNumChildNodes()
{
    unsigned int i = 0;
    TiXmlNode *node;
    for(node = m_XMLNode->FirstChild() ; node != 0 ; node = node->NextSibling())
    {
        i++;
    }
    return i;
}

/*=== very inefficient ===*/
/*=== including comment nodes ===*/
NodeImpl* NodeImpl::GetChildNode(unsigned int index)
{
    assert(index < GetNumChildNodes());
    unsigned int i = 0;
    TiXmlNode *node;
    for(node = m_XMLNode->FirstChild();
        node != 0 ; node = node->NextSibling())
    {
        if(i == index)
        {
            return new NodeImpl(node);
        }
        i++;
    }

    return NULL;
}

// find the nearest node which is not comment
NodeImpl *NodeImpl::FindEffectiveNode(const TiXmlNode *p)
{
    while (p != NULL)
    {
        if (p->Type() != TiXmlNode::TINYXML_COMMENT)
        {
            return new NodeImpl((TiXmlNode*)p);
        }
        p = p->NextSibling();
    }
    return NULL;
}

NodeImpl* NodeImpl::GetFirstChildNode() const
{
    return FindEffectiveNode(m_XMLNode->FirstChild());
}

NodeImpl* NodeImpl::GetNextSibling() const
{
    return FindEffectiveNode(m_XMLNode->NextSibling());
}


NodeImpl* NodeImpl::AppendChildNode(const std::string &name)
{
    TiXmlElement * childNode = new TiXmlElement(name.c_str());
    m_XMLNode->LinkEndChild(childNode);
    return new NodeImpl(childNode);
}

void NodeImpl::SetAttribute(const std::string &name,
                            const std::string &value)
{
    TiXmlElement * element;
    switch(m_XMLNode->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        element = (TiXmlElement*) m_XMLNode;
        element->SetAttribute(name.c_str(),value.c_str());
        break;
        // FIXME ignore others
    case TiXmlNode::TINYXML_TEXT:
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_TYPECOUNT:
    case TiXmlNode::TINYXML_COMMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
        XMLEXCEPTION("Failed to add attribute to the node");
        break;
    }
}

std::string NodeImpl::GetAttribute(const std::string &name)
{
    const char *value = NULL;
    TiXmlElement * element;
    switch(m_XMLNode->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        element = (TiXmlElement*) m_XMLNode;
        //TODO check it out
        value = (element->Attribute(name.c_str()));
        break;
        // FIXME ignore others
    case TiXmlNode::TINYXML_TEXT:
    case TiXmlNode::TINYXML_DOCUMENT:
    case TiXmlNode::TINYXML_DECLARATION:
    case TiXmlNode::TINYXML_TYPECOUNT:
    case TiXmlNode::TINYXML_COMMENT:
    case TiXmlNode::TINYXML_UNKNOWN:
        XMLEXCEPTION("Failed to get attribute from the node");
        break;
    }
    return (value!=NULL) ? value : "";
}

