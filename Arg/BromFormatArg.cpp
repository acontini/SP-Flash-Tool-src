#include <memory.h>
#include <iostream>

#include "../Arg/BromFormatArg.h"
#include "../Logger/Log.h"
#include "../BootRom/flashtool.h"

//Brom Format Argument
BromFormatArg::BromFormatArg()
{
    this->FillDefaultValue();
}

BromFormatArg::BromFormatArg(const FlashTool_Format_Arg &format_arg)
{
    memset(&flashtool_format_arg_, 0, sizeof(FlashTool_Format_Arg));
    this->flashtool_format_arg_.m_format_cfg.m_format_begin_addr = format_arg.m_format_cfg.m_format_begin_addr;
    this->flashtool_format_arg_.m_format_cfg.m_format_length = format_arg.m_format_cfg.m_format_length;
    this->flashtool_format_arg_.m_format_cfg.m_AddrType = format_arg.m_format_cfg.m_AddrType;
    this->flashtool_format_arg_.m_format_cfg.m_validation = format_arg.m_format_cfg.m_validation;

    this->flashtool_format_arg_.m_storage_type = format_arg.m_storage_type;
    this->flashtool_format_arg_.m_erase_flag = format_arg.m_erase_flag;

    this->flashtool_format_arg_.m_cb_format_report_init = format_arg.m_cb_format_report_init;
    this->flashtool_format_arg_.m_cb_format_report_init_arg = format_arg.m_cb_format_report_init_arg;
    this->flashtool_format_arg_.m_cb_format_report = format_arg.m_cb_format_report;
    this->flashtool_format_arg_.m_cb_format_report_arg = format_arg.m_cb_format_report_arg;
    //Report nand format info in ONLY format operation
    this->flashtool_format_arg_.m_cb_format_statistics = format_arg.m_cb_format_statistics;
    this->flashtool_format_arg_.m_cb_format_statistics_arg = format_arg.m_cb_format_statistics_arg;
}

void BromFormatArg::FillDefaultValue(void)
{
    memset(&flashtool_format_arg_, 0, sizeof(FlashTool_Format_Arg));
    this->flashtool_format_arg_.m_format_cfg.m_format_begin_addr = 0;
    this->flashtool_format_arg_.m_format_cfg.m_format_length = 0;
    this->flashtool_format_arg_.m_format_cfg.m_AddrType = NUTL_ADDR_PHYSICAL;
    this->flashtool_format_arg_.m_format_cfg.m_validation = _FALSE;

    this->flashtool_format_arg_.m_storage_type = HW_STORAGE_NAND;
    this->flashtool_format_arg_.m_erase_flag = NUTL_ERASE;

    this->flashtool_format_arg_.m_cb_format_report_init = cb_format_report_init;
    this->flashtool_format_arg_.m_cb_format_report_init_arg = NULL;
    this->flashtool_format_arg_.m_cb_format_report = cb_format_report;
    this->flashtool_format_arg_.m_cb_format_report_arg = NULL;
    //Report nand format info in ONLY format operation
    this->flashtool_format_arg_.m_cb_format_statistics = cb_format_statistics;
    this->flashtool_format_arg_.m_cb_format_statistics_arg = NULL;
}

int BromFormatArg::cb_format_report_init(
        HW_StorageType_E  storage_type,
        U64 begin_addr,
        U64 length,
        void *usr_arg)
{
    LOGI("Formatting Flash(%d)... addr(0x%llx), len(0x%llx)",
        storage_type, begin_addr, length);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromFormatArg::cb_format_report(
    unsigned char finished_percentage,
    void * /*usr_arg*/)
{
    static unsigned char last_percentage = 0;

    if ( (finished_percentage != last_percentage) &&
         (finished_percentage % 10 == 0) )
    {
        LOGI("%d%% of flash has been formatted.", (int) finished_percentage);
        last_percentage = finished_percentage;
    }

    return 0;
}


int BromFormatArg::cb_format_statistics(
    const FormatStatisticsReport_T *p_report,
    void * /*usr_arg*/)
{
    float bad_block_ratio = 0;
    bad_block_ratio = (p_report->m_bad_blocks+p_report->m_err_blocks);
    bad_block_ratio /= p_report->m_total_blocks;
    bad_block_ratio *= 100;

    LOGI("Format range: 0x%llx ~ 0x%llx",
         p_report->m_fmt_begin_addr,
         p_report->m_fmt_begin_addr+p_report->m_fmt_length);

    LOGI("Bad Blocks: %d", p_report->m_bad_blocks);
    LOGI("Error Blocks: %d", p_report->m_err_blocks);
    LOGI("Total Blocks: %d",p_report->m_total_blocks);
    LOGI("Bad/Error Block Ratio: %3.2f%%", bad_block_ratio);

    return 0;
}

void BromFormatArg::Dump()
{
    LOG("m_format_cfg.m_format_begin_addr(0x%llx).", flashtool_format_arg_.m_format_cfg.m_format_begin_addr);
    LOG("m_format_cfg.m_format_length(0x%llx).", flashtool_format_arg_.m_format_cfg.m_format_length);
    LOG("m_format_cfg.m_AddrType(%s).", AddrTypeFlagToString(flashtool_format_arg_.m_format_cfg.m_AddrType));
    LOG("m_format_cfg.m_validation(%d).", flashtool_format_arg_.m_format_cfg.m_validation);

    LOG("m_storage_type(%s).", HWStorageTypeToString(flashtool_format_arg_.m_storage_type));
    LOG("m_erase_flag(%s).",  EraseFlagToString(flashtool_format_arg_.m_erase_flag));

    LOG("m_cb_format_report_init(0x%x).", flashtool_format_arg_.m_cb_format_report_init);
    LOG("m_cb_format_report_init_arg(0x%x).", flashtool_format_arg_.m_cb_format_report_init_arg);
    LOG("m_cb_format_report(0x%x).", flashtool_format_arg_.m_cb_format_report);
    LOG("m_cb_format_report_arg(0x%x).", flashtool_format_arg_.m_cb_format_report_arg);
    //Report nand format info in ONLY format operation
    LOG("m_cb_format_statistics(0x%x).", flashtool_format_arg_.m_cb_format_statistics);
    LOG("m_cb_format_statistics_arg(0x%x).", flashtool_format_arg_.m_cb_format_statistics_arg);
}
