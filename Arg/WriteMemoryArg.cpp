#include "WriteMemoryArg.h"
#include "../Logger/Log.h"


int __stdcall WriteMemoryArg::cb_write_memory_init()
{
    LOGI("Write Memory Initial...");
    return 0;
}

int __stdcall WriteMemoryArg::cb_write_memory_progress(unsigned char percent,
                                                       U64  sent_bytes,
                                                       U64  total_bytes,
                                                       void*         usr_arg)
{
    static unsigned char lastReportedProgress = 0;
    if((percent != lastReportedProgress) && (percent % 10 ==0))
    {
        LOGI("%d%% of data write to memory, (%llu /%llu).", percent, sent_bytes, total_bytes);
        lastReportedProgress = percent;
    }

     LOG("%s", usr_arg);

    return 0;
}


WriteMemoryArg::WriteMemoryArg()
{
   FillDefaultValue();
}

void WriteMemoryArg::FillDefaultValue()
{
   wm_arg.m_flash_type = HW_STORAGE_NAND;
   wm_arg.m_addressing_mode = AddressingMode_LogicalAddress;
   wm_arg.m_address = 0;
   wm_arg.m_container_length = 0;
   wm_arg.m_input_mode = InputMode_FromFile;
   wm_arg.m_program_mode = ProgramMode_PageOnly;
   wm_arg.m_part_id = 0;
   wm_arg.m_input = NULL;
   wm_arg.m_input_length = 0;
   //cb_write_memory_init(this);
   wm_arg.m_cb_progress = cb_write_memory_progress;
   wm_arg.m_cb_progress_arg = this;
}

