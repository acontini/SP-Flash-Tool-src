#ifndef BROMADAPTERARG_H
#define BROMADAPTERARG_H

#include "../BootRom/brom.h"

class BromAdapterArg
{
public:
    BromAdapterArg();
    explicit BromAdapterArg(const FlashTool_BROM_DL_Arg &dl_arg);

    FlashTool_BROM_DL_Arg GetBromAdapterDLArg(void) const {
        return this->brom_dl_arg_;
    }

    void set_cb_wbuf_init(CALLBACK_WRITE_BUF_PROGRESS_INIT cb)
    {
        brom_dl_arg_.m_cb_init = cb;
    }

    void set_cb_wbuf_progress(CALLBACK_WRITE_BUF_PROGRESS cb)
    {
        brom_dl_arg_.m_cb_progress = cb;
    }

    void set_dl_begin_addr(const U64 addr)
    {
        brom_dl_arg_.begin_addr = addr;
    }

    void set_jump_addr(const U64 addr)
    {
        brom_dl_arg_.jump_addr = addr;
    }

    void set_buf_len(const U64 len)
    {
        brom_dl_arg_.dl_data_len = len;
    }

    void set_dl_buf(const unsigned char *buf)
    {
        brom_dl_arg_.dl_data_buffer = (char*)buf;
    }

    void set_is_jump(bool jump)
    {
        brom_dl_arg_.is_jump = jump;
    }

    static int __stdcall BromDownload_Progress_Init(void* usr_arg);

    static int __stdcall BromDownload_Progress( unsigned char   finished_percentage,
                                                unsigned int    sent_bytes,
                                                unsigned int    total_bytes,
                                                void          *usr_arg);

private:
    void FillDefaultValue();

    FlashTool_BROM_DL_Arg brom_dl_arg_;
};

#endif // BROMADAPTERARG_H
