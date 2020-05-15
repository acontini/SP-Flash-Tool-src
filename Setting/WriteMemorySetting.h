#ifndef WRITEMEMORYSETTING_H
#define WRITEMEMORYSETTING_H

#include "ISetting.h"
#include "../XMLParser/XMLNode.h"
#include "../BootRom/flashtool_api.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>

namespace APCore
{
class WriteMemorySetting :public ISetting
{
public:
    typedef void (__stdcall * void_callback)();
    WriteMemorySetting();

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    //setters
    void set_cb_write_memory_progress(const CALLBACK_WRITE_FLASH_PROGRESS cb)
    {
        cb_write_flash_progress = cb;
    }

     void set_cb_write_memory_init(void_callback cb)
    {
        cb_write_flash_init = cb;
    }

    void set_flash_type(const HW_StorageType_E storage_type)
    {
        this->storage_type_ = storage_type;
    }

    void set_addressing_mode(const AddressingMode addr_mode)
    {
        this->addr_mode_ = addr_mode;
    }

    void set_address(const U64 addr)
    {
        this->addr_ = addr;
    }

    void set_container_length(unsigned int container_len)
    {
        this->container_len_ = container_len;
    }

    void set_input_mode(const InputMode input_mode)
    {
        this->input_mode_ = input_mode;
    }

    void set_program_mode(const ProgramMode prog_mode)
    {
        this->prog_mode_ = prog_mode;
    }

    void set_input(const std::string &file_name)
    {
        this->input_file_ = file_name.c_str();
    }

    void set_input_length(U64 input_len)
    {
        this->input_len_ = input_len;
    }

    void set_part_id(U32 id)
    {
        this->part_id_ = id;
    }

private:
   CALLBACK_WRITE_FLASH_PROGRESS cb_write_flash_progress;
   void_callback cb_write_flash_init;
   U64 input_len_;
   unsigned int container_len_;
   U64 addr_;
   InputMode input_mode_;
   ProgramMode prog_mode_;
   AddressingMode addr_mode_;
   std::string input_file_;
   U32 part_id_;
};
}

#endif // WRITEMEMORYSETTING_H
