#ifndef BROMADAPTERSETTING_H
#define BROMADAPTERSETTING_H

#include "ISetting.h"

namespace APCore
{

class BromAdapterSetting : public ISetting
{
public:
    BromAdapterSetting();
    explicit BromAdapterSetting(BromAdapterSetting* bromAdapter_setting);

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    void set_cb_wbuf_init(CALLBACK_WRITE_BUF_PROGRESS_INIT cb)
    {
        cb_wbuf_init = cb;
    }

    void set_cb_wbuf_progress(CALLBACK_WRITE_BUF_PROGRESS cb)
    {
        cb_wbuf_progress = cb;
    }

    void set_start_addr(U64 addr)
    {
        start_address = addr;
    }

    void set_jump_addr(U64 addr)
    {
        jump_address = addr;
    }

    void set_buf_len(U64 len)
    {
        buf_len = len;
    }

    void set_buf(unsigned char *buffer)
    {
        buf = buffer;
    }

    void set_isjump(bool jump)
    {
        is_jump = jump;
    }

    void set_is_only_jump(bool only_jump)
    {
        is_only_jump = only_jump;
    }

    void set_is_manual_addr(bool manual_addr)
    {
        is_manual_addr = manual_addr;
    }

private:
    CALLBACK_WRITE_BUF_PROGRESS_INIT cb_wbuf_init;
    CALLBACK_WRITE_BUF_PROGRESS      cb_wbuf_progress;

    U64 start_address;
    U64 jump_address;
    U64 buf_len;
    unsigned char *buf;
    bool is_jump;
    bool is_only_jump;
    bool is_manual_addr;
};

}

#endif // BROMADAPTERSETTING_H
