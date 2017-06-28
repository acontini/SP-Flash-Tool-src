/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2011
*
*****************************************************************************/
#ifndef _BROM_BOOT_ARG_H
#define _BROM_BOOT_ARG_H

#include "../BootRom/flashtool_api.h"

class BromBootArg
{
public:
    BromBootArg();
    explicit BromBootArg(const FlashTool_Connect_Arg& conn_arg);
    ~BromBootArg(){}

    const FlashTool_Connect_Arg &GetBromBootArg(void) const {
        return flashtool_connect_arg_;
    }
    //Setters
    void set_chip_type(const BBCHIP_TYPE chip_type) {
        this->flashtool_connect_arg_.m_boot_arg.m_bbchip_type = chip_type;
    }
    void set_ms_boot_timeout(const unsigned int timeout) {
        this->flashtool_connect_arg_.m_boot_arg.m_ms_boot_timeout = timeout;
    }
    void set_cb_com_init_stage(const CALLBACK_COM_INIT_STAGE cb_com_init) {
        this->flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage = cb_com_init;
    }
    void set_cb_com_init_stage_arg(void * host_pointer) {
        this->flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage_arg = host_pointer;
    }
    void set_auth_handle(const AUTH_HANDLE_T auth_handle) {
        this->flashtool_connect_arg_.m_boot_arg.m_auth_handle = auth_handle;
    }
    void set_scert_handle(const SCERT_HANDLE_T scert_handle) {
        this->flashtool_connect_arg_.m_boot_arg.m_scert_handle = scert_handle;
    }
    void set_da_handle(const DA_HANDLE_T da_handle) {
        this->flashtool_connect_arg_.m_boot_arg.m_da_handle = da_handle;
    }
    void set_cb_download_da_init(const CALLBACK_WRITE_BUF_PROGRESS_INIT cb_download_da_init) {
        this->flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init = cb_download_da_init;
    }
    void set_cb_download_da_init_arg(void * host_pointer) {
        this->flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init_arg = host_pointer;
    }
    void set_cb_download_da(const CALLBACK_WRITE_BUF_PROGRESS download_da) {
        this->flashtool_connect_arg_.m_boot_arg.m_cb_download_da = download_da;
    }
    void set_cb_download_da_arg(void * host_pointer) {
        this->flashtool_connect_arg_.m_boot_arg.m_cb_download_da_arg = host_pointer;
    }
    void set_cb_security_pre_process_notify(const CALLBACK_SECURITY_PRE_PROCESS_NOTIFY cb_security_pre_process_notify) {
        this->flashtool_connect_arg_.m_cb_security_pre_process_notify = cb_security_pre_process_notify;
    }
    void set_cb_security_pre_process_notify_arg(void * host_pointer) {
        this->flashtool_connect_arg_.m_cb_security_pre_process_notify_arg = host_pointer;
    }
    void set_bmt_block_count(const unsigned int bmt_block_count) {
        this->flashtool_connect_arg_.m_boot_arg.m_bmt_block_count = bmt_block_count;
    }

    void Dump(void);
public:
    //console callback functions
    static int __stdcall cb_com_init_stage(HANDLE hCOM, void *usr_arg);
    static int __stdcall cb_download_da_init(void *usr_arg);
    static int __stdcall cb_download_da(unsigned char percent, unsigned int sent_bytes, unsigned int total_bytes, void *usr_arg);
    static int __stdcall cb_security_pre_process_notify(void *usr_arg);
private:
    void FillDefaultValue(void);
private:
    FlashTool_Connect_Arg flashtool_connect_arg_;
    const int kBMTDefaultBlockNum;
};

class BromUSBStatusArg
{
public:
    BromUSBStatusArg();
    explicit BromUSBStatusArg(const FlashTool_USB_Status_Arg &stat_arg);
        ~BromUSBStatusArg(){}

    void set_cb_usb_stat_init(const CALLBACK_USB_STATUS_INIT cb)
    {
        this->flashtool_usb_stat_arg_.m_cb_usb_status_init = cb;
    }

    void set_cb_usb_stat_init_arg(void *host_pointer)
    {
        this->flashtool_usb_stat_arg_.cb_usb_status_init_arg = host_pointer;
    }
    const FlashTool_USB_Status_Arg &GetUsbStatArg(void) const
    {
        return flashtool_usb_stat_arg_;
    }

private:
    void FillDefaultValue(void);

private:
    FlashTool_USB_Status_Arg flashtool_usb_stat_arg_;
    static int __stdcall cb_checkusbstatus_init(void * arg, const char* usb_status);

};

class BromWatchDogArg
{
public:

    BromWatchDogArg();
    explicit BromWatchDogArg(const FlashTool_EnableWDT_Arg &wdt_arg);
    ~BromWatchDogArg(){}

    const FlashTool_EnableWDT_Arg &GetBromWDTArg(void) const
    {
        return flashtool_enablewdt_arg_;
    }

    void Dump(void);
public:
    //console callback functions
    static int __stdcall cb_format_report_init(
        HW_StorageType_E  storage_type,
        U64 begin_addr,
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
private:
    FlashTool_EnableWDT_Arg flashtool_enablewdt_arg_;
};

#endif
