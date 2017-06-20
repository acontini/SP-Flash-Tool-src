#ifndef NORAUTOFORMATARG_H
#define NORAUTOFORMATARG_H

#include "AutoFormatArg.h"

namespace APCore
{
class Connection;
class NORFormatArg : public AutoFormatArg
{
public:
    NORFormatArg();

    virtual void GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                                    const FormatSetting::AutoFormatFlag fmt_flag,
                                    bool does_physical_format,
                                    U64 &fmt_addr,
                                    U64 &fmt_len);

};
}
#endif // NORAUTOFORMATARG_H
