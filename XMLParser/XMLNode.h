/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#ifndef _XML_NODE_H
#define _XML_NODE_H

#include <string>
#include <QSharedPointer>
//TODO add header file 
#include "../XMLParser/XMLNodeImpl.h"

namespace XML
{
    class NodeImpl;

    class Node
    {
    public:
        explicit Node(NodeImpl *impl);
        ~Node();

    public:
        std::string GetName() const;
        std::string GetText() const;
        void SetText(const std::string &text);
        void AppendText(const std::string &text);
        void PrependTextBeforeChild(const std::string &text,
                                    unsigned int childIndex);

        unsigned int GetNumChildNodes() const;

        Node AppendChildNode(const std::string &name);
        Node AppendChildNode(const std::string &name,
                             const std::string &text);

        Node GetFirstChildNode() const;
        Node GetNextSibling() const;

        void SetAttribute(const std::string &name,
                          const std::string &value);
        std::string GetAttribute(const std::string &name) const;

        bool IsEmpty() const { return m_Impl==NULL; }

    protected:
        // not suggested, very inefficient...
        Node GetChildNode(unsigned int index) const;

    private:
        QSharedPointer<NodeImpl> m_Impl;
    };

    void ThrowXMLException(const std::string& msg, const char* file, const int line);

    class XMLException : public std::exception{
    private:
        std::string mMsg;
    public:
        XMLException(const std::string& msg)throw();
        ~XMLException()throw();
        const char* what() const throw();
    };
}

#define XMLEXCEPTION(x) \
	do{ XML::ThrowXMLException(x, __FILE__, __LINE__); } while(0);

#endif // _XML_NODE_H
