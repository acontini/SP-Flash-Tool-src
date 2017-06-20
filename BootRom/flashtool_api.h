/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2006
 *
 ******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  flashtool_api.h
 *
 * Project:
 * --------
 *  Flash Download/Readback/Format Library.
 *
 * Description:
 * ------------
 *  Exported C interface APIs for FlashTool Library.
 *
 * Author:
 * -------
 *  Amos Hsu (mtk00539)
 *
 *******************************************************************************/

#ifndef _FLASHTOOL_API_H_
#define _FLASHTOOL_API_H_

#include "mtk_status.h"
#include "brom.h"
#include "DOWNLOAD.H"
#include "flashtool_handle.h"
#include "external_memory_config.h"
#include "flashtoolex_struct.h"

#ifdef  __cplusplus
extern "C" {
#endif

struct FLASHTOOL_API_HANDLE;
typedef struct FLASHTOOL_API_HANDLE *   FLASHTOOL_API_HANDLE_T;


//------------------------------------------------------------------------------
// accuracy enum
//------------------------------------------------------------------------------
typedef enum {
    ACCURACY_AUTO       = 0,        // auto detect by baudrate
    ACCURACY_1_3        = 3,        //   33%
    ACCURACY_1_4        = 4,        //   25%
    ACCURACY_1_10       = 10,       //   10%
    ACCURACY_1_100      = 100,      //    1%
    ACCURACY_1_1000     = 1000,     //  0.1%
    ACCURACY_1_10000    = 10000     // 0.01%
}ACCURACY;

//------------------------------------------------------------------------------
// com port setting
//------------------------------------------------------------------------------

#ifndef COM_DEFAULT_TIMEOUT
#define COM_DEFAULT_TIMEOUT     0xFFFFFFF
#endif
#define COM_NO_TIMEOUT          0

//------------------------------------------------------------------------------
// COM_PORT structure
//------------------------------------------------------------------------------
typedef enum {
    COM_TYPE_NUM    = 0,    //!<: The COM port is provided by com port number
    COM_TYPE_HANDLE = 1,    //!<: The COM port is provided by com port handle
    COM_TYPE_NAME   = 2,    //!<: The COM port is provided by com port name
} COM_TYPE;

typedef struct {
    COM_TYPE            type;
    union {
        unsigned int    number;
        HANDLE          handle;
        const char*     name;
    };
} COM_PORT_HANDLE;

typedef struct {
    COM_PORT_HANDLE com;
    UART_BAUDRATE   baudrate;
    unsigned int    ms_read_timeout;
    unsigned int    ms_write_timeout;
} COM_PORT_SETTING;

#define INIT_COM_PORT_HANDLE_BY_NUM(com, num) \
    do { com.type = COM_TYPE_NUM;    com.number = num; } while(0)
#define INIT_COM_PORT_HANDLE_BY_HANDLE(com, hCOM) \
    do { com.type = COM_TYPE_HANDLE; com.handle = hCOM; } while(0)
#define INIT_COM_PORT_HANDLE_BY_NAME(com, str) \
    do { com.type = COM_TYPE_NAME;   com.name = str; } while(0)
//------------------------------------------------------------------------------
// NvRAM report structure
//------------------------------------------------------------------------------
typedef struct {
    U64 m_nvram_start_addr;
    U64 m_nvram_size;
}NVRAM_REPORT_T;

//------------------------------------------------------------------------------
// DA report structure
//------------------------------------------------------------------------------
typedef struct DA_REPORT_T {
    // DA version
    unsigned char       m_expected_da_major_ver;
    unsigned char       m_expected_da_minor_ver;
    unsigned char       m_da_major_ver;
    unsigned char       m_da_minor_ver;
    // bbchip
    BBCHIP_TYPE         m_bbchip_type;
    char                m_bbchip_name[32];
    unsigned short      m_bbchip_hw_ver;
    unsigned short      m_bbchip_sw_ver;
    unsigned short      m_bbchip_hw_code;
    EXT_CLOCK           m_ext_clock;
    unsigned char       m_bbchip_secure_ver;
    unsigned char       m_bbchip_bl_ver;
    // NOR flash report
    STATUS_E            m_nor_ret;
    HW_ChipSelect_E     m_nor_chip_select[2];
    unsigned short      m_nor_flash_id;
    unsigned int        m_nor_flash_size;
    unsigned short      m_nor_flash_dev_code_1;
    unsigned short      m_nor_flash_dev_code_2;
    unsigned short      m_nor_flash_dev_code_3;
    unsigned short      m_nor_flash_dev_code_4;
    STATUS_E            m_nor_flash_otp_status;
    unsigned int        m_nor_flash_otp_size;
    // NAND flash report
    STATUS_E            m_nand_ret;
    HW_ChipSelect_E     m_nand_chip_select;
    unsigned short      m_nand_flash_id;
    U64                 m_nand_flash_size;
    unsigned short      m_nand_id_count;
    unsigned short      m_nand_flash_dev_code[16];
    unsigned short      m_nand_pagesize;
    unsigned short      m_nand_sparesize;
    unsigned short      m_nand_pages_per_block;
    unsigned char       m_nand_io_interface;
    unsigned char       m_nand_addr_cycle;
    unsigned char       m_nand_bmt_exist;
    // internal sram report
    STATUS_E            m_int_sram_ret;
    unsigned int        m_int_sram_size;
    // external ram report
    STATUS_E            m_ext_ram_ret;
    HW_RAMType_E        m_ext_ram_type;
    HW_ChipSelect_E     m_ext_ram_chip_select;
   U64       m_ext_ram_size;

    // MTK Security Platform error code
    unsigned char       m_msp_err_code;

    // CODE area and SEC_RO area download status
    STATUS_E            m_download_status;

    // target boot-up style
    STATUS_E            m_boot_style;

    // target fw version
    unsigned int        m_fw_ver_len;
    char                m_fw_ver[64];

    // target random id
    char                m_random_id[16];

    // current selected DL_HANDLE
    DL_HANDLE_T         m_dl_handle;

    // EMMC flash report
    STATUS_E            m_emmc_ret;
    U64        m_emmc_boot1_size;
    U64        m_emmc_boot2_size;
    U64        m_emmc_rpmb_size;
    U64        m_emmc_gp1_size;
    U64        m_emmc_gp2_size;
    U64        m_emmc_gp3_size;
    U64        m_emmc_gp4_size;
    U64                 m_emmc_ua_size;
    U32                 m_emmc_cid[4];
    U8                   m_emmc_fwver[8];

    // SDMMC flash report
    STATUS_E            m_sdmmc_ret;
    U64                 m_sdmmc_ua_size;
    U32                 m_sdmmc_cid[4];

    // UFS flash report
    STATUS_E            m_ufs_ret;
    U64                 m_ufs_lu0_size;
    U64                 m_ufs_lu1_size;
    U64                 m_ufs_lu2_size;
    U64                 m_ufs_lu3_size;
    U32                 m_ufs_cid[4];

} DA_REPORT_T;

//------------------------------------------------------------------------------
// prototype of callback function
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_DA_REPORT)(const DA_REPORT_T  *p_da_report, void *usr_arg);
typedef int (__stdcall *CALLBACK_SET_HIGHSPEED_BAUDRATE)(HANDLE hCOM, unsigned char BaudrateId, void *usr_arg);
typedef int (__stdcall *CALLBACK_DOWNLOAD_PROGRESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_DOWNLOAD_PROGRESS)(unsigned char finished_percentage, U64 finished_bytes, U64 total_bytes, void *usr_arg);
typedef int (__stdcall *CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS)(unsigned char finished_percentage, U64 finished_bytes, U64 total_bytes, void *usr_arg);
typedef int (__stdcall *CALLBACK_CHECKSUM_PROGRESS_INIT)(void *usr_arg, const char* image_name);
typedef int (__stdcall *CALLBACK_CHECKSUM_PROGRESS)(unsigned char finished_percentage, U64 finished_bytes, U64 total_bytes, void *usr_arg);
typedef int (__stdcall *CALLBACK_READBACK_PROGRESS_INIT)(HW_StorageType_E  storage_type, U64 rb_addr, U64 rb_length, const char *rb_filepath, void *usr_arg);
typedef int (__stdcall *CALLBACK_READBACK_PROGRESS)(unsigned char finished_percentage, U64 finished_bytes, U64 total_bytes, void *usr_arg);
typedef int (__stdcall *CALLBACK_FORMAT_PROGRESS_INIT)(HW_StorageType_E  storage_type, U64 begin_addr, U64 length, void *usr_arg);
typedef int (__stdcall *CALLBACK_FORMAT_PROGRESS)(unsigned char finished_percentage, void *usr_arg);
typedef struct {
    U64    m_fmt_begin_addr;
    U64    m_fmt_length;
    unsigned int    m_total_blocks;
    unsigned int    m_bad_blocks;
    unsigned int    m_err_blocks;
} FormatStatisticsReport_T;
typedef int (__stdcall *CALLBACK_FORMAT_STATISTICS)(const FormatStatisticsReport_T  *p_report, void *usr_arg);
typedef int (__stdcall *CALLBACK_POST_PROCESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_POST_PROCESS)(unsigned char finished_percentage, void *usr_arg);
typedef int (__stdcall *CALLBACK_SECURITY_PRE_PROCESS_NOTIFY)(void *usr_arg);
typedef int (__stdcall *CALLBACK_SECURITY_POST_PROCESS_NOTIFY)(void *usr_arg);

typedef int (__stdcall *CALLBACK_PROGRESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_PROGRESS)(unsigned char finished_percentage, U64 finished_bytes, U64 total_bytes, void *usr_arg);

typedef int (__stdcall *CALLBACK_MEMORYTEST_PROGRESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_MEMORYTEST_PROGRESS)(unsigned int current_progress, U64 finished_bytes, U64 total_bytes, void *usr_arg);
typedef int (__stdcall *CALLBACK_MEMORYTEST_NAND_BAD_BLOCK_REPORT)(U64 bad_block_address, void *usr_arg);

typedef CALLBACK_READBACK_PROGRESS CALLBACK_READ_FLASH_PROGRESS;
typedef CALLBACK_DOWNLOAD_PROGRESS CALLBACK_WRITE_FLASH_PROGRESS;


LIBEXPORT int __stdcall FlashTool_LoadConfigFile(
                            ExternalMemoryConfig *external_memory_config,
                            const char *config_file);

//------------------------------------------------------------------------------
// Connect/Disconnect
//------------------------------------------------------------------------------
typedef enum {
	FIRST_DA = 1,
	SECOND_DA = 2
}CONN_DA_END_STAGE;

typedef enum {
	DA_LOG_LEVEL_TRACE = 0,
	DA_LOG_LEVEL_DEBUG = 1,
	DA_LOG_LEVEL_INFO = 2,
	DA_LOG_LEVEL_WARNING = 3,
	DA_LOG_LEVEL_ERROR = 4,
	DA_LOG_LEVEL_FATAL = 5
}DA_LOG_LEVEL_E;

typedef struct {

    // com port timeout setting
    unsigned int                            m_com_ms_read_timeout;
    unsigned int                            m_com_ms_write_timeout;

    // BootFlashTool arg
    BOOT_FLASHTOOL_ARG                      m_boot_arg;

    // security pre-process notify callback
    CALLBACK_SECURITY_PRE_PROCESS_NOTIFY    m_cb_security_pre_process_notify;
    void                                    *m_cb_security_pre_process_notify_arg;

    // chip-select of NOR flash memory
    HW_ChipSelect_E                         m_nor_chip_select[2];

    // chip-select of NAND flash memory
    HW_ChipSelect_E                         m_nand_chip_select;

    // NFI access control register
    unsigned int                            *m_p_nand_acccon;

    //By HW_StorageType_E to map different storage type operations
    HW_StorageType_E                        m_storage_type;

    // 2011-11-08: Add DL_HANDLE attribute for dummy partition layout setting
    DL_HANDLE_T                             m_p_dl_handle;

	unsigned char                           m_force_charge;	// value of 'FC_TYPE'

#define RESET_BY_PWR_KEY_ALONE	0x50	// 'P'
#define RESET_BY_PWR_HOME_KEY	0x68	// 'h' - default(everything other than 'P' means 'h')
	unsigned char                           m_reset_key; // power key reset, or power+home key reset

	//add for DRAM flip test set connect which DA, 1st or 2nd
	CONN_DA_END_STAGE						m_conn_da_end_stage;
	_BOOL									m_1st_da_enable_dram;

	DA_LOG_LEVEL_E                          m_da_log_level;
} FlashTool_Connect_Arg;

typedef struct {

    // DA report
    DA_REPORT_T         m_da_report;

} FlashTool_Connect_Result;

_DEPRECATED_API int __stdcall FlashTool_NandUtil_Connect(
                        unsigned char  com_port,
                        const FlashTool_Connect_Arg  *p_arg,
                        FlashTool_Connect_Result  *p_result,
                        int *p_stopflag,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle);


LIBEXPORT int __stdcall FlashTool_Connect(
                        unsigned char  com_port,
                        const FlashTool_Connect_Arg  *p_arg,
                        FlashTool_Connect_Result  *p_result,
                        const ExternalMemoryConfig      *p_external_memory_config,
                        int *p_stopflag,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        _BOOL bCheckScatter = _TRUE);

LIBEXPORT int __stdcall FlashTool_Connect_ByhCOM(
                        HANDLE  hCOM,
                        const FlashTool_Connect_Arg  *p_arg,
                        FlashTool_Connect_Result  *p_result,
                        const ExternalMemoryConfig      *p_external_memory_config,
                        int *p_stopflag,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        _BOOL bCheckScatter = _TRUE);

LIBEXPORT int __stdcall FlashTool_Connect_ByName(
                        const char* name,
                        const FlashTool_Connect_Arg  *p_arg,
                        FlashTool_Connect_Result  *p_result,
                        const ExternalMemoryConfig      *p_external_memory_config,
                        int *p_stopflag,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        _BOOL bCheckScatter = _TRUE);

LIBEXPORT int __stdcall FlashTool_CloseComport(FLASHTOOL_API_HANDLE_T  p_ft_handle);

LIBEXPORT int __stdcall FlashTool_Connect_Ex(
                        COM_PORT_HANDLE *p_com_port,
                        const FlashTool_Connect_Arg  *p_arg,
                        FlashTool_Connect_Result  *p_result,
                        const ExternalMemoryConfig      *p_external_memory_config,
                        int *p_stopflag,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        _BOOL bCheckScatter = _TRUE);

LIBEXPORT int __stdcall FlashTool_IsConnectWithBootRom(
                        const FLASHTOOL_API_HANDLE_T  p_ft_handle,
                        _BOOL *p_is_connect_with_bootrom);

/* *
 * ENABLE_DA_FINISH_CMD: USB device will disappear in device manger after FlashTool_Disconnect
 * DISABLE_DA_FINISH_CMD: USB device will NOT disappear in device manger after FlashTool_Disconnect
 * */
typedef enum {
    ENABLE_DA_FINISH_CMD = 0,
    DISABLE_DA_FINISH_CMD = 1,
}DAFinishCMD_E;
LIBEXPORT int __stdcall FlashTool_EnableDAFinishCMD(const FLASHTOOL_API_HANDLE_T  p_ft_handle,
        const DAFinishCMD_E da_finish_cmd_status);
/* Add for controlling USB and device power before disconnection, 2012/08/02 { */
LIBEXPORT int __stdcall FlashTool_Rehabilitate(const FLASHTOOL_API_HANDLE_T  p_ft_handle,
        const DEVICE_STATUS device_status);
/* Add for controlling USB and device power before disconnection, 2012/08/02 } */
LIBEXPORT int __stdcall FlashTool_Disconnect(FLASHTOOL_API_HANDLE_T  *p_ft_handle);
LIBEXPORT int __stdcall FlashTool_Disconnect_BROM(FLASHTOOL_API_HANDLE_T  *p_ft_handle);

LIBEXPORT int __stdcall FlashTool_ChangeCOM(FLASHTOOL_API_HANDLE_T *p_ft_handle, unsigned char com_port);
LIBEXPORT int __stdcall FlashTool_ChangeCOM_Ex(FLASHTOOL_API_HANDLE_T *p_ft_handle, COM_PORT_HANDLE *p_com_port);
LIBEXPORT int __stdcall FlashTool_SetupUSBDL(FLASHTOOL_API_HANDLE_T *p_ft_handle, unsigned char enable_hs);

/* Add for setup USB comport, 2013/01/16 { */
typedef struct{
    U8  enable_hs;
    U16 pid;
    U16 vid;
} USB_Setup_Info;
LIBEXPORT int __stdcall FlashTool_SetupUSBDL_EX(FLASHTOOL_API_HANDLE_T *p_ft_handle,
        const USB_Setup_Info *p_usb_setup_info);
/* Add for setup USB comport, 2013/01/16}*/

/* Add for checking USB status, 2012/08/08 { */
typedef int (__stdcall *CALLBACK_USB_STATUS_INIT)(void *usr_arg, const char* usb_status);
typedef enum {
    USB_FULL_SPEED = 0,
    USB_HIGH_SPEED = 1,
    USB_ULTRA_HIGH_SPEED = 2,
    USB_STATUS_UNKNOWN = 0xFFFF
}USB_SPEED_STATUS_E;
typedef struct {
    CALLBACK_USB_STATUS_INIT m_cb_usb_status_init;
    void *cb_usb_status_init_arg;
}FlashTool_USB_Status_Arg;
typedef struct {
    USB_SPEED_STATUS_E usb_speed_status;
}FlashTool_USB_Status_Result;
LIBEXPORT int __stdcall FlashTool_CheckUSBStatus(const FLASHTOOL_API_HANDLE_T p_ft_handle,
    const FlashTool_USB_Status_Arg *p_arg,
    FlashTool_USB_Status_Result *p_result);
LIBEXPORT const char* __stdcall USBSpeedStatusToString(USB_SPEED_STATUS_E usb_status);
/* Add for checking USB status, 2012/08/08 }*/
//------------------------------------------------------------------------------
// Post-Process
//------------------------------------------------------------------------------
typedef struct {

    // post-process callback
    CALLBACK_POST_PROCESS_INIT          m_cb_post_process_init;
    void *                              m_cb_post_process_init_arg;
    CALLBACK_POST_PROCESS               m_cb_post_process;
    void *                              m_cb_post_process_arg;

} FlashTool_PostProcess_Arg;

LIBEXPORT int __stdcall FlashTool_PostProcess(FLASHTOOL_API_HANDLE_T  ft_handle, const FlashTool_PostProcess_Arg *p_arg);

//------------------------------------------------------------------------------
// Change Baudrate
//------------------------------------------------------------------------------
typedef struct {

    UART_BAUDRATE   m_baudrate;
    unsigned char   m_baudrate_full_sync_count;

    // set high-speed baudrate callback
    CALLBACK_SET_HIGHSPEED_BAUDRATE         m_cb_set_high_speed_baudrate;
    void *                                  m_cb_set_high_speed_baudrate_arg;

} FlashTool_ChangeBaudrate_Arg;

LIBEXPORT int __stdcall FlashTool_ChangeBaudrate(FLASHTOOL_API_HANDLE_T  ft_handle, const FlashTool_ChangeBaudrate_Arg  *p_arg);

//------------------------------------------------------------------------------
// Write USB download control bit
//------------------------------------------------------------------------------
_DEPRECATED_API int __stdcall FlashTool_WriteUSBDownloadControlBit(FLASHTOOL_API_HANDLE_T ft_handle);

//------------------------------------------------------------------------------
// Enable WatchDog Timeout
//------------------------------------------------------------------------------
typedef struct{
    unsigned int m_timeout_ms;
    _BOOL        m_async;       //!< 1: disconnect before watchdog timeout; 0: disconnect after watchdog timeout
    _BOOL        m_reboot;      //!< 1: enter charging after download; 0: auto boot up after download;
    /* 1: Make PreLoader enter download mode without timeout;
     * 0: PreLoader with default timeout for download */
    _BOOL        m_dlbit;
    _BOOL        m_bNotResetRTCTime;    //!<:1: not reset RTC time; 0: reset RTC time if have formated flash before enable watchdog
} FlashTool_EnableWDT_Arg;

LIBEXPORT int __stdcall FlashTool_EnableWatchDogTimeout(FLASHTOOL_API_HANDLE_T  ft_handle, const FlashTool_EnableWDT_Arg *p_arg);
//------------------------------------------------------------------------------
// Enable DRAM
//------------------------------------------------------------------------------

typedef struct {

    // external ram report
    STATUS_E            m_ext_ram_ret;
    HW_RAMType_E        m_ext_ram_type;
    HW_ChipSelect_E     m_ext_ram_chip_select;
    U64        m_ext_ram_size;

} FlashTool_EnableDRAM_Result;

LIBEXPORT int __stdcall FlashTool_EnableDRAM(
                     FLASHTOOL_API_HANDLE_T  ft_handle,
                     const DRAM_SETTING  dram_setting,
                     FlashTool_EnableDRAM_Result    *p_dram_result);

//------------------------------------------------------------------------------
// Decrypt Image
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall FlashTool_DecryptImage_Config(
                    FLASHTOOL_API_HANDLE_T ft_handle,
                    IMGDEC_HANDLE_T imgdec_handle);

//------------------------------------------------------------------------------
// Decrypt INI File
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall GetCipherHeaderLen(void);

LIBEXPORT int __stdcall FlashTool_DecryptINI(
                    unsigned char* FileBuf,
                    unsigned char* AESKey,
                    unsigned int KeyLen);

//------------------------------------------------------------------------------
// Download
//------------------------------------------------------------------------------
typedef struct {
    // single DL_HANDLE
    DL_HANDLE_T                     m_dl_handle;

    // multi DL_HANDLE List
    DL_HANDLE_LIST_T                m_dl_handle_list;

    // da report callback
    CALLBACK_DA_REPORT                  m_cb_da_report;
    void *                              m_cb_da_report_arg;

    // DL_HANDLE download progress callback
    ACCURACY                            m_download_accuracy;
    CALLBACK_DOWNLOAD_PROGRESS_INIT     m_cb_download_flash_init;
    void *                              m_cb_download_flash_init_arg;
    CALLBACK_DOWNLOAD_PROGRESS          m_cb_download_flash;
    void *                              m_cb_download_flash_arg;

    // Boot Loader download progress callback
    CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS_INIT  m_cb_download_bloader_init;
    void *                                      m_cb_download_bloader_init_arg;
    CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS       m_cb_download_bloader;
    void *                                      m_cb_download_bloader_arg;

    // security post-process notify callback
    CALLBACK_SECURITY_POST_PROCESS_NOTIFY       m_cb_security_post_process_notify;
    void *                                      m_cb_security_post_process_notify_arg;

    // Checksum progress callback
    CALLBACK_CHECKSUM_PROGRESS_INIT             m_cb_checksum_init;
    void *                                      m_cb_checksum_init_arg;
    CALLBACK_CHECKSUM_PROGRESS                  m_cb_checksum;
    void *                                      m_cb_checksum_arg;

    // The flag of m_enable_tgt_res_layout_check is used to control whether if target resource
    // layout checking operation will be performed.
    // _TRUE: Enable target resource layout checking operation.
    // _FALSE: Disable target resource layout checking operation.
    _BOOL               m_enable_tgt_res_layout_check;

    // The flag to check if target side baseband chip version is corresponding to ROM file on PC side.
    _BOOL               m_enable_bbchip_ver_check;

    // Download Style : _FALSE  : best effort erase
    //                  _TRUE   : sequential erase (sector by sector)
    _BOOL               m_downloadstyle_sequential;

} FlashTool_Download_Arg;

typedef struct {

    // dummy

} FlashTool_Download_Result;

LIBEXPORT int __stdcall FlashTool_Download(
                        FLASHTOOL_API_HANDLE_T  ft_handle,
                        const FlashTool_Download_Arg  *p_dl_arg,
                        FlashTool_Download_Result  *p_dl_result);

//------------------------------------------------------------------------------
// Readback
//------------------------------------------------------------------------------
typedef struct {

    HW_StorageType_E                    m_storage_type;

    RB_HANDLE_T                         m_rb_handle;

    // readback progress callback
    ACCURACY                            m_readback_accuracy;
    CALLBACK_READBACK_PROGRESS_INIT     m_cb_readback_flash_init;
    void *                              m_cb_readback_flash_init_arg;
    CALLBACK_READBACK_PROGRESS          m_cb_readback_flash;
    void *                              m_cb_readback_flash_arg;

} FlashTool_Readback_Arg;

typedef struct {

    // dummy

} FlashTool_Readback_Result;

LIBEXPORT int __stdcall FlashTool_Readback(
                        FLASHTOOL_API_HANDLE_T  ft_handle,
                        const FlashTool_Readback_Arg  *p_rb_arg,
                        FlashTool_Readback_Result  *p_rb_result);

//------------------------------------------------------------------------------
// Format
//------------------------------------------------------------------------------
typedef struct {
    // automatically format the FAT area on flash
    _BOOL               m_auto_format_fat;
    // enable/disable erase validation
    _BOOL               m_validation;
    // begin_addr and length only take effect when option is m_enable_auto_format_fat is _FALSE
    U64        m_format_begin_addr;
    U64        m_format_length;
    unsigned int        m_part_id;
    NUTL_AddrTypeFlag_E m_AddrType;
} FORMAT_CONFIG_T;

typedef struct {

    HW_StorageType_E                    m_storage_type;

    // format config
    FORMAT_CONFIG_T                     m_format_cfg;

    // only take effect on NAND flash
    NUTL_EraseFlag_E                    m_erase_flag;

    // format progress callback
    CALLBACK_FORMAT_PROGRESS_INIT       m_cb_format_report_init;
    void *                              m_cb_format_report_init_arg;
    CALLBACK_FORMAT_PROGRESS            m_cb_format_report;
    void *                              m_cb_format_report_arg;
    // format statistics callback
    CALLBACK_FORMAT_STATISTICS          m_cb_format_statistics;
    void *                              m_cb_format_statistics_arg;

} FlashTool_Format_Arg;

typedef struct {

    FormatStatisticsReport_T            m_format_statistics;

} FlashTool_Format_Result;

LIBEXPORT int __stdcall FlashTool_Format(
                        FLASHTOOL_API_HANDLE_T  ft_handle,
                        const FlashTool_Format_Arg  *p_fmt_arg,
                        FlashTool_Format_Result  *p_fmt_result);

//------------------------------------------------------------------------------
// Flash read/write
//------------------------------------------------------------------------------
typedef enum
{
    AddressingMode_BlockIndex = 0,
    AddressingMode_PhysicalAddress,
    AddressingMode_LogicalAddress,
} AddressingMode;

typedef enum
{
    OutputMode_ToBuffer = 0,
    OutputMode_ToFile
} OutputMode;

typedef enum
{
    ProgramMode_PageOnly = 0,
    ProgramMode_PageSpare,

} ProgramMode;

typedef struct
{
    HW_StorageType_E m_flash_type;
    AddressingMode m_addressing_mode;
    U64 m_address;
    U64 m_length;
    unsigned int m_container_length; // In blocks, only valid for NAND
    OutputMode m_output_mode;
    char *m_output;

    CALLBACK_READ_FLASH_PROGRESS m_cb_progress;
    void *m_cb_progress_arg;
} ReadFlashMemoryParameter;

typedef struct
{
    U64 m_num_bytes_read;
} ReadFlashMemoryResult;

typedef enum
{
    InputMode_FromBuffer = 0,
    InputMode_FromFile
} InputMode;

typedef struct
{
    HW_StorageType_E m_flash_type;
    AddressingMode m_addressing_mode;
    U64 m_address;
    unsigned int m_container_length; // In blocks, only valid for NAND
    InputMode m_input_mode;
    const char *m_input;
    U64 m_input_length;
    unsigned int m_part_id;

    CALLBACK_WRITE_FLASH_PROGRESS m_cb_progress;
    void *m_cb_progress_arg;

    ProgramMode m_program_mode;
} WriteFlashMemoryParameter;

LIBEXPORT int __stdcall FlashTool_ReadFlashMemory(
                            FLASHTOOL_API_HANDLE_T ft_handle,
                            const ReadFlashMemoryParameter *parameter,
                            ReadFlashMemoryResult *result);

LIBEXPORT int __stdcall FlashTool_WriteFlashMemory(
                            FLASHTOOL_API_HANDLE_T ft_handle,
                            const WriteFlashMemoryParameter *parameter);

//------------------------------------------------------------------------------
// Retrieve FAT ranges
//------------------------------------------------------------------------------
typedef struct
{
    unsigned int m_start_address;
    unsigned int m_length;    // 0 for an invalid range
} Range;

_DEPRECATED_API int __stdcall FlashTool_GetFATRanges(FLASHTOOL_API_HANDLE_T ft_handle,
                                            Range *nor_fat_range,
                                            Range *nand_fat_range);

//------------------------------------------------------------------------------
// SEC_RO read/write
//------------------------------------------------------------------------------
typedef struct {
    unsigned int        m_addr;
    unsigned int        m_len;
} SecRO_Info_T;

typedef struct {
    unsigned int        m_len;
    unsigned char *     m_data;
} SecRO_T;

_DEPRECATED_API int __stdcall FlashTool_SecRO_GetInfo(FLASHTOOL_API_HANDLE_T, SecRO_Info_T*);
_DEPRECATED_API int __stdcall FlashTool_SecRO_Read(FLASHTOOL_API_HANDLE_T, SecRO_T*);
_DEPRECATED_API int __stdcall FlashTool_SecRO_Write(FLASHTOOL_API_HANDLE_T, const SecRO_T*);

//------------------------------------------------------------------------------
// CustPara read/write
//------------------------------------------------------------------------------
typedef struct {
    unsigned int        m_addr;
    unsigned int        m_len;
} CustPara_Info_T;

typedef struct {
    unsigned int        m_len;
    unsigned char *     m_data;
} CustPara_T;

_DEPRECATED_API int __stdcall FlashTool_CustPara_GetInfo(FLASHTOOL_API_HANDLE_T, CustPara_Info_T*);
_DEPRECATED_API int __stdcall FlashTool_CustPara_Read(FLASHTOOL_API_HANDLE_T, CustPara_T*);
_DEPRECATED_API int __stdcall FlashTool_CustPara_Write(FLASHTOOL_API_HANDLE_T, const CustPara_T*);

//------------------------------------------------------------------------------
// Update FlashTool Config
//------------------------------------------------------------------------------
typedef struct {
    unsigned int        m_lockdown:1;
    unsigned int        m_disable_code_download:1;
    unsigned int        m_disable_code_readback:1;
    unsigned int        m_disable_code_format:1;
    unsigned int        m_disable_sec_ro_download:1;
    unsigned int        m_disable_sec_ro_readback:1;
    unsigned int        m_disable_fat_download:1;
    unsigned int        m_disable_fat_readback:1;
    unsigned int        m_disable_fat_format:1;
    unsigned int        m_restricted_area_access_forbidden:1;
} SecurityBitCtrl_T;

typedef union {
    SecurityBitCtrl_T   m_bit_ctrl;
    unsigned int        m_uint32;
} SecurityCfg_U;

typedef struct {
    SecurityCfg_U       m_security_cfg;
} FlashToolCfg_T;

LIBEXPORT int __stdcall FlashTool_UpdateConfig(FLASHTOOL_API_HANDLE_T  ft_handle, const FlashToolCfg_T  *p_ft_cfg);

//------------------------------------------------------------------------------
// Get Project ID
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall FlashTool_GetProjectId(FLASHTOOL_API_HANDLE_T  ft_handle, char *output_buf, unsigned int output_buf_len);
//------------------------------------------------------------------------------
// Get Boot Loader Security Version
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall FlashTool_GetPLSecVersion(FLASHTOOL_API_HANDLE_T  ft_handle, unsigned int *bl_sec_version);

// Get DL handle for adapter
LIBEXPORT int __stdcall FlashTool_GetDLHandle(FLASHTOOL_API_HANDLE_T ft_handle, DL_HANDLE_T *dl_handle);
//------------------------------------------------------------------------------
// OTP read/write
//------------------------------------------------------------------------------
typedef enum {
     PARAM_READ = 0
    ,PARAM_WRITE
    ,PARAM_OPERATION_END
} PARAM_OPERATION;

typedef enum {
     OTP_READ = 0
    ,OTP_WRITE
    ,OTP_LOCK
    ,OTP_WRITE_AND_LOCK
    ,OTP_OPERATION_END
} OTP_OPERATION;

typedef struct {
    U64                 m_addr;
    unsigned int        m_len;
    unsigned char *     m_data;
} OTP_T;

LIBEXPORT int __stdcall FlashTool_OTP_CheckDevice(FLASHTOOL_API_HANDLE_T  ft_handle);
LIBEXPORT int __stdcall FlashTool_OTP_GetSize(FLASHTOOL_API_HANDLE_T  ft_handle, unsigned int * otp_size);
LIBEXPORT int __stdcall FlashTool_OTP_Read(FLASHTOOL_API_HANDLE_T  ft_handle, OTP_T * p_otp);
LIBEXPORT int __stdcall FlashTool_OTP_Write(FLASHTOOL_API_HANDLE_T  ft_handle, const OTP_T * p_otp);
LIBEXPORT int __stdcall FlashTool_OTP_Lock(FLASHTOOL_API_HANDLE_T  ft_handle);
LIBEXPORT int __stdcall FlashTool_OTP_LockCheckStatus(FLASHTOOL_API_HANDLE_T  ft_handle);
LIBEXPORT int __stdcall FlashTool_SetOTPAddress(FLASHTOOL_API_HANDLE_T  ft_handle,U64 uOTPBeginAddress);

//------------------------------------------------------------------------------
// Repair Tool : Exception Record Retriever
//------------------------------------------------------------------------------
_DEPRECATED_API int __stdcall RepairTool_GetDIRInfo( FLASHTOOL_API_HANDLE_T  ft_handle, char * fn_dir);
_DEPRECATED_API int __stdcall RepairTool_GetExceptionRecord( FLASHTOOL_API_HANDLE_T  ft_handle, char * fn_exceptionlog, char * fn_store);

//------------------------------------------------------------------------------
// Memory Test Tool : RAM/Flash Stress Test and Addr/Data Bus Test
//------------------------------------------------------------------------------
#define MEMORYTEST_STOP     9876

typedef struct {
    HW_MemoryType_E         m_memory_device;
    U64            			m_start_addr;
    U64					m_size;
    HW_MemoryIO_E           m_memory_io;
    HW_MemoryTestMethod_E   m_test_method;
    unsigned int            m_test_pattern;

    // callback function
    CALLBACK_MEMORYTEST_PROGRESS_INIT         m_cb_memorytest_progress_init;
    void *                                    m_cb_memorytest_progress_init_arg;
    CALLBACK_MEMORYTEST_PROGRESS              m_cb_memorytest_progress;
    void *                                    m_cb_memorytest_progress_arg;
    CALLBACK_MEMORYTEST_NAND_BAD_BLOCK_REPORT m_cb_memorytest_nand_bad_block_report;
    void *                                    m_cb_memorytest_nand_bad_block_report_arg;

    // stop flag
    int *                   m_p_stopflag;
} FlashTool_MemoryTest_Arg;

typedef struct {
    // return value
    unsigned int            m_ret;
    // Error result for Pattern Test and Inc/Dec Test
    unsigned int            m_pass_pattern;
    U64           		     m_fail_addr;
    unsigned int            m_fail_pattern;
    // Error result for Data Bus Test, Address Bus Test, and I/O Bus Test
    unsigned int            m_fail_value;
    unsigned int            m_fail_pin;
    unsigned int            m_fail_pin2;
    // Memory dump value for HW_MEM_DUMP
    unsigned char           *m_dump_buf;

    unsigned int            m_num_bad_blocks; // Only valid for NAND test
} FlashTool_MemoryTest_Result;

LIBEXPORT int __stdcall FlashTool_MemoryTest( FLASHTOOL_API_HANDLE_T  ft_handle, const FlashTool_MemoryTest_Arg *p_mt_arg, FlashTool_MemoryTest_Result * p_mt_result);


typedef struct {
    // post-process callback
    CALLBACK_PROGRESS_INIT              m_cb_progress_init;
    void *                              m_cb_progress_init_arg;
    CALLBACK_PROGRESS                   m_cb_progress;
    void *                              m_cb_progress_arg;
} FlashTool_Progress_Arg;

typedef struct {
    // dummy
} FlashTool_Progress_RESULT;

_DEPRECATED_API int __stdcall FlashTool_ExtractNFB(
	FLASHTOOL_API_HANDLE_T  ft_handle,
	char * fn_store,
	const FlashTool_Progress_Arg  *p_pgs_arg);

_DEPRECATED_API int __stdcall FlashTool_InjectNFB(
	FLASHTOOL_API_HANDLE_T  ft_handle,
	char * fn_store,
	const FlashTool_Progress_Arg  *p_pgs_arg);

//------------------------------------------------------------------------------
// DA Relay Mode for GPS Project
//------------------------------------------------------------------------------
typedef struct {

    unsigned int        m_power_pin;
    unsigned int        m_reset_pin;
    unsigned int        m_standby_pin;
    unsigned int        m_32k_pin;
} GPIO_GPS_SETTING;

LIBEXPORT int __stdcall FlashTool_RelayDA(
	FLASHTOOL_API_HANDLE_T  ft_handle,
	const GPIO_GPS_SETTING *p_gpio_gps);

LIBEXPORT int __stdcall FlashTool_ReadPartitionCount(
	FLASHTOOL_API_HANDLE_T ft_handle,
	unsigned int *p_part_count);
LIBEXPORT int __stdcall FlashTool_ReadPartitionInfo(
	FLASHTOOL_API_HANDLE_T ft_handle,
	PART_INFO *p_part_info,
	unsigned int max_part_count);

// To get total reserved ROM size (PMT+BMT+OPT)
LIBEXPORT int __stdcall FlashTool_GetReservedRomSize(
	FLASHTOOL_API_HANDLE_T	ft_handle,
	DL_HANDLE_T				dl_handle,
	U64 *					p_rsv_size);

// To get info of regulated ROMs from scatter file
LIBEXPORT int __stdcall FlashTool_RomGetCount(
	FLASHTOOL_API_HANDLE_T ft_handle,
	DL_HANDLE_T  dl_handle,
	unsigned int *p_rom_count);
LIBEXPORT int __stdcall FlashTool_RomGetInfoAll(
	FLASHTOOL_API_HANDLE_T ft_handle,
	DL_HANDLE_T  dl_handle,
	ROM_INFO *p_rom_info,
	unsigned int max_rom_count);

//------------------------------------------------------------------------------
// Get IMEI PID SWV information
//------------------------------------------------------------------------------
typedef struct
{
    unsigned char m_uIMEI[IMEI_MAX_LENTH];
    unsigned char m_uPID[PID_MAX_LENTH];
    unsigned char m_uSWV[SWV_MAX_LENTH];

}IMEI_PID_SWV_Info;

LIBEXPORT int __stdcall FlashTool_GetIMEI_PID_SWV_Info(FLASHTOOL_API_HANDLE_T  ft_handle, IMEI_PID_SWV_Info* pProductInfo);

LIBEXPORT int __stdcall FlashTool_Connect_BROM(unsigned int  com_port,
                           const FlashTool_Connect_Arg  *p_arg,
                           FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                           int *p_stopflag,
                           _BOOL bCheckScatter = _TRUE);

LIBEXPORT const char* __stdcall ChipTypeToString(BBCHIP_TYPE chiptype);

LIBEXPORT BBCHIP_TYPE __stdcall BBChipTypeFromBBChipName(const char* bbchip_name);

LIBEXPORT int __stdcall FlashTool_Connect_BROM_Ex(COM_PORT_HANDLE *p_com_port,
                                                  const FlashTool_Connect_Arg *p_arg,
                                                  FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                                                  int *p_stopflag,
                                                  _BOOL bCheckScatter = _TRUE);

LIBEXPORT int __stdcall FlashTool_Connect_BROM_ByhCOM(
                        HANDLE  hCOM,
                        const FlashTool_Connect_Arg  *p_arg,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        int *p_stopflag,
                        _BOOL bCheckScatter = _TRUE);

LIBEXPORT int __stdcall FlashTool_Connect_BROM_ByName(
                        const char* name,
                        const FlashTool_Connect_Arg  *p_arg,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        int *p_stopflag,
                        _BOOL bCheckScatter = _TRUE);

// export for adapter
LIBEXPORT int __stdcall FlashTool_Setup_API_Handle(const char* name,
                        const FlashTool_Connect_Arg  *p_arg,
                        FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                        int *p_stopflag);

LIBEXPORT _BOOL __stdcall FlashTool_IsToStop(FLASHTOOL_API_HANDLE_T  ft_handle);

LIBEXPORT const BOOT_RESULT *__stdcall FlashTool_GetBootResult(
	FLASHTOOL_API_HANDLE_T ft_handle);

LIBEXPORT int __stdcall FlashTool_Connect_Download_DA(
                                  const FlashTool_Connect_Arg  *p_arg,
                                  FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                                  FlashTool_Connect_Result  *p_result,
                                  int *p_stopflag,
                                  _BOOL bCheckScatter=_TRUE);
LIBEXPORT int __stdcall FlashTool_Connect_Download_InitDA(
                                  const FlashTool_Connect_Arg  *p_arg,
                                  FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                                  FlashTool_Connect_Result  *p_result,
                                  int *p_stopflag,
                                  _BOOL bCheckScatter=_TRUE);

LIBEXPORT int __stdcall FlashTool_Connect_Download_LoopDA(
                                  const FlashTool_Connect_Arg  *p_arg,
                                  FLASHTOOL_API_HANDLE_T  *p_ft_handle,
                                  FlashTool_Connect_Result  *p_result,
                                  int *p_stopflag);
LIBEXPORT int __stdcall FlashTool_SetDAReport(FLASHTOOL_API_HANDLE_T  *p_ft_handle,
												DA_REPORT_T& da_report);

LIBEXPORT int __stdcall FlashTool_GetDAReport(FLASHTOOL_API_HANDLE_T  *p_ft_handle,
												DA_REPORT_T* p_da_report);


/* Add to check status info of battery, 2012/12/08 {*/
typedef struct {
    //dummy
}FlashTool_CheckBattery_Arg;
typedef struct {
    U32 bat_voltage_value;
}FlashTool_CheckBattery_Result;
LIBEXPORT int __stdcall FlashTool_Check_Battery(const FLASHTOOL_API_HANDLE_T  ft_handle,
        const FlashTool_CheckBattery_Arg  *p_bat_arg,
        FlashTool_CheckBattery_Result  *p_bat_result);
/* Add to check status info of battery, 2012/12/08 }*/

/*Support to check format/download status, 2013/01/19 }*/
typedef struct {
    char   image_name[16];
    uint32 pc_checksum;
    uint32 da_checksum;
    char   checksum_status[8];
    //Forward compatibility
    uint32 image_index;
    uint32 pl_checksum;
} Flash_Checksum_Result;

typedef struct {
    char          magic_num[32];
    char          version[32-DL_MAGIC_OFFSET];
    Flash_Checksum_Result part_info[MAX_LOAD_SECTIONS];
    char          ram_checksum[16];
    char          download_status[16];
} Flash_Status_Result;

LIBEXPORT int __stdcall FlashTool_ReadFlashInfo(const FLASHTOOL_API_HANDLE_T  ft_handle,
        Flash_Status_Result *p_flash_status_result);

typedef struct {
    char custom_dl_info[DL_CUSTOM_INFO_SIZE];
} Flash_DL_Cust_Info;
LIBEXPORT int __stdcall FlashTool_WriteCustFlashInfo(const FLASHTOOL_API_HANDLE_T  ft_handle,
        const Flash_DL_Cust_Info *p_dl_cust_info_arg);
/*Support to check format/download status, 2013/01/19 {*/

//------------------------------------------------------------------------------
// Efsue write Command
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall FlashTool_WriteEfuseAll(
	FLASHTOOL_API_HANDLE_T ft_handle,
	const Efuse_Common_Arg *common_arg,
	const Efuse_Secure_Arg *secure_arg,
        const Efuse_Lock_Arg *lock_arg);

LIBEXPORT int __stdcall FlashTool_WriteEfuseAll_Ex(
        FLASHTOOL_API_HANDLE_T ft_handle,
        const Efuse_Common_Arg *common_arg,
        const Efuse_Secure_Arg *secure_arg,
        const Efuse_Lock_Arg *lock_arg,
        const Efuse_STB_Key_Arg *stb_arg,
        const Efuse_Extra_Arg   *extra_arg);

LIBEXPORT int __stdcall FlashTool_ReadEfuseAll(
    FLASHTOOL_API_HANDLE_T ft_handle,
    Efuse_Common_Arg *common_arg,
    Efuse_Secure_Arg *secure_arg,
    Efuse_Lock_Arg *lock_arg);

LIBEXPORT int __stdcall FlashTool_ReadEfuseAll_Ex(
    FLASHTOOL_API_HANDLE_T ft_handle,
    Efuse_Common_Arg *common_arg,
    Efuse_Secure_Arg *secure_arg,
    Efuse_Lock_Arg *lock_arg,
    Efuse_STB_Key_Arg   *stb_arg,
    Efuse_Extra_Arg     *extra_arg);


/* API: check chip support which modem */
typedef struct {
    bool b2G:        1;//GSM, EDGE
    bool b3G:        1;//TD-3G | HSPA | C2K
    bool b4G:        1;//LTE
    unsigned char RFU: 5;
}ModemType;

typedef enum{
	MODEM_UNKNOWN = 0x0,
	MODEM_1M = 1,
	MODEM_2M = 2,
	MODEM_3M = 3,
	MODEM_4M = 4,
	MODEM_5M = 5,
	MODEM_6M = 6
}ModemType_E;

LIBEXPORT int __stdcall FlashTool_GetSupportModem(
    FLASHTOOL_API_HANDLE_T ft_handle,
    ModemType_E *pModemType
);

#ifdef _WIN32
//------------------------------------------------------------------------------
//  BROM Connection callback functions, only for WINDOWS!!!- by helio
//------------------------------------------------------------------------------
typedef HANDLE (__stdcall *CbFunc_Com_CreateFile)(const char * pPortName, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_CancelIo)(HANDLE hCon, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_CloseHandle)(HANDLE hCon, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_WriteFile)(HANDLE hCon, const void *pBuf, DWORD nNumberOfBytesToWrite, DWORD *pNumberOfBytesWritten, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_ReadFile)(HANDLE hCon, void *pBuf, DWORD nNumberOfBytesToRead, DWORD *pNumberOfBytesRead, const char* filename, const int line);

typedef DWORD (__stdcall *CbFunc_Com_GetLastError)(const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_FlushFileBuffers)(HANDLE hCon, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_ClearCommBreak)(HANDLE hCon, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_ClearCommError)(HANDLE hCon, DWORD *pErrors, COMSTAT *pStat, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_SetupComm)(HANDLE hCon, DWORD dwInQueue, DWORD dwOutQueue, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_EscapeCommFunction)(HANDLE hCon, DWORD dwFunc, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_GetCommProperties)(HANDLE hCon, COMMPROP *pCommProp, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_GetCommState)(HANDLE hCon, DCB *pDCB, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_GetCommTimeouts)(HANDLE hCon, COMMTIMEOUTS *pCommTimeouts, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_PurgeComm)(HANDLE hCon, DWORD dwFlags, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_SetCommState)(HANDLE hCon, DCB *pDCB, const char* filename, const int line);
typedef _BOOL (__stdcall *CbFunc_Com_SetCommTimeouts)(HANDLE hCon, COMMTIMEOUTS *pCommTimeouts, const char* filename, const int line);

/*
    API: register/unregister com port confirguration relative callback functions
           only support WINDOWNS!!!!
*/
LIBEXPORT void __stdcall FlashTool_RegComFunctions(CbFunc_Com_CreateFile pFuncCreateFile,
                                                                                                   CbFunc_Com_CancelIo             pFuncCancelIo,                
                                                                                                   CbFunc_Com_CloseHandle             pFuncCloseHandle,              
                                                                                                   CbFunc_Com_WriteFile               pFuncWriteFile,            
                                                                                                   CbFunc_Com_ReadFile                pFuncReadFile,           
                                                                                                   CbFunc_Com_GetLastError        pFuncGetLastError,         
                                                                                                   CbFunc_Com_FlushFileBuffers    pFuncFlushFileBuffers,        
                                                                                                   CbFunc_Com_ClearCommBreak      pFuncClearCommBreak,     
                                                                                                   CbFunc_Com_ClearCommError      pFuncClearCommError,          
                                                                                                   CbFunc_Com_SetupComm           pFuncSetupComm,              
                                                                                                   CbFunc_Com_EscapeCommFunction  pFuncEscapeCommFunction,     
                                                                                                   CbFunc_Com_GetCommProperties   pFuncGetCommProperties,      
                                                                                                   CbFunc_Com_GetCommState        pFuncGetCommState,                
                                                                                                   CbFunc_Com_GetCommTimeouts pFuncGetCommTimeouts,             
                                                                                                   CbFunc_Com_PurgeComm           pFuncPurgeComm,                 
                                                                                                   CbFunc_Com_SetCommState        pFuncSetCommState,            
                                                                                                   CbFunc_Com_SetCommTimeouts pFuncSetCommTimeouts);
LIBEXPORT void __stdcall FlashTool_UnRegComFunctions();
#endif

LIBEXPORT int __stdcall FlashTool_DisableEMMCHWResetPin(FLASHTOOL_API_HANDLE_T ft_handle);
LIBEXPORT int __stdcall FlashTool_SetEMMCHWResetPin(FLASHTOOL_API_HANDLE_T ft_handle, emmc_hwreset_value value);

LIBEXPORT void __stdcall FlashTool_GetLastErrorMessage(FLASHTOOL_API_HANDLE_T ft_handle, char* err_msg);
LIBEXPORT int __stdcall FlashTool_GetRPMBStatus(FLASHTOOL_API_HANDLE_T ft_handle, flash_hw_status_e* rpmb_status);

typedef enum DATA_DIRECTION{
	DATA_DIRECT_INPUT = 0,
	DATA_DIRECT_OUTPUT = 1
}DATA_DIRECTION;

LIBEXPORT int __stdcall FlashTool_PL_Chanllenge(FLASHTOOL_API_HANDLE_T ft_handle, DATA_DIRECTION direct, unsigned char* databuf, unsigned int databuf_len);

/* chipname: in MTxxxx format */
LIBEXPORT bool __stdcall FlashTool_Chip_isOldArch(const char* chipname);

#ifdef  __cplusplus
}
#endif

#endif
