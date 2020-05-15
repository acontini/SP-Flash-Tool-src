#ifndef WRITEMEMORYARG_H
#define WRITEMEMORYARG_H

#include "../BootRom/flashtool_api.h"
#include <string>

class WriteMemoryArg
{
public:
    WriteMemoryArg(void);
    WriteFlashMemoryParameter *raw_arg(){return &wm_arg;}

    //setters
    void set_cb_write_memory_progress(const CALLBACK_WRITE_FLASH_PROGRESS cb)
    {
        wm_arg.m_cb_progress = cb;
    }

    void set_flash_type(const HW_StorageType_E storage_type)
    {
        wm_arg.m_flash_type = storage_type;
    }

    void set_addressing_mode(const AddressingMode addr_mode)
    {
        wm_arg.m_addressing_mode = addr_mode;
    }

    void set_address(const U64 addr)
    {
        wm_arg.m_address = addr;
    }

    void set_container_length(unsigned int container_len)
    {
        wm_arg.m_container_length = container_len;
    }

    void set_input_mode(const InputMode input_mode)
    {
        wm_arg.m_input_mode = input_mode;
    }

    void set_program_mode(const ProgramMode prog_mode)
    {
        wm_arg.m_program_mode = prog_mode;
    }

    void set_input(const std::string& file_name)
    {
        wm_arg.m_input = file_name.c_str();
    }

    void set_input_length(U64 input_len)
    {
        wm_arg.m_input_length = input_len;
    }

    void set_part_id(unsigned int part_id)
    {
        wm_arg.m_part_id = part_id;
    }

    static int __stdcall cb_write_memory_init();
    static int __stdcall cb_write_memory_progress(unsigned char percent,
                                                  U64  sent_bytes,
                                                  U64  total_bytes,
                                                  void*         usr_arg);
private:
    void FillDefaultValue(void);
    WriteFlashMemoryParameter wm_arg;
};

#endif // WRITEMEMORYARG_H
