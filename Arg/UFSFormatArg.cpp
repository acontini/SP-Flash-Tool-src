#include "UFSFormatArg.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

APCore::UFSFormatArg::UFSFormatArg(const QSharedPointer<Connection> &conn, bool is_com_format,
                                     UFS_Part_E id, bool is_auto_fmt):
    format_all_length_(0),
    is_combo_fmt_(is_com_format),
    is_auto_fmt_(is_auto_fmt),
    part_id_(id)
{
    this->CalcFormatAllLength(& conn->da_report());
}

void APCore::UFSFormatArg::GenAutoFormatRange(const QSharedPointer<Connection> &conn,
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
            unsigned int dsp_bl_addr = 0;
            unsigned int dsp_bl_len = 0;

            if(!AutoFormatArg::QueryPartLayout(conn, "PRELOADER", bl_addr, bl_len))
            {
                THROW_APP_EXCEPTION(FT_FIND_NVRAM_ERROR, "");
            }

            fmt_addr = bl_addr + bl_len;

            if(AutoFormatArg::QueryPartLayout(conn, "DSP_BL", dsp_bl_addr, dsp_bl_len))
                fmt_addr += dsp_bl_len;

            fmt_len = this->format_all_length_ - fmt_addr;
        }
        break;

        default:
            Q_ASSERT(0 && "Unknown auto format flag.");
    }
}

void  APCore::UFSFormatArg::CalcFormatAllLength(const DA_REPORT_T* da_report)
{
    if(is_combo_fmt_)
    {
        if(is_auto_fmt_)
        {
            com_fmt_range range;
            range.fmt_begin = 0;

            for(int i = UFS_PART_LU0; i <= UFS_PART_LU2; i++)
            {
                range.part_id = i;

                switch(i)
                {
                case UFS_PART_LU0:
                    range.fmt_length = da_report->m_ufs_lu0_size;
                    range.is_bl_range = true;
                    break;

                case UFS_PART_LU1:
                    range.fmt_length = da_report->m_ufs_lu1_size;
                    range.is_bl_range = true;
                    break;

                case UFS_PART_LU2:
                    range.fmt_length = da_report->m_ufs_lu2_size;
                    range.is_bl_range = false;
                    break;
                }

                if(i == part_id_)
                    this->format_all_length_ = range.fmt_length;

                fmt_range_list_.push_back(range);
            }
        }
        else if(part_id_ > UFS_PART_UNKNOWN && part_id_ < UFS_PART_END)
        {
            switch(part_id_)
            {
            case UFS_PART_LU0:
                this->format_all_length_ = da_report->m_ufs_lu0_size;
                break;

            case UFS_PART_LU1:
                this->format_all_length_ = da_report->m_ufs_lu1_size;
                break;

            case UFS_PART_LU2:
                this->format_all_length_ = da_report->m_ufs_lu2_size;
                break;

            default:
                break;
            }
        }
    }
    else
        this->format_all_length_ = da_report->m_ufs_lu0_size +
            da_report->m_ufs_lu1_size + da_report->m_ufs_lu2_size/* + da_report->m_ufs_lu3_size*/;

}
