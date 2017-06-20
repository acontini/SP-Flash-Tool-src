#ifndef EMMCFORAMTARG_H
#define EMMCFORAMTARG_H

#include "AutoFormatArg.h"

namespace APCore
{
class Connection;
class EMMCFormatArg:public AutoFormatArg
{
public:
    EMMCFormatArg(const QSharedPointer<Connection> & conn,
                  bool is_com_format = false,
                  EMMC_Part_E id = EMMC_PART_UNKNOWN,
                   bool is_auto_fmt = false);

    virtual void GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                                    const FormatSetting::AutoFormatFlag fmt_flag,
                                    bool does_physical_format,
                                    U64 &fmt_addr,
                                    U64 &fmt_len);


    void set_is_combo_fmt(bool is_combo_fmt)
    {
        this->is_combo_fmt_ = is_combo_fmt;
    }

    void set_is_auto_fmt(bool auto_fmt)
    {
        this->is_auto_fmt_ = auto_fmt;
    }

    void set_partition_id(EMMC_Part_E id)
    {
        part_id_ = id;
    }

    std::list<com_fmt_range>& get_format_range() { return fmt_range_list_;}

private:	
    U64 format_all_length_;
    bool is_combo_fmt_;
    bool is_auto_fmt_;
    EMMC_Part_E part_id_;
//    std::list<com_fmt_range> fmt_range_list_;
    void CalcFormatAllLength(const DA_REPORT_T* da_report);
};

}



#endif // EMMCFORAMTARG_H
