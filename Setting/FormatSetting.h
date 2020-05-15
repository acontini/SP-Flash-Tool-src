#ifndef FORMATSETTING_H
#define FORMATSETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"

namespace APCore
{
class FormatSetting : public ISetting
{
public:
    enum AutoFormatFlag
    {
        Format_ALL,
        Format_NVRAM,
        Format_Except_BL,
        Format_FlagEnd
    };

    FormatSetting();
    virtual QSharedPointer<ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    QSharedPointer<FormatSetting> Clone() const
    {
        return QSharedPointer<FormatSetting>(new FormatSetting(*this));
    }

    void set_cb_fmt_init(CALLBACK_FORMAT_PROGRESS_INIT cb)
    {
        cb_fmt_init = cb;
    }

    void set_cb_fmt_progress(CALLBACK_FORMAT_PROGRESS cb)
    {
        cb_fmt_progress = cb;
    }

    void set_cb_fmt_statistics(CALLBACK_FORMAT_STATISTICS cb)
    {
        cb_fmt_statistics = cb;
    }

    void set_validation(bool does_valid)
    {
        does_validation = does_valid;
    }

    void set_physical_fmt(bool does_physical)
    {
        does_physical_fmt = does_physical;
    }

    void set_erase_flag(NUTL_EraseFlag_E flag)
    {
        erase_flag = flag;
    }

    void set_auto_format(bool auto_fmt)
    {
        does_auto_format = auto_fmt;
    }

    void set_begin_addr(U64 addr)
    {
        begin_addr = addr;
    }

    void set_length(U64 len)
    {
        length = len;
    }

    void set_part_id(U32 id)
    {
        part_id = id;
    }

    void set_auto_format_flag(AutoFormatFlag flag)
    {
        auto_format_flag = flag;
    }

    void set_whole_format(bool whole_format)
    {
        does_whole_format = whole_format;
    }

    void set_is_normal_format(bool normal_fmt)
    {
        is_normal_format_ = normal_fmt;
    }

    void update_format_type_by_region_id(U32 part_id);

    bool does_format_whole()
    {
        return does_whole_format;
    }

    bool is_auto_format()
    {
        return does_auto_format;
    }

    AutoFormatFlag get_format_flag()
    {
        return auto_format_flag;
    }

private:
    FormatSetting(const FormatSetting&);
    FormatSetting& operator=(const FormatSetting&);

    CALLBACK_FORMAT_PROGRESS_INIT cb_fmt_init;
    CALLBACK_FORMAT_PROGRESS cb_fmt_progress;
    CALLBACK_FORMAT_STATISTICS cb_fmt_statistics;

    bool does_validation;
    bool does_physical_fmt;
    bool is_normal_format_;

    NUTL_EraseFlag_E erase_flag;

    bool does_auto_format;
    bool does_whole_format;
    U64 begin_addr;
    U64 length;
    U32 part_id;

    AutoFormatFlag auto_format_flag;
};

}

#endif // FORMATSETTING_H
