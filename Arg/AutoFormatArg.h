#ifndef AUTOFORMATARG_H
#define AUTOFORMATARG_H

#include "../Setting/FormatSetting.h"

namespace APCore
{
typedef struct
{
    U64 fmt_begin;
    U64 fmt_length;
    U32 part_id;
    bool is_bl_range;
}com_fmt_range;

class Connection;
class AutoFormatArg
{
public:
    AutoFormatArg();
    AutoFormatArg(const AutoFormatArg& ref);
    AutoFormatArg& operator = (const AutoFormatArg& ref);
    virtual ~AutoFormatArg();

    virtual void GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                                    const FormatSetting::AutoFormatFlag fmt_flag,
                                    bool does_physical_format,
                                    U64 &fmt_addr,
                                    U64 &fmt_len) = 0;
    virtual std::list<com_fmt_range>& get_format_range() { return fmt_range_list_;}

protected:

    bool QueryPartLayout(const QSharedPointer<Connection> &conn,const std::string part_name,
                          unsigned int &addr, unsigned int &len);


protected:
    std::list<com_fmt_range> fmt_range_list_;
private:
    U64 format_start_address_;
    U64 format_length_;

};

}

#endif // AUTOFORMATARG_H
