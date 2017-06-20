/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/
//#include <vcl.h>
//TODO: Qt ignoring this pragma.
//#pragma hdrstop

#include <cassert>
//TODO fix sstream compile bug
#ifdef _WIN32 
#undef max 
#undef min
#endif
#include <sstream>
#include <stdexcept>

#include "../XMLParser/XMLNodeImpl.h"
#include "../XMLParser/XMLDocumentImpl.h"
#include "../XMLParser/XMLNode.h"
#include "../Logger/Log.h"
#include "../Host/Inc/RuntimeMemory.h"

using XML::NodeImpl;
using XML::DocumentImpl;
//using Msxml2_tlb::TCOMIXMLDOMElement;
//using Msxml2_tlb::TCOMIXMLDOMParseError;
using std::endl;
//using std::runtime_error; using vcl Exception
using std::string;
using std::wstring;

/**
 * Take care of COM library initialization and deinitialization
 */
class COMInitializer : private NonCopyable
{
public:
    COMInitializer()
    {
/*        const HRESULT errorCode =
                ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

        if (FAILED(errorCode))
        {
            assert(0);
        }
        */
    }

    ~COMInitializer()
    {
//        ::CoUninitialize();
    }
};

static const COMInitializer g_COMInitializer;

DocumentImpl::DocumentImpl(const string &xmlFile)
    : m_MainDoc(NULL)
{
    InitDoc();
    Load(xmlFile);
}

DocumentImpl::DocumentImpl(const string &xmlVersion,
                           const string &encoding,
                           const string &rootNodeName)
    : m_MainDoc(NULL)
{
    InitDoc();
    m_DefaultXmlEncodingType = TIXML_ENCODING_UTF8;
    // TODO need delete this, seems auto delete in tinyxml.
    TiXmlDeclaration * decl = new TiXmlDeclaration(xmlVersion.c_str(), "UTF-8", "");
    m_MainDoc->LinkEndChild(decl);
    // TODO RootNode need add.
    CreateRootNode(rootNodeName);
    LOG("The encoding is %s.", &encoding);
}

DocumentImpl::~DocumentImpl()
{
	if (NULL != m_MainDoc)
		delete m_MainDoc;
}

void DocumentImpl::Load(const string &xmlFile)
{
    // Load File.
    if (m_MainDoc->LoadFile(xmlFile.c_str()) == false)
    {
        XMLEXCEPTION("Failed to load the XML document");
    }
}

void DocumentImpl::Save(const string &xmlFile)
{
    if (m_MainDoc->SaveFile(xmlFile.c_str()) == false)
    {
        XMLEXCEPTION("Failed to save the XML document");
    }
}

NodeImpl* DocumentImpl::GetRootNode()
{
    return new NodeImpl((TiXmlNode *)(m_MainDoc->RootElement()));
}

void DocumentImpl::InitDoc()
{
	m_MainDoc = new TiXmlDocument();
}

void DocumentImpl::Validate(const std::string &schemaFile)
{
	// FIXME Just delete here.
    LOG("The schemaFile is %s.", schemaFile.c_str());
}

void DocumentImpl::CreateProcessingInstruction(const string &target,
                                               const string &data)
{
// FIXME DO nothing here.
/*    Msxml2_tlb::TCOMIXMLDOMProcessingInstruction processingInstruction;

    HRESULT result = m_MainDoc->createProcessingInstruction(
                                        WideString(target.c_str()),
                                        WideString(data.c_str()),
                                        &processingInstruction);

    if (FAILED(result))
    {
        XMLEXCEPTION("Failed to create processing instruction");
    }

    if (FAILED(m_MainDoc->appendChild(processingInstruction, NULL)))
    {
        XMLEXCEPTION("Failed to add processing instruction");
    }
    */
    LOG("The target is %s, The data is %s. ", target.c_str(), data.c_str());
}

void DocumentImpl::CreateRootNode(const string &rootNodeName)
{
	TiXmlElement * rootElement = new TiXmlElement(rootNodeName.c_str());
	m_MainDoc->LinkEndChild(rootElement);
/*    TCOMIXMLDOMElement rootNode(
                m_MainDoc->createElement(WideString(rootNodeName.c_str())));

    if (FAILED(m_MainDoc->appendChild(rootNode, NULL)))
    {
        XMLEXCEPTION("Failed to create a root element");
    }
*/    
}

