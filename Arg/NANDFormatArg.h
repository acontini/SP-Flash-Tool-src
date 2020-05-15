#ifndef NANDFORMATARG_H
#define NANDFORMATARG_H

#include "AutoFormatArg.h"

namespace APCore
{
class Connection;
class NANDFormatArg:public AutoFormatArg
{
public:
    NANDFormatArg(DL_HANDLE_T dl_handle);

    virtual void GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                                    const FormatSetting::AutoFormatFlag fmt_flag,
                                    bool does_physical_format,
                                    U64 &fmt_addr,
                                    U64 &fmt_len);

private:	
    U64 format_start_address_;
    U64 format_length_;
    DL_HANDLE_T dl_handle_;
};

}


#endif // NANDFORMATARG_H
