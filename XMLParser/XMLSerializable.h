/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#ifndef _XML_SERIALIZABLE_H
#define _XML_SERIALIZABLE_H


namespace XML
{
    class Node;

    class Serializable
    {
    protected:
        Serializable() {}
        virtual ~Serializable() {}

    public:
        virtual void LoadXML(const Node &node) = 0;
        virtual void SaveXML(Node &node) const = 0;
    };
}

#endif // _XML_SERIALIZABLE_H
