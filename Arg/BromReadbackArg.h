#ifndef BROMREADBACKARG_H
#define BROMREADBACKARG_H

#include "../BootRom/flashtool_api.h"

class BromReadbackArg
{
public:
    BromReadbackArg();

    FlashTool_Readback_Arg *raw_arg() { return &rb_arg;}

    void set_cb_readback_init(CALLBACK_READBACK_PROGRESS_INIT cb)
    {
        rb_arg.m_cb_readback_flash_init = cb;
    }

    void set_cb_readback_progress(CALLBACK_READBACK_PROGRESS cb)
    {
        rb_arg.m_cb_readback_flash = cb;
    }

    void set_rb_handle(RB_HANDLE_T handle)
    {
        rb_arg.m_rb_handle = handle;
    }

    void set_storage_type(HW_StorageType_E type)
    {
        rb_arg.m_storage_type = type;
    }

    static int __stdcall cb_readback_init(HW_StorageType_E storage_type,
                                          U64 rb_addr,
                                          U64 rb_len,
                                          const char *rb_file,
                                          void *usr_arg);

    static int __stdcall cb_readback_progress(unsigned char finished_percent,
                                              U64 finished_bytes,
                                              U64 total_bytes,
                                              void *usr_arg);

private:
    void FillDefaultVaule();

    FlashTool_Readback_Arg rb_arg;
};

#endif // BROMREADBACKARG_H
