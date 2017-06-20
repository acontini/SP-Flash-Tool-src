#include "FormatSetting.h"
#include "../Cmd/FormatCommand.h"
#include "../XMLParser/XMLNode.h"
#include "ConnSetting.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Setting/PlatformSetting.h"
#include <QtGlobal>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>

namespace APCore
{

FormatSetting::FormatSetting():
    cb_fmt_init(NULL),
    cb_fmt_progress(NULL),
    cb_fmt_statistics(NULL),
    does_validation(false),
    does_physical_fmt(true),
    is_normal_format_(true),
    erase_flag(NUTL_ERASE),
    does_auto_format(false),
    begin_addr(0),
    length(0),
    auto_format_flag(Format_FlagEnd)
{
    storage_type_ = HW_STORAGE_NAND;
}

QSharedPointer<APCore::ICommand> FormatSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::FormatCommand> cmd(new APCore::FormatCommand(key));
    if(cb_fmt_init != NULL)
        cmd->format_arg.set_cb_format_report_init(cb_fmt_init);
    if(cb_fmt_progress != NULL)
        cmd->format_arg.set_cb_format_report(cb_fmt_progress);
    if(cb_fmt_statistics != NULL)
        cmd->format_arg.set_cb_format_statistics(cb_fmt_statistics);

    cmd->format_arg.set_addr_type(
                does_physical_fmt?NUTL_ADDR_PHYSICAL:NUTL_ADDR_LOGICAL);
    cmd->format_arg.set_validation(does_validation?_TRUE:_FALSE);
    cmd->format_arg.set_storage_type(storage_type_);
    cmd->storage_type = storage_type_;
    cmd->format_arg.set_m_erase_flag(erase_flag);
    cmd->set_partition_id(part_id);
    cmd->set_is_normal_fmt(is_normal_format_);
    cmd->does_auto_fmt = does_auto_format;
    cmd->set_does_physical_format(does_physical_fmt);

    if(does_auto_format)
    {
        //auto format is actual not "auto format".
        cmd->format_arg.set_auto_format(_FALSE);
        cmd->format_arg.set_format_begin_addr(0);
        cmd->format_arg.set_format_length(0);
        cmd->auto_fmt_flag = auto_format_flag;
    }
    else
    {
        cmd->format_arg.set_auto_format(_FALSE);
        cmd->format_arg.set_format_begin_addr(begin_addr);
        cmd->format_arg.set_format_length(length);
        cmd->format_arg.set_partition_id(part_id);
    }

    return cmd;
}

void FormatSetting::LoadXML(const XML::Node &node)
{
    std::string validation = node.GetAttribute("validation");
    if(!validation.empty() && validation=="true")
    {
        does_validation = true;
    }
    else
    {
        does_validation = false;
    }

    std::string physical = node.GetAttribute("physical");
    if(!physical.empty() && physical=="true")
    {
        does_physical_fmt = true;
    }
    else
    {
        does_physical_fmt = false;
    }

    std::string erase = node.GetAttribute("erase-flag");
    if(!erase.empty())
    {
        if(erase == "NormalErase")
        {
            erase_flag = NUTL_ERASE;
        }
        else if(erase == "ForceErase")
        {
            erase_flag = NUTL_FORCE_ERASE;
        }
        else if(erase == "MarkBadBlock")
        {
            erase_flag = NUTL_MARK_BAD_BLOCK;
        }
        else
        {
            Q_ASSERT(0 && "invalid erase flag.");
        }
    }

    std::string auto_fmt = node.GetAttribute("auto-format");
    if(!auto_fmt.empty() && auto_fmt=="true")
    {
        does_auto_format = true;
    }
    else
    {
        does_auto_format = false;
    }

    if(does_auto_format)
    {
        std::string auto_flag = node.GetAttribute("auto-format-flag");
        if(!auto_flag.empty())
        {
            if(auto_flag == "FormatAll")
            {
                auto_format_flag = Format_ALL;
            }
            else if(auto_flag == "FormatNVRAM")
            {
                auto_format_flag = Format_NVRAM;
            }
            else if(auto_flag == "FormatExcept_BL")
            {
                auto_format_flag = Format_Except_BL;
            }
            else
            {
                Q_ASSERT(0 && "invalid auto format flag.");
            }
        }
    }

    XML::Node child_node = node.GetFirstChildNode();

    while (!child_node.IsEmpty())
    {
        if(child_node.GetName() == "begin-addr")
        {
            sscanf(child_node.GetText().c_str(),"%llx",&begin_addr);
        }
        else if(child_node.GetName() == "length")
        {
            sscanf(child_node.GetText().c_str(),"%llx",&length);
        }
        else if(child_node.GetName() == "part-id")
        {
            sscanf(child_node.GetText().c_str(), "%d", (int*)&part_id);
        }
        else if(child_node.GetName() == "connection")
        {
            this->conn_setting = APCore::ConnSetting::FromXML(child_node);
        }
        else
        {
            Q_ASSERT(0 && "unknown format setting.");
        }

        child_node = child_node.GetNextSibling();
    }
}

void FormatSetting::SaveXML(XML::Node &node) const
{
    XML::Node root_node(node.AppendChildNode("format"));

    root_node.SetAttribute("validation",does_validation?"true":"false");
    root_node.SetAttribute("physical",does_physical_fmt?"true":"false");

    std::string erase_flag_str;
    switch(erase_flag)
    {
    case NUTL_ERASE:
        erase_flag_str = "NormalErase";
        break;
    case NUTL_FORCE_ERASE:
        erase_flag_str = "ForceErase";
        break;
    case NUTL_MARK_BAD_BLOCK:
        erase_flag_str = "MarkBadBlock";
        break;
    default:
        Q_ASSERT(0);
    }
    root_node.SetAttribute("erase-flag",erase_flag_str);

    root_node.SetAttribute("auto-format", does_auto_format?"true":"false");
    if(does_auto_format)
    {
        std::string auto_flag_str;
        switch(auto_format_flag)
        {
        case Format_ALL:
            auto_flag_str = "FormatAll";
            break;
        case Format_NVRAM:
            auto_flag_str = "FormatNVRAM";
            break;
        case Format_Except_BL:
            auto_flag_str = "FormatExcept_BL";
            break;
        default:
            Q_ASSERT(0);
        }
        root_node.SetAttribute("auto-format-flag",auto_flag_str);
    }
    else
    {
        std::ostringstream address;
        address<<std::hex<<"0x"<<std::setfill('0')<<std::setw(16)<<begin_addr;
        root_node.AppendChildNode("begin-addr",address.str());

        std::ostringstream fmt_length;
        fmt_length<<std::hex<<"0x"<<std::setfill('0')<<std::setw(16)<<length;
        root_node.AppendChildNode("length",fmt_length.str());

        std::ostringstream region_str;
        region_str<<part_id;
        root_node.AppendChildNode("part-id", region_str.str());
    }

//    if(conn_setting != NULL)
//    {
//        conn_setting->SaveXML(root_node);
//    }
}

void FormatSetting::update_format_type_by_region_id(U32 part_id)
{

    if(HW_STORAGE_EMMC == storage_type_)
    {
        if((EMMC_Part_E)part_id > EMMC_PART_UNKNOWN && (EMMC_Part_E)part_id < EMMC_PART_END)
            is_normal_format_ = false;
    }
    else if(HW_STORAGE_UFS == storage_type_)
    {
        if((UFS_Part_E)part_id > UFS_PART_UNKNOWN && (UFS_Part_E)part_id < UFS_PART_END)
            is_normal_format_ = false;
    }

}
}
