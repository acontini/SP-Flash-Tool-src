#include "ComboFormatCommand.h"
#include "../Resource/ResourceManager.h"

namespace APCore
{
ComboFormatCommand::ComboFormatCommand(APKey key):
    ICommand(key),
    fmt_setting_(NULL),
    storage_type(HW_STORAGE_EMMC)
{
}

ComboFormatCommand::~ComboFormatCommand()
{

}

void ComboFormatCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(is_combo_format_ && does_auto_fmt_)
        DoComboFormat(conn);
    else
        DoNormatFormat(conn);
}

void ComboFormatCommand::DoNormatFormat(const QSharedPointer<Connection> &conn)
{
    fmt_setting_->set_is_normal_format(!is_combo_format_);

    return fmt_setting_->CreateCommand(conn->ap_key())->exec(conn);
}

void ComboFormatCommand::DoComboFormat(const QSharedPointer<Connection> &conn)
{
    QSharedPointer<APCore::AutoFormatArg> auto_fmt_ptr;
    if(HW_STORAGE_EMMC == storage_type)
    {
        auto_fmt_ptr = QSharedPointer<APCore::AutoFormatArg>(new EMMCFormatArg(conn, true, EMMC_PART_UNKNOWN, true));
    }
    else if(HW_STORAGE_UFS == storage_type)
    {
        auto_fmt_ptr = QSharedPointer<APCore::AutoFormatArg>(new UFSFormatArg(conn, true, UFS_PART_UNKNOWN, true));
    }

    std::list<com_fmt_range> fmt_range = auto_fmt_ptr.data()->get_format_range();

    std::list<com_fmt_range>::const_iterator it = fmt_range.begin();

    while(it != fmt_range.end())
    {
        if(this->auto_fmt_flag_ == FormatSetting::Format_Except_BL && it->is_bl_range)
        {
            ++it;
            continue;
        }

        fmt_setting_->set_auto_format(false);
        fmt_setting_->set_part_id(it->part_id);
        fmt_setting_->set_begin_addr(it->fmt_begin);
        fmt_setting_->set_length(it->fmt_length);

        fmt_setting_->CreateCommand(conn->ap_key())->exec(conn);

        ++it;
    }
}

}
