/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#ifndef _XML_DOCUMENT_H
#define _XML_DOCUMENT_H

#include <memory>
#include <string>
#include "../ResourceManager/NonCopyable.h"
#include "../XMLParser/XMLNode.h"


namespace XML
{
    class DocumentImpl;

    class Document : private NonCopyable
    {
    public:
        explicit Document(const std::string &xmlFile);
        Document(const std::string &xmlVersion,
                 const std::string &encoding,
                 const std::string &rootNodeName);
        ~Document();

    public:
        void Load(const std::string &xmlFile);
        void Save(const std::string &xmlFile);
        void Validate(const std::string &schemaFile);
        Node GetRootNode();
        void Beautify();

    private:
        void DoBeautify(Node &node, unsigned int depth);

    private:
        std::auto_ptr<DocumentImpl> m_Impl;
    };
}

#endif // _XML_DOCUMENT_H
