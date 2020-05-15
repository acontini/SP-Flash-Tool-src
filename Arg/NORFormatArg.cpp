#include "NORFormatArg.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

APCore::NORFormatArg::NORFormatArg()
{

}

void APCore::NORFormatArg::GenAutoFormatRange(const QSharedPointer<Connection> &conn,
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
            fmt_len = conn->da_report().m_nor_flash_size;

            LOG("Format_ALL(0x%llx, 0x%llx)", fmt_addr, fmt_len);
    }
    break;

    case FormatSetting::Format_Except_BL:
    {
        unsigned int bl_addr = 0;
        unsigned int bl_len  = 0;
        if(this->QueryPartLayout(conn, "PRELOADER", bl_addr, bl_len))
        {
            fmt_addr = bl_addr + bl_len;
            fmt_len = conn->da_report().m_nor_flash_size - fmt_addr;


            LOG("Format_Except_BL(0x%llx, 0x%llx)", fmt_addr, fmt_len);
        }
        else
        {
            THROW_APP_EXCEPTION(FT_FIND_NVRAM_ERROR, "");
        }
    }
    break;

    default:
        Q_ASSERT(0 && "Unknown auto format flag.");
    }
}
