#ifndef FORMATCOMMAND_H
#define FORMATCOMMAND_H

#include "ICommand.h"
#include "../BootRom/DOWNLOAD.H"
#include "../Arg/BromFormatArg.h"
#include "../Arg/AutoFormatArg.h"
#include "../Arg/NORFormatArg.h"
#include "../Arg/NANDFormatArg.h"
#include "../Arg/EMMCFormatArg.h"
#include "../Arg/SDMMCAutoFormatArg.h"
#include "../Arg/UFSFormatArg.h"
#include "../Setting/FormatSetting.h"

namespace APCore
{
class AutoFormatArg;
class FormatCommand : public ICommand
{
public:
    FormatCommand(APKey key);
    virtual ~FormatCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_does_physical_format(const bool does)
    {
        this->does_physical_format = does;
    }

    void set_storage_type(const HW_StorageType_E storage_type)
    {
        this->storage_type = storage_type;
    }

    void set_auto_format_flag(FormatSetting::AutoFormatFlag auto_fmt_flag)
    {
        this->auto_fmt_flag = auto_fmt_flag;
    }

    void does_auto_format_flag(bool does)
    {
        this->does_auto_fmt = does;
    }

    void set_partition_id(U32 id)
    {
        this->part_id = id;
    }

    void set_is_normal_fmt(bool normal_fmt)
    {
        this->is_normal_fmt_ = normal_fmt;
    }

private:
    friend class FormatSetting;
    BromFormatArg format_arg;
    bool does_auto_fmt;
    FormatSetting::AutoFormatFlag auto_fmt_flag;
    HW_StorageType_E storage_type;
    bool does_physical_format;
    U32 part_id;
    bool is_normal_fmt_;

    QSharedPointer<APCore::AutoFormatArg> CreateAutoFormatArg(const HW_StorageType_E type, const QSharedPointer<Connection> & conn);
    bool ValidateFormatParameter(const QSharedPointer<Connection> &conn, const FlashTool_Format_Arg &fmt_arg);
};

}

#endif // FORMATCOMMAND_H
