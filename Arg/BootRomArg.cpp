/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2011
*
*****************************************************************************/
#include <memory.h>
#include <iostream>

#include "../Arg/BootRomArg.h"
#include "../Logger/Log.h"
#include "../BootRom/SLA_Challenge.h"
#include "../BootRom/flashtool.h"

using std::cout;
using std::endl;

//Brom Boot Argument
BromBootArg::BromBootArg(const FlashTool_Connect_Arg &conn_arg):
    kBMTDefaultBlockNum(80)
{
    memset(&flashtool_connect_arg_, 0, sizeof(FlashTool_Connect_Arg));
    // com port timeout setting
    flashtool_connect_arg_.m_com_ms_read_timeout = conn_arg.m_com_ms_read_timeout;
    flashtool_connect_arg_.m_com_ms_write_timeout = conn_arg.m_com_ms_write_timeout;
    flashtool_connect_arg_.m_boot_arg.m_baudrate = conn_arg.m_boot_arg.m_baudrate;

    // fill BOOT_FLASHTOOL_ARG
    flashtool_connect_arg_.m_boot_arg.m_bbchip_type = conn_arg.m_boot_arg.m_bbchip_type; //g_BBCHIP_TYPE;
    flashtool_connect_arg_.m_boot_arg.m_ext_clock = conn_arg.m_boot_arg.m_ext_clock;
    flashtool_connect_arg_.m_boot_arg.m_ms_boot_timeout = conn_arg.m_boot_arg.m_ms_boot_timeout;
    flashtool_connect_arg_.m_boot_arg.m_max_start_cmd_retry_count = conn_arg.m_boot_arg.m_max_start_cmd_retry_count;

    // Initialization for Edge Moudle
    // This callback function will be invoke after COM port is opened
    // You can do some initialization by using this callback function.
    flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage = conn_arg.m_boot_arg.m_cb_com_init_stage;
    flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage_arg = conn_arg.m_boot_arg.m_cb_com_init_stage_arg;

    // This callback function will be invoke after BootROM start cmd is passed.
    // You can issue other BootROM command by brom_handle and hCOM which
    // provides callback arguments,or do whatever you want otherwise.
    flashtool_connect_arg_.m_boot_arg.m_cb_in_brom_stage  = conn_arg.m_boot_arg.m_cb_in_brom_stage;
    flashtool_connect_arg_.m_boot_arg.m_cb_in_brom_stage_arg = conn_arg.m_boot_arg.m_cb_in_brom_stage_arg;

    // speed-up BootROM stage baudrate
    flashtool_connect_arg_.m_boot_arg.m_speedup_brom_baudrate = conn_arg.m_boot_arg.m_speedup_brom_baudrate;

    // Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message
    flashtool_connect_arg_.m_boot_arg.m_ready_power_on_wnd_handle = conn_arg.m_boot_arg.m_ready_power_on_wnd_handle;
    flashtool_connect_arg_.m_boot_arg.m_ready_power_on_wparam = conn_arg.m_boot_arg.m_ready_power_on_wparam;
    flashtool_connect_arg_.m_boot_arg.m_ready_power_on_lparam = conn_arg.m_boot_arg.m_ready_power_on_lparam;

    // serial link authentication arg
    flashtool_connect_arg_.m_boot_arg.m_auth_handle = conn_arg.m_boot_arg.m_auth_handle;
    flashtool_connect_arg_.m_boot_arg.m_scert_handle = conn_arg.m_boot_arg.m_scert_handle;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge = conn_arg.m_boot_arg.m_cb_sla_challenge;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_arg = conn_arg.m_boot_arg.m_cb_sla_challenge_arg;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_end = conn_arg.m_boot_arg.m_cb_sla_challenge_end;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_end_arg = conn_arg.m_boot_arg.m_cb_sla_challenge_end_arg;

    // EMI Control for bank0 and bank1
    flashtool_connect_arg_.m_boot_arg.m_p_bank0_mem_cfg = conn_arg.m_boot_arg.m_p_bank0_mem_cfg;
    flashtool_connect_arg_.m_boot_arg.m_p_bank1_mem_cfg = conn_arg.m_boot_arg.m_p_bank1_mem_cfg;

    // DA handle should be filed NULL under skip bootrom mode
    flashtool_connect_arg_.m_boot_arg.m_da_handle = conn_arg.m_boot_arg.m_da_handle;

    // da download progress callack
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init = conn_arg.m_boot_arg.m_cb_download_da_init;
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init_arg = conn_arg.m_boot_arg.m_cb_download_da_init_arg;
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da = conn_arg.m_boot_arg.m_cb_download_da;
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da_arg = conn_arg.m_boot_arg.m_cb_download_da_arg;


    // security pre-process notify callback
    flashtool_connect_arg_.m_cb_security_pre_process_notify = conn_arg.m_cb_security_pre_process_notify;
    flashtool_connect_arg_.m_cb_security_pre_process_notify_arg = conn_arg.m_cb_security_pre_process_notify_arg;

    // chip-select of NOR flash memory
    flashtool_connect_arg_.m_nor_chip_select[0] = conn_arg.m_nor_chip_select[0];
    flashtool_connect_arg_.m_nor_chip_select[1] = conn_arg.m_nor_chip_select[1];
    flashtool_connect_arg_.m_nand_chip_select = conn_arg.m_nand_chip_select;

    //  NFI access contol register
    flashtool_connect_arg_.m_p_nand_acccon = conn_arg.m_p_nand_acccon;

    // BMT block count
    flashtool_connect_arg_.m_boot_arg.m_bmt_block_count = conn_arg.m_boot_arg.m_bmt_block_count;
}

BromBootArg::BromBootArg() :
    kBMTDefaultBlockNum(80)
{
    FillDefaultValue();
}

void BromBootArg::FillDefaultValue()
{
    memset(&flashtool_connect_arg_, 0, sizeof(FlashTool_Connect_Arg));
    // com port timeout setting
    flashtool_connect_arg_.m_com_ms_read_timeout = 100; //COM_DEFAULT_TIMEOUT;
    flashtool_connect_arg_.m_com_ms_write_timeout = 1500;// COM_DEFAULT_TIMEOUT;
    flashtool_connect_arg_.m_boot_arg.m_baudrate = 115200;

    // fill BOOT_FLASHTOOL_ARG
    flashtool_connect_arg_.m_boot_arg.m_bbchip_type = AUTO_DETECT_BBCHIP; //g_BBCHIP_TYPE;
    flashtool_connect_arg_.m_boot_arg.m_ext_clock = AUTO_DETECT_EXT_CLOCK;
    flashtool_connect_arg_.m_boot_arg.m_ms_boot_timeout = BOOT_INFINITE;
    flashtool_connect_arg_.m_boot_arg.m_max_start_cmd_retry_count = DEFAULT_BROM_START_CMD_RETRY_COUNT;

    // Initialization for Edge Moudle
    // This callback function will be invoke after COM port is opened
    // You can do some initialization by using this callback function.
    flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage = BromBootArg::cb_com_init_stage;
    flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage_arg = NULL;

    // This callback function will be invoke after BootROM start cmd is passed.
    // You can issue other BootROM command by brom_handle and hCOM which
    // provides callback arguments,or do whatever you want otherwise.
    flashtool_connect_arg_.m_boot_arg.m_cb_in_brom_stage  = NULL;
    flashtool_connect_arg_.m_boot_arg.m_cb_in_brom_stage_arg = NULL;

    // speed-up BootROM stage baudrate
    flashtool_connect_arg_.m_boot_arg.m_speedup_brom_baudrate = _TRUE;

    // Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message
    flashtool_connect_arg_.m_boot_arg.m_ready_power_on_wnd_handle = NULL;
    flashtool_connect_arg_.m_boot_arg.m_ready_power_on_wparam = NULL;
    flashtool_connect_arg_.m_boot_arg.m_ready_power_on_lparam = NULL;

    // serial link authentication arg
    flashtool_connect_arg_.m_boot_arg.m_auth_handle = NULL;
    flashtool_connect_arg_.m_boot_arg.m_scert_handle = NULL;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge = SLA_Challenge;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_end = SLA_Challenge_END;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_arg = NULL;
    flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_end_arg = NULL;

    // EMI Control for bank0 and bank1
    flashtool_connect_arg_.m_boot_arg.m_p_bank0_mem_cfg = NULL;
    flashtool_connect_arg_.m_boot_arg.m_p_bank1_mem_cfg = NULL;

    // DA handle should be filed NULL under skip bootrom mode
    flashtool_connect_arg_.m_boot_arg.m_da_handle = NULL;

    // da download progress callack
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init = cb_download_da_init;
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init_arg = NULL;
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da = cb_download_da;
    flashtool_connect_arg_.m_boot_arg.m_cb_download_da_arg = NULL;


    // security pre-process notify callback
    flashtool_connect_arg_.m_cb_security_pre_process_notify = cb_security_pre_process_notify;
    flashtool_connect_arg_.m_cb_security_pre_process_notify_arg = NULL;

    // chip-select of NOR flash memory
    flashtool_connect_arg_.m_nor_chip_select[0] = CS_0;
    flashtool_connect_arg_.m_nor_chip_select[1] = CS_WITH_DECODER;
    flashtool_connect_arg_.m_nand_chip_select = CS_0;

    //  NFI access contol register
    flashtool_connect_arg_.m_p_nand_acccon = NULL;

    // BMT block count
    flashtool_connect_arg_.m_boot_arg.m_bmt_block_count = kBMTDefaultBlockNum;
    flashtool_connect_arg_.m_storage_type = HW_STORAGE_NAND;

    //set connect da deafult value to 2nd, no need enable dram in 1stDA
    flashtool_connect_arg_.m_conn_da_end_stage = SECOND_DA;
    flashtool_connect_arg_.m_1st_da_enable_dram = _FALSE;

    //set da_log_level default value to info
    flashtool_connect_arg_.m_da_log_level = DA_LOG_LEVEL_INFO;
}

int BromBootArg::cb_com_init_stage(HANDLE hCOM, void *usr_arg)
{
    LOGI("COM port is open. Trying to sync with the target...");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    LOG("hCOM(%p).", hCOM);
    return 0;
}

int BromBootArg::cb_download_da_init(void *usr_arg)
{
    LOGI("Download DA now...");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromBootArg::cb_download_da(
    unsigned char percent,
    unsigned int sent_bytes,
    unsigned int total_bytes,
    void * /*usr_arg*/)
{
    static unsigned char lastReportedProgress = 0;

    if ( (percent != lastReportedProgress) &&
         (percent % 10 == 0) )
    {
        LOGI("%d%% of DA has been sent.", (int) percent);
        lastReportedProgress = percent;
    }

    LOG("The sent_bytes is %d", sent_bytes);
    LOG("The total_bytes is %d", total_bytes);

    return 0;
}

int BromBootArg::cb_security_pre_process_notify(void *usr_arg)
{
    LOG("Proceed 1st time download pre-process ...");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

void BromBootArg::Dump(void)
{
    // com port timeout setting
    LOG("m_com_ms_read_timeout(0x%x).", flashtool_connect_arg_.m_com_ms_read_timeout);
    LOG("m_com_ms_write_timeout(0x%x).", flashtool_connect_arg_.m_com_ms_write_timeout);

    // fill BOOT_FLASHTOOL_ARG
    LOG("boot_arg.m_bbchip_type(%d).", flashtool_connect_arg_.m_boot_arg.m_bbchip_type);
    LOG("m_ext_clock(%d).", flashtool_connect_arg_.m_boot_arg.m_ext_clock);
    LOG("m_baudrate(%d).", flashtool_connect_arg_.m_boot_arg.m_baudrate);
    LOG("m_ms_boot_timeout(0x%x).", flashtool_connect_arg_.m_boot_arg.m_ms_boot_timeout);
    LOG("m_max_start_cmd_retry_count(%d).", flashtool_connect_arg_.m_boot_arg.m_max_start_cmd_retry_count);

    // Initialization for Edge Moudle
    LOG("m_boot_arg.m_cb_com_init_stage(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage);
    LOG("m_boot_arg.m_cb_com_init_stage_arg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_com_init_stage_arg);

    // This callback function will be invoke after BootROM start cmd is passed.
    LOG("m_boot_arg.m_cb_in_brom_stage(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_in_brom_stage);
    LOG("m_boot_arg.m_cb_in_brom_stage_arg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_in_brom_stage_arg);

    // speed-up BootROM stage baudrate
    LOG("m_boot_arg.m_speedup_brom_baudrate(%d).", flashtool_connect_arg_.m_boot_arg.m_speedup_brom_baudrate);

    // Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message
    LOG("m_boot_arg.m_ready_power_on_wnd_handle(0x%x).", flashtool_connect_arg_.m_boot_arg.m_ready_power_on_wnd_handle);
    LOG("m_boot_arg.m_ready_power_on_wparam(0x%x).", flashtool_connect_arg_.m_boot_arg.m_ready_power_on_wparam);
    LOG("m_boot_arg.m_ready_power_on_lparam(0x%x).", flashtool_connect_arg_.m_boot_arg.m_ready_power_on_lparam);

    // serial link authentication arg
    LOG("m_boot_arg.m_auth_handle(0x%x).", flashtool_connect_arg_.m_boot_arg.m_auth_handle);
    LOG("m_boot_arg.m_scert_handle(0x%x).", flashtool_connect_arg_.m_boot_arg.m_scert_handle);
    LOG("m_boot_arg.m_cb_sla_challenge(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge);
    LOG("m_boot_arg.m_cb_sla_challenge_arg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_arg);
    LOG("m_boot_arg.m_cb_sla_challenge_end(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_end);
    LOG("m_boot_arg.m_cb_sla_challenge_end_arg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_sla_challenge_end_arg);

    // EMI Control for bank0 and bank1
    LOG("m_boot_arg.m_p_bank0_mem_cfg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_p_bank0_mem_cfg);
    LOG("m_boot_arg.m_p_bank1_mem_cfg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_p_bank1_mem_cfg);

    // DA handle should be filed NULL under skip bootrom mode
    LOG("m_boot_arg.m_da_handle(0x%x).", flashtool_connect_arg_.m_boot_arg.m_da_handle);

    // da download progress callack
    LOG("m_boot_arg.m_cb_download_da_init(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init);
    LOG("m_boot_arg.m_cb_download_da_init_arg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_download_da_init_arg);
    LOG("m_boot_arg.m_cb_download_da(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_download_da);
    LOG("m_cb_download_da_arg(0x%x).", flashtool_connect_arg_.m_boot_arg.m_cb_download_da_arg);

    // security pre-process notify callback
    LOG("m_cb_security_pre_process_notify(0x%x).", flashtool_connect_arg_.m_cb_security_pre_process_notify);
    LOG("m_cb_security_pre_process_notify_arg(0x%x).", flashtool_connect_arg_.m_cb_security_pre_process_notify_arg);

    // chip-select of NOR flash memory
    LOG("m_nor_chip_select[0](%d).", flashtool_connect_arg_.m_nor_chip_select[0]);
    LOG("m_nor_chip_select[1](%d).", flashtool_connect_arg_.m_nor_chip_select[1]);
    LOG("m_nand_chip_select(%d).", flashtool_connect_arg_.m_nand_chip_select);

    //  NFI access contol register
    LOG("m_p_nand_acccon(%u).", flashtool_connect_arg_.m_p_nand_acccon);

    // BMT block count
    LOG("m_bmt_block_count(%u).", flashtool_connect_arg_.m_boot_arg.m_bmt_block_count);

}

//USB Status Argument
BromUSBStatusArg::BromUSBStatusArg(const FlashTool_USB_Status_Arg & stat_arg)
{
      this->flashtool_usb_stat_arg_.cb_usb_status_init_arg = stat_arg.cb_usb_status_init_arg;
      this->flashtool_usb_stat_arg_.m_cb_usb_status_init = stat_arg.m_cb_usb_status_init;
}

BromUSBStatusArg:: BromUSBStatusArg()
{
      FillDefaultValue();
}

int BromUSBStatusArg::cb_checkusbstatus_init(void * arg, const char* usb_status)
{
    LOGI("USB Status: %s", usb_status);
    arg = 0;
    Q_UNUSED(arg);
    return 0;
}

void BromUSBStatusArg::FillDefaultValue()
{
     this->flashtool_usb_stat_arg_.cb_usb_status_init_arg = NULL;
     this->flashtool_usb_stat_arg_.m_cb_usb_status_init = cb_checkusbstatus_init;
}

//Watch Dog Argment
BromWatchDogArg::BromWatchDogArg()
{
      FillDefaultValue();
}

BromWatchDogArg:: BromWatchDogArg(const FlashTool_EnableWDT_Arg &wdt_arg)
{
     flashtool_enablewdt_arg_.m_async = wdt_arg.m_async;
     flashtool_enablewdt_arg_.m_reboot = wdt_arg.m_reboot;
     flashtool_enablewdt_arg_.m_timeout_ms = wdt_arg.m_timeout_ms;
}

void BromWatchDogArg::FillDefaultValue()
{

     memset(&flashtool_enablewdt_arg_, 0, sizeof(flashtool_enablewdt_arg_));
     flashtool_enablewdt_arg_.m_timeout_ms = 5000;
     flashtool_enablewdt_arg_.m_async = _FALSE;
     flashtool_enablewdt_arg_.m_reboot = _TRUE;
}

void BromWatchDogArg::Dump()
{
     LOG("m_async(%d).", flashtool_enablewdt_arg_.m_async);
     LOG("m_reboot(%d).", flashtool_enablewdt_arg_.m_reboot);
     LOG("m_timeout_ms(%d).", flashtool_enablewdt_arg_.m_timeout_ms);
}

