#include "ReadbackSetting.h"
#include "../Cmd/ReadbackCommand.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/RBHandle.h"
#include "../Logger/Log.h"
#include "../XMLParser/XMLNode.h"
#include "../Public/AppCore.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "ReadbackSettingAssist.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>

namespace APCore
{

ReadbackSetting::ReadbackSetting()
    : m_IsphysicalReadback(false),
      cb_readback_init_(NULL),
      cb_readback_progress_(NULL)
{
    storage_type_ = HW_STORAGE_NONE;
}

QSharedPointer<APCore::ICommand> ReadbackSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::ReadbackCommand> cmd(new APCore::ReadbackCommand(key));
    cmd->readback_arg().set_storage_type(storage_type_);

    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key);
    cmd->readback_arg().set_rb_handle(rb_handle->handle_t());

    if(cb_readback_init_ != NULL)
    {
        cmd->readback_arg().set_cb_readback_init(cb_readback_init_);
    }
    if(cb_readback_progress_ != NULL)
    {
        cmd->readback_arg().set_cb_readback_progress(cb_readback_progress_);
    }


    if(!items.empty())
    {
        for(std::list<ReadbackItem>::const_iterator it=items.begin();
            it!=items.end(); ++it)
        {
            rb_handle->AppendItem(*it);
        }
    }


    return cmd;
}

void ReadbackSetting::LoadXML(const XML::Node &node)
{
    LOG("The node name is %s.", node.GetName().c_str());

    XML::Node child = node.GetFirstChildNode();

    while(!child.IsEmpty())
    {
        const std::string name = child.GetName();

        if(name == "physical-readback")
        {
            std::string physical = child.GetAttribute("is-physical-readback");
            if(!physical.empty() && physical=="true")
            {
                m_IsphysicalReadback = true;
            }
            else
            {
                m_IsphysicalReadback = false;
            }
        }
        else if(name == "readback-list")
        {
            XML::Node rb_node = child.GetFirstChildNode();

            while (!rb_node.IsEmpty())
            {
                std::string _path = APCore::ReadbackSettingAssist::GenerateValidPath(rb_node.GetText());
                LOGD("readback file path: %s", _path.c_str());
                QString index = rb_node.GetAttribute("readback-index").c_str();
                int _index = index.toInt();
                bool _enable = rb_node.GetAttribute("readback-enable") == "true" ? true : false;

                NUTL_ReadFlag_E read_flag = NUTL_READ_PAGE_ONLY;
                std::string rdFlag = rb_node.GetAttribute("readback-flag");

                if(rdFlag == "NULL_READ_PAGE_SPARE")
                    read_flag = NUTL_READ_PAGE_SPARE;
                else if(rdFlag == "NUTL_READ_PAGE_ONLY")
                    read_flag = NUTL_READ_PAGE_ONLY;
                else if(rdFlag == "NUTL_READ_SPARE_ONLY")
                    read_flag = NUTL_READ_SPARE_ONLY;
                else if(rdFlag == "NUTL_READ_PAGE_WITH_ECC")
                    read_flag = NUTL_READ_PAGE_WITH_ECC;
                else if(rdFlag == "NUTL_VERIFY_AFTER_PROGRAM")
                    read_flag = NUTL_READ_PAGE_SPARE;
                else
                    Q_ASSERT(0 && "unknown read flag");

                U64 begin_addr;
                sscanf(rb_node.GetAttribute("start-address").c_str(),"%llX",&begin_addr);
                unsigned int len(0);
                sscanf(rb_node.GetAttribute("readback-length").c_str(), "%X", &len);

                NUTL_AddrTypeFlag_E addr_flag = NUTL_ADDR_LOGICAL;
                std::string ad_flag = rb_node.GetAttribute("addr-flag");
                if(ad_flag == "NUTL_ADDR_LOGICAL")
                    addr_flag = NUTL_ADDR_LOGICAL;
                else if(ad_flag == "NUTL_ADDR_PHYSICAL")
                    addr_flag = NUTL_ADDR_PHYSICAL;
                else
                    Q_ASSERT(0 && "unknown address flag");

//                U32 region_id = 0;
//                sscanf(rb_node.GetAttribute("part-id").c_str(), "%u", &region_id);
                QString part_id = rb_node.GetAttribute("part-id").c_str();
                U32 region_id = part_id.toInt();

                ReadbackItem item(_index, _enable, begin_addr, len, _path, read_flag, region_id, addr_flag);
                items.push_back(item);

                rb_node = rb_node.GetNextSibling();
            }
        }
        child = child.GetNextSibling();
    }
}

void ReadbackSetting::SaveXML(XML::Node &node) const
{
    LOG("The node name is %s.", node.GetName().c_str());
    XML::Node readback_node(node.AppendChildNode("readback"));
    XML::Node physical_node = readback_node.AppendChildNode("physical-readback");
    physical_node.SetAttribute("is-physical-readback", "false");

    if(!items.empty())
    {
        XML::Node readback_list_node = readback_node.AppendChildNode("readback-list");

        for(std::list<ReadbackItem>::const_iterator it = items.begin();
            it != items.end(); ++it)
        {
            XML::Node rb_node = readback_list_node.AppendChildNode("readback-rom-item",
                                                                   it->path());
            QString index = QString::number(it->index());
            rb_node.SetAttribute("readback-index", index.toStdString());
            rb_node.SetAttribute("readback-enable", it->enable()? "true" : "false");

            QString read_flag;

            switch(it->read_flag())
            {
            case NUTL_READ_PAGE_SPARE:
                read_flag = "NULL_READ_PAGE_SPARE";
                break;

            case NUTL_READ_PAGE_ONLY:
                read_flag = "NUTL_READ_PAGE_ONLY";
                break;

            case NUTL_READ_SPARE_ONLY:
                read_flag = "NUTL_READ_SPARE_ONLY";
                break;

            case NUTL_READ_PAGE_WITH_ECC:
                read_flag = "NUTL_READ_PAGE_WITH_ECC";
                break;

            case NUTL_VERIFY_AFTER_PROGRAM:
                read_flag = "NUTL_VERIFY_AFTER_PROGRAM";
                break;

            default:
                read_flag = "UNKNOWN";
            }

            rb_node.SetAttribute("readback-flag", read_flag.toStdString());

            std::ostringstream address;
            address<<std::hex<<"0x"<<std::setfill('0')<<std::setw(16)<<it->addr();
            rb_node.SetAttribute("start-address", address.str());

            std::ostringstream length;
            length<<std::hex<<"0x"<<std::setfill('0')<<std::setw(8)<<it->len();
            rb_node.SetAttribute("readback-length", length.str());

            QString addr_flag;

            switch(it->addr_flag())
            {
            case NUTL_ADDR_LOGICAL:
                addr_flag = "NUTL_ADDR_LOGICAL";
                break;

            case NUTL_ADDR_PHYSICAL:
                addr_flag = "NUTL_ADDR_PHYSICAL";
                break;

            default:
                addr_flag = "UNKNOWN";
                break;
            }

            rb_node.SetAttribute("addr-flag", addr_flag.toStdString());

            std::ostringstream region_str;
            region_str<<it->region();
            rb_node.SetAttribute("part-id", region_str.str());
        }
    }
}

}
