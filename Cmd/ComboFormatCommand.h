#ifndef COMBOFORMATCOMMAND_H
#define COMBOFORMATCOMMAND_H

#include "ICommand.h"
#include "../Setting/ComboFormatSetting.h"
#include "../Setting/FormatSetting.h"
#include "../Arg/AutoFormatArg.h"
#include "../Arg/EMMCFormatArg.h"
#include "../Arg/UFSFormatArg.h"

#include <QSharedPointer>

namespace APCore
{
class ComboFormatCommand : public ICommand
{
public:
    ComboFormatCommand(APKey key);
    virtual ~ComboFormatCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void DoComboFormat(const QSharedPointer<Connection> &conn);
    void DoNormatFormat(const QSharedPointer<Connection> &conn);

    void set_format_setting(
        const QSharedPointer<FormatSetting> &fmt_setting)
    {
        this->fmt_setting_ = fmt_setting;
    }

    void set_is_combo_format(bool is_combo_fmt)
    {
        this->is_combo_format_ = is_combo_fmt;
    }

    void set_auto_format_flag(FormatSetting::AutoFormatFlag auto_fmt_flag)
    {
        this->auto_fmt_flag_ = auto_fmt_flag;
    }

    void set_is_auto_format(bool does)
    {
        this->does_auto_fmt_ = does;
    }

    void set_storage_type(const HW_StorageType_E storage_type)
    {
        this->storage_type = storage_type;
    }

private:
    QSharedPointer<APCore::FormatSetting> fmt_setting_;
    bool is_combo_format_;
    bool does_auto_fmt_;
    FormatSetting::AutoFormatFlag auto_fmt_flag_;

    std::list<com_fmt_range> format_range_list;
    HW_StorageType_E storage_type;
};
}

#endif // COMBOFORMATCOMMAND_H
