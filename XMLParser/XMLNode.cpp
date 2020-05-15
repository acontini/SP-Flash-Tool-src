/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/
//TODO: Qt ignoring this pragma.
//#pragma hdrstop
//TODO fix sstream compile bug 
#ifdef _WIN32
#undef max 
#undef min
#endif
#include <sstream>
#include "../XMLParser/XMLNode.h"
#include "../XMLParser/XMLNodeImpl.h"

XML::XMLException::XMLException(const std::string& msg)throw(){
    mMsg = "[XML Exception]" + msg;

}

const char* XML::XMLException::what() const throw(){

    return mMsg.c_str();

}
XML::XMLException::~XMLException() throw(){
}

void XML::ThrowXMLException(const std::string& msg, const char* file, const int line){
	std::ostringstream oss;
	oss<<msg<<std::endl;
	oss<<"(file "<<file<<", line "<<line<<")";
	throw XML::XMLException(oss.str());
}


using XML::Node;
using XML::NodeImpl;

Node::Node(NodeImpl *impl)
    : m_Impl(impl)
{
}

Node::~Node()
{
}

std::string Node::GetName() const
{
    return m_Impl->GetName();
}

std::string Node::GetText() const
{
    return m_Impl->GetText();
}

void Node::SetText(const std::string &text)
{
    m_Impl->SetText(text);
}

void Node::AppendText(const std::string &text)
{
    m_Impl->AppendText(text);
}

void Node::PrependTextBeforeChild(const std::string &text,
                                  unsigned int childIndex)
{
    m_Impl->PrependTextBeforeChild(text, childIndex);
}

unsigned int Node::GetNumChildNodes() const
{
    return m_Impl->GetNumChildNodes();
}

Node Node::GetChildNode(unsigned int index) const
{
    return Node(m_Impl->GetChildNode(index));
}

Node Node::GetFirstChildNode() const
{
    return Node(m_Impl->GetFirstChildNode());
}

Node Node::GetNextSibling() const
{
    return Node(m_Impl->GetNextSibling());
}

Node Node::AppendChildNode(const std::string &name)
{
    return Node(m_Impl->AppendChildNode(name));
}

Node Node::AppendChildNode(const std::string &name,
                           const std::string &text)
{
    Node childNode(AppendChildNode(name));
    childNode.SetText(text);

    return childNode;
}

void Node::SetAttribute(const std::string &name,
                        const std::string &value)
{
    m_Impl->SetAttribute(name, value);
}

std::string Node::GetAttribute(const std::string &name) const
{
    return m_Impl->GetAttribute(name);
}
