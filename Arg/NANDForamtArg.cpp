#include "NANDFormatArg.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

APCore::NANDFormatArg::NANDFormatArg(DL_HANDLE_T dl_handle)
{
    this->dl_handle_ = dl_handle;
}

void APCore::NANDFormatArg::GenAutoFormatRange(const QSharedPointer<Connection> &conn,
                        const FormatSetting::AutoFormatFlag fmt_flag,
                        bool does_physical_format,
                        U64 &fmt_addr,
                        U64 &fmt_len)
{
    unsigned int bmt_blocks = 0;
    int ret = DL_GetBMTBlockCount(dl_handle_, &bmt_blocks);
    if(ret == S_DONE)
    {
        LOG("Got BMT block count = %d", bmt_blocks);
    }
    else
    {
        LOG("Failed to get BMT block count, use 80 as default.");
        bmt_blocks = 80;
    }

    const unsigned int reserved_tail_blocks = bmt_blocks+2/*PMT*/;
   
    switch(fmt_flag)
    {
    case FormatSetting::Format_ALL:
        {
            fmt_addr = 0;
            if(!does_physical_format)
            {
                fmt_len = conn->da_report().m_nand_flash_size -
                    (reserved_tail_blocks
                     *conn->da_report().m_nand_pagesize
                     *conn->da_report().m_nand_pages_per_block);
            }
            else
            {
                fmt_len = conn->da_report().m_nand_flash_size;
            }
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
                if(!does_physical_format)
                {
                    fmt_len = conn->da_report().m_nand_flash_size - fmt_addr -
                         (reserved_tail_blocks
                         *conn->da_report().m_nand_pagesize
                         *conn->da_report().m_nand_pages_per_block);
                }
                else
                {
                    fmt_len = conn->da_report().m_nand_flash_size - fmt_addr;
                }

                LOG("Format_Except_BL_NVRAM(0x%llx, 0x%llx)", fmt_addr, fmt_len);
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



