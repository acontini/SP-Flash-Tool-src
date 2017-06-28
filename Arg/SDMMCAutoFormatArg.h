#ifndef SDMMCAUTOFORMATARG_H
#define SDMMCAUTOFORMATARG_H

#include "AutoFormatArg.h"

namespace APCore
{
class Connection;

class SDMMCAutoFormatArg : public AutoFormatArg
{
public:
    SDMMCAutoFormatArg(const QSharedPointer<Connection> &conn);

    virtual void GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                                    const FormatSetting::AutoFormatFlag fmt_flag,
                                    bool does_physical_format,
                                    U64 &fmt_addr,
                                    U64 &fmt_len);

private:
    U64 format_all_length_;
    void CalcFormatAllLength(const DA_REPORT_T* da_report);
};

}

#endif // SDMMCAUTOFORMATARG_H
