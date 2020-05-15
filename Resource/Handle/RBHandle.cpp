#include "RBHandle.h"

#include "../../Logger/Log.h"
#include "../../Err/Exception.h"

#include <QtGlobal>

namespace APCore
{

RBHandle::RBHandle()
{
    int ret = RB_Create(&handle_t_);
    LOG("RB_Create(%d)", ret);
}

RBHandle::~RBHandle()
{
    LOGD("dtor of RBHandle");
    int ret = RB_ClearAll(handle_t_);
    LOGD("RB_ClearAll(%d)", ret);
    ret = RB_Destroy(&handle_t_);
    LOGD("RB_Destroy(%d)", ret);
}

void RBHandle::ClearAll(void)
{
    RB_ClearAll(handle_t_);
}

void RBHandle::AppendItem(const ReadbackItem &item)
{
    int ret = S_DONE;
    ret = RB_Append(handle_t_, item.path().c_str(),
                    item.addr(), item.len());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetEnableAttr(handle_t_, item.index(),
                           item.enable()?_TRUE:_FALSE);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackFlag(handle_t_, item.index(),
                             item.read_flag());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackAddr(handle_t_, item.index(),
                             item.addr());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackLength(handle_t_, item.index(),
                               item.len());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetAddrTypeFlag(handle_t_, item.index(),
                             item.addr_flag());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackPartition(handle_t_, item.index(),
                                  item.region());

    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }

    GetCount();
}

unsigned int RBHandle::GetCount() const
{
    int ret = S_DONE;
    unsigned short count = 0;

    ret = RB_GetCount(handle_t_, &count);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
    LOG("RB_GetCount(%d)",count);
    return count;
}

void RBHandle::DeleteItem(unsigned int index)
{
    Q_ASSERT(index < GetCount());

    int ret = RB_Delete(handle_t_, index);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void RBHandle::EnableItem(unsigned int index, bool does_enable)
{
    int ret = RB_SetEnableAttr(handle_t_, index, does_enable?_TRUE:_FALSE);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void RBHandle::UpdateItem(const ReadbackItem& item)
{
    int ret = S_DONE;

    ret = RB_SetFilepath(handle_t_, item.index(),
                         item.path().c_str());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetEnableAttr(handle_t_, item.index(),
                           item.enable()?_TRUE:_FALSE);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackFlag(handle_t_, item.index(),
                             item.read_flag());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackAddr(handle_t_, item.index(),
                             item.addr());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackLength(handle_t_, item.index(),
                               item.len());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetAddrTypeFlag(handle_t_, item.index(),
                             item.addr_flag());
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = RB_SetReadbackPartition(handle_t_, item.index(),
                                  item.region());

    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

ReadbackItem RBHandle::GetItem(unsigned int index)
{
    Q_ASSERT(index < GetCount());

    RB_INFO info;
    memset(&info, 0, sizeof(RB_INFO));
    int ret = RB_GetInfo(handle_t_, index, &info);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    return ReadbackItem(info.index,
                        info.enable==_TRUE?true:false,
                        info.readback_addr,
                        info.readback_len,
                        info.filepath,
                        info.m_read_flag,
                        info.part_id
                        );
}


void RBHandle::SetReadbackFlag(unsigned int index, NUTL_ReadFlag_E flag)
{
    Q_ASSERT(index < GetCount());

    int ret = RB_SetReadbackFlag(handle_t_, index, flag);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void RBHandle::SetReadbackAddr(unsigned int index, U64 addr)
{
    Q_ASSERT(index < GetCount());

    int ret = RB_SetReadbackAddr(handle_t_, index, addr);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void RBHandle::SetReadbackLen(unsigned int index, U64 len)
{
    Q_ASSERT(index < GetCount());

    int ret = RB_SetReadbackLength(handle_t_, index, len);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void RBHandle::SetAddrTypeFlag(unsigned int index, NUTL_AddrTypeFlag_E flag)
{
    Q_ASSERT(index < GetCount());

    int ret = RB_SetAddrTypeFlag(handle_t_, index, flag);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void RBHandle::SetReadbackPartid(unsigned int index, U32 id)
{
    Q_ASSERT(index < GetCount());

    int ret = RB_SetReadbackPartition(handle_t_, index, id);
    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

}
