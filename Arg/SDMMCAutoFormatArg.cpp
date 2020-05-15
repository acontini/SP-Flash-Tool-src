#include "SDMMCAutoFormatArg.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"


APCore::SDMMCAutoFormatArg::SDMMCAutoFormatArg(const QSharedPointer<Connection> &conn)
{
    this->CalcFormatAllLength(&conn->da_report());
}

void APCore::SDMMCAutoFormatArg::GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                                                    const FormatSetting::AutoFormatFlag fmt_flag,
                                                    bool does_physical_format,
                                                    U64 &fmt_addr,
                                                    U64 &fmt_len)
{
    LOG("physical(%d)", does_physical_format);

    switch(fmt_flag)
    {
    case FormatSetting::Format_ALL:
        {
            fmt_addr = 0;
            fmt_len = this->format_all_length_;

            LOG("Format_ALL(0x%llx, 0x%llx)", fmt_addr, fmt_len);
        }
        break;

    case FormatSetting::Format_Except_BL:
    {
        unsigned int bl_addr = 0;
        unsigned int bl_len  = 0;

        if(!AutoFormatArg::QueryPartLayout(conn, "PRELOADER", bl_addr, bl_len))
        {
            THROW_APP_EXCEPTION(FT_FIND_NVRAM_ERROR, "");
        }

        fmt_addr = bl_addr + bl_len;


        fmt_len = this->format_all_length_ - fmt_addr;
    }
        break;

    default:
        Q_ASSERT(0 && "Unknown auto format flag.");
    }
}

void APCore::SDMMCAutoFormatArg::CalcFormatAllLength(const DA_REPORT_T *da_report)
{
    this->format_all_length_ = da_report->m_sdmmc_ua_size;
}
