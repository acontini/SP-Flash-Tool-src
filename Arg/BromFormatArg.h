#ifndef BROMFORMATARG_H
#define BROMFORMATARG_H

#include "../BootRom/flashtool_api.h"

class BromFormatArg
{
public:
    BromFormatArg(void);
    explicit BromFormatArg(const FlashTool_Format_Arg& format_arg);
    ~BromFormatArg(void){}
    FlashTool_Format_Arg GetBromFormatArg(void) const {
        return this->flashtool_format_arg_;
    }
    //setters
    void set_auto_format(const _BOOL auto_fmt) {
        this->flashtool_format_arg_.m_format_cfg.m_auto_format_fat = auto_fmt;
    }

    void set_format_begin_addr(const U64 begin_addr) {
        this->flashtool_format_arg_.m_format_cfg.m_format_begin_addr = begin_addr;
    }
    void set_format_length(const U64 length) {
        this->flashtool_format_arg_.m_format_cfg.m_format_length = length;
    }
    void set_addr_type(const NUTL_AddrTypeFlag_E addr_type) {
        this->flashtool_format_arg_.m_format_cfg.m_AddrType = addr_type;
    }
    void set_validation(const _BOOL validation) {
        this->flashtool_format_arg_.m_format_cfg.m_validation = validation;
    }
    void set_storage_type(const HW_StorageType_E storage_type) {
        this->flashtool_format_arg_.m_storage_type = storage_type;
    }
    void set_m_erase_flag(const NUTL_EraseFlag_E erase_flag) {
        this->flashtool_format_arg_.m_erase_flag = erase_flag;
    }

    void set_partition_id(const U32 id)
    {
        this->flashtool_format_arg_.m_format_cfg.m_part_id = id;
    }

    void set_cb_format_report_init(const CALLBACK_FORMAT_PROGRESS_INIT cb_format_report_init) {
        this->flashtool_format_arg_.m_cb_format_report_init = cb_format_report_init;
    }
    void set_cb_format_report_init_arg(void* host_pointer) {
        this->flashtool_format_arg_.m_cb_format_report_arg = host_pointer;
    }
    void set_cb_format_report(const CALLBACK_FORMAT_PROGRESS cb_format_report) {
        this->flashtool_format_arg_.m_cb_format_report = cb_format_report;
    }
    void set_cb_format_report_arg(void* host_pointer) {
        this->flashtool_format_arg_.m_cb_format_report_arg = host_pointer;
    }
    void set_cb_format_statistics(const CALLBACK_FORMAT_STATISTICS cb_format_statistics) {
        this->flashtool_format_arg_.m_cb_format_statistics = cb_format_statistics;
    }
    void set_cb_format_statistics_arg(void* host_pointer) {
        this->flashtool_format_arg_.m_cb_format_statistics_arg = host_pointer;
    }

    void Dump(void);
public:
    //console callback functions
    static int __stdcall cb_format_report_init(
        HW_StorageType_E  storage_type,
        U64  begin_addr,
        U64 length,
        void *usr_arg);
    static int __stdcall cb_format_report(
        unsigned char finished_percentage,
        void *usr_arg);
    static int __stdcall cb_format_statistics(
        const FormatStatisticsReport_T* p_report,
        void* usr_arg);
private:
    void FillDefaultValue(void);
    FlashTool_Format_Arg flashtool_format_arg_;
};


#endif // BROMFORMATARG_H
