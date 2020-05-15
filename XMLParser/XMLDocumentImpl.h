/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#ifndef _XML_DOCUMENT_IMPL_H
#define _XML_DOCUMENT_IMPL_H

#include <string>
//#include <MSXML2_TLB.h>
#include "../XMLParser/tinyxml.h"
#include "../ResourceManager/NonCopyable.h"


namespace XML
{
    class NodeImpl;

    class DocumentImpl : private NonCopyable
    {
    public:
        explicit DocumentImpl(const std::string &xmlFile);
        DocumentImpl(const std::string &xmlVersion,
                     const std::string &encoding,
                     const std::string &rootNodeName);
        ~DocumentImpl();

    public:
        void Load(const std::string &xmlFile);
        void Save(const std::string &xmlFile);
        void Validate(const std::string &schemaFile);
        NodeImpl* GetRootNode();

    private:
        void InitDoc();
        void CreateProcessingInstruction(const std::string &target,
                                         const std::string &data);
        void CreateRootNode(const std::string &rootNodeName);
// TODO 
//        std::string FormatParseError(
//                const Msxml2_tlb::TCOMIXMLDOMParseError &parseError);

    private:
// TODO 
//        Msxml2_tlb::TCOMIXMLDOMDocument2 m_MainDoc;
		TiXmlDocument *m_MainDoc;
		TiXmlEncoding m_DefaultXmlEncodingType;


    };
}

#endif // _XML_DOCUMENT_IMPL_H
