#ifndef RBHANDLE_H
#define RBHANDLE_H

#include "../../BootRom/flashtool_handle.h"
#include "../../Public/AppTypes.h"
#include "../../Rules/ReadbackRule.h"

namespace APCore
{

class RBHandle
{
public:
    RBHandle();
    ~RBHandle();

    void AppendItem(const ReadbackItem &item);
    void DeleteItem(unsigned int index);
    void EnableItem(unsigned int index, bool does_enable);
    void UpdateItem(const ReadbackItem &item);
    ReadbackItem GetItem(unsigned int index);

    void SetReadbackFlag(unsigned int index, NUTL_ReadFlag_E flag);
    void SetReadbackAddr(unsigned int index, U64 addr);
    void SetReadbackLen(unsigned int index, U64 len);
    void SetAddrTypeFlag(unsigned int index, NUTL_AddrTypeFlag_E flag);
    void SetReadbackPartid(unsigned int index, U32  id);

    void ClearAll(void);

    unsigned int GetCount() const;

    RB_HANDLE_T handle_t() const{ return handle_t_;}

private:
    RB_HANDLE_T handle_t_;
};

}

#endif // RBHANDLE_H
