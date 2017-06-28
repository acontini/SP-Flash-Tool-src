#include "WriteMemorySetting.h"

#include "../Cmd/WriteMemoryCommand.h"
#include "../Resource/CoreHandle.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore{

WriteMemorySetting::WriteMemorySetting():
    cb_write_flash_progress(NULL),
    cb_write_flash_init(NULL),
    input_len_(0),
    container_len_(0),
    addr_(0),
    input_mode_(InputMode_FromFile),
    prog_mode_(ProgramMode_PageOnly),
    addr_mode_(AddressingMode_LogicalAddress),
    part_id_(0)
{
}

QSharedPointer<APCore::ICommand> WriteMemorySetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::WriteMemoryCommand> cmd(new APCore::WriteMemoryCommand(key));
    cmd->wm_arg_.set_address(addr_);
    cmd->wm_arg_.set_addressing_mode(addr_mode_);
    cmd->wm_arg_.set_container_length(container_len_);
    cmd->wm_arg_.set_flash_type(storage_type_);
    cmd->wm_arg_.set_input(input_file_);
    cmd->wm_arg_.set_input_length(input_len_);
    cmd->wm_arg_.set_input_mode(input_mode_);
    cmd->wm_arg_.set_program_mode(prog_mode_);
    cmd->wm_arg_.set_part_id(part_id_);
    if(cb_write_flash_init != NULL)
    {
       cmd->set_write_memory_init(cb_write_flash_init);
    }
    if(cb_write_flash_progress != NULL)
    {
       cmd->wm_arg_.set_cb_write_memory_progress(cb_write_flash_progress);
    }

    return cmd;
}

void WriteMemorySetting::LoadXML(const XML::Node &node)
{
    LOG("The node name is %s.", node.GetName().c_str());
    XML::Node wm_node = node.GetFirstChildNode();

    if(!wm_node.IsEmpty())
    {
        const std::string name = wm_node.GetName();
        if(name == "write-memory-item")
        {
            input_file_ = wm_node.GetText();

            std::string input_flag = wm_node.GetAttribute("input-mode");
            if(input_flag == "FromBuffer")
                input_mode_ = InputMode_FromBuffer;
            else if(input_flag == "FromFile")
                input_mode_ = InputMode_FromFile;
            else
                Q_ASSERT(0 && "unknown input mode");

            std::string prog_flag = wm_node.GetAttribute("program-mode");
            if(prog_flag == "PageOnly")
                prog_mode_ = ProgramMode_PageOnly;
            else if(prog_flag == "PageSpare")
                prog_mode_ = ProgramMode_PageSpare;
            else
                Q_ASSERT(0 && "unknown program mode");

            std::string addr_flag = wm_node.GetAttribute("addr-mode");
            if(addr_flag == "BlockIndex")
                addr_mode_ = AddressingMode_BlockIndex;
            else if(addr_flag == "PhysicalAddress")
                addr_mode_ = AddressingMode_PhysicalAddress;
            else if(addr_flag == "LogicalAddress")
                addr_mode_ = AddressingMode_LogicalAddress;
            else
                Q_ASSERT(0 && "unknown addressing mode");

            sscanf(wm_node.GetAttribute("address").c_str(),"%llX",&addr_);
            sscanf(wm_node.GetAttribute("input-length").c_str(), "%llX", &input_len_);

            QString region_id = wm_node.GetAttribute("part-id").c_str();
            part_id_ = region_id.toInt();
        }
    }
}

void WriteMemorySetting::SaveXML(XML::Node &node) const
{
    LOG("The node name is %s.", node.GetName().c_str());

    XML::Node parent_node(node.AppendChildNode("write-memory"));

    XML::Node wm_node(parent_node.AppendChildNode("write-memory-item",input_file_));
    QString input_flag;
    switch(input_mode_)
    {
    case InputMode_FromBuffer:
        input_flag = "FromBuffer";
        break;

    case InputMode_FromFile:
        input_flag = "FromFile";
        break;

    default:
        input_flag = "UNKNOWN";
    }
    wm_node.SetAttribute("input-mode", input_flag.toStdString());

    QString prog_flag;
    switch(prog_mode_)
    {
    case ProgramMode_PageOnly:
        prog_flag = "PageOnly";
        break;

    case ProgramMode_PageSpare:
        prog_flag = "PageSpare";
        break;

    default:
        prog_flag = "UNKNOWN";
    }
    wm_node.SetAttribute("program-mode", prog_flag.toStdString());

    QString addr_flag;
    switch(addr_mode_)
    {
    case AddressingMode_BlockIndex:
        addr_flag = "BlockIndex";
        break;

    case AddressingMode_PhysicalAddress:
        addr_flag = "PhysicalAddress";
        break;

    case AddressingMode_LogicalAddress:
        addr_flag = "LogicalAddress";
        break;

    default:
        input_flag = "UNKNOWN";
    }
    wm_node.SetAttribute("addr-mode", addr_flag.toStdString());

    std::ostringstream address;
    address<<std::hex<<"0x"<<std::setfill('0')<<std::setw(16)<<addr_;
    wm_node.SetAttribute("address", address.str());

    std::ostringstream length;
    length<<std::hex<<"0x"<<std::setfill('0')<<std::setw(8)<<input_len_;
    wm_node.SetAttribute("input-length", length.str());

    std::ostringstream region_str;
    region_str<<part_id_;
    wm_node.SetAttribute("part-id", region_str.str());
}

}
