/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2003
 *
 ******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  flashtool.h
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
 
 *******************************************************************************/
 
#ifndef _FLASHTOOL_H_
#define _FLASHTOOL_H_

#include "mtk_status.h"
#include "brom.h"
#include "DOWNLOAD.H"
#include "flashtool_handle.h"
#include "flashtool_api.h"
#include "external_memory_config.h"

#ifdef	__cplusplus
extern "C" {
#endif

	
//------------------------------------------------------------------------------
// utility functions                                                            
//------------------------------------------------------------------------------
LIBEXPORT const char* __stdcall GetNorFlashNameByTypeId(unsigned short type_id);
LIBEXPORT const char* __stdcall GetNandFlashNameByTypeId(unsigned short type_id);
LIBEXPORT const char* __stdcall HWChipSelectToString(HW_ChipSelect_E  cs);
LIBEXPORT const char* __stdcall HWStorageTypeToString(HW_StorageType_E  type);
LIBEXPORT const char* __stdcall HWRamTypeToString(HW_RAMType_E  type);
LIBEXPORT const char* __stdcall HWMemoryTypeToString(HW_MemoryType_E type);
LIBEXPORT const char* __stdcall HWMemoryIOToString(HW_MemoryIO_E type);
LIBEXPORT const char* __stdcall HWMemoryTestMethodToString(HW_MemoryTestMethod_E type);
LIBEXPORT const char* __stdcall ReadFlagToString(NUTL_ReadFlag_E  flag);
LIBEXPORT const char* __stdcall EraseFlagToString(NUTL_EraseFlag_E  flag);
LIBEXPORT const char* __stdcall ProgramFlagToString(NUTL_ProgramFlag_E  flag);
LIBEXPORT const char* __stdcall AddrTypeFlagToString(NUTL_AddrTypeFlag_E  flag);
LIBEXPORT const char* __stdcall EmmcPartToString(EMMC_Part_E part);
LIBEXPORT const char* __stdcall ParamOpToString(PARAM_OPERATION  op);
LIBEXPORT const char* __stdcall OTPOpToString(OTP_OPERATION  op);
_DEPRECATED_API int   __stdcall NvRAMInfoReport(NVRAM_REPORT_T *p_NvRamReport);   /**< mh: merged from 16 */

//------------------------------------------------------------------------------
// DA report related function                                                   
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall ConvertDAReportToString(
		const DA_REPORT_T		*p_da_report,
		char					*p_device_hint,
		unsigned int			device_hint_size,
		char					*p_detail_report,
		unsigned int			detail_report_size);

//------------------------------------------------------------------------------
// prototype of callback function                                               
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_PARAM_PROGRESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_PARAM_PROGRESS)(unsigned char finished_percentage, void *usr_arg);
//------------------------------------------------------------------------------
// FLASHTOOL_ARG structure                                                      
//------------------------------------------------------------------------------

typedef	struct {
	
	// BootFlashTool arg 
	BOOT_FLASHTOOL_ARG						m_boot_arg;

	// da report callback 
	CALLBACK_DA_REPORT						m_cb_da_report;
	void *									m_cb_da_report_arg;

	// security pre-process notify callback 
	CALLBACK_SECURITY_PRE_PROCESS_NOTIFY	m_cb_security_pre_process_notify;
	void *									m_cb_security_pre_process_notify_arg;

	// set high-speed baudrate callback 
	CALLBACK_SET_HIGHSPEED_BAUDRATE			m_cb_set_high_speed_baudrate;
	void *									m_cb_set_high_speed_baudrate_arg;
	
	unsigned char							m_baudrate_full_sync_count;

	HW_ChipSelect_E		m_nor_chip_select[2];

	HW_ChipSelect_E		m_nand_chip_select;
	unsigned int *		m_p_nand_acccon;
	// enable DRAM setting from UI thread
	_BOOL				m_enable_ui_dram_cfg;
	DRAM_SETTING		m_dram_cfg;

} FLASHTOOL_ARG;

typedef	struct {
	
	// DA report 
	DA_REPORT_T			m_da_report;

	DA_INFO				m_da_info;
	AUTH_INFO			m_auth_info;
	SCERT_INFO			m_scert_info;

} FLASHTOOL_RESULT;


//------------------------------------------------------------------------------
// FLASHTOOL_DOWNLOAD structure                                                             
//------------------------------------------------------------------------------

typedef	struct {

	HW_StorageType_E				m_storage_type;     /**< mh: merged from 16 */

	// single DL_HANDLE 
	DL_HANDLE_T						m_dl_handle;
	// multi DL_HANDLE List 
	DL_HANDLE_LIST_T				m_dl_handle_list;

	// DL_HANDLE download progress callback 
	ACCURACY							m_download_accuracy;
	CALLBACK_DOWNLOAD_PROGRESS_INIT		m_cb_download_flash_init;
	void *								m_cb_download_flash_init_arg;
	CALLBACK_DOWNLOAD_PROGRESS			m_cb_download_flash;
	void *								m_cb_download_flash_arg;

	// Boot Loader download progress callback 
	CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS_INIT	m_cb_download_bloader_init;
	void *										m_cb_download_bloader_init_arg;
	CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS		m_cb_download_bloader;
	void *										m_cb_download_bloader_arg;

    // Checksum progress callback 
    CALLBACK_CHECKSUM_PROGRESS_INIT             m_cb_checksum_init;
    void *                                      m_cb_checksum_init_arg;
    CALLBACK_CHECKSUM_PROGRESS                  m_cb_checksum;
    void *                                      m_cb_checksum_arg;

	// security post-process notify callback 
	CALLBACK_SECURITY_POST_PROCESS_NOTIFY		m_cb_security_post_process_notify;
	void *										m_cb_security_post_process_notify_arg;

	// write FlashToolCfg/CUST_PARA/SEC_RO/OTP parameters 

	// FlashToolCfg 
	_BOOL				m_ft_cfg_enable;
	FlashToolCfg_T		m_ft_cfg;
	// CUST_PARA 
	_BOOL				m_cust_para_enable;
	CustPara_T			m_cust_para;
	// SEC_RO 
	_BOOL				m_sec_ro_enable;
	SecRO_T				m_sec_ro;
	// OTP 
	_BOOL				m_otp_enable;
	OTP_OPERATION		m_otp_op;
	OTP_T				m_otp;

	CALLBACK_PARAM_PROGRESS_INIT		m_cb_param_process_init;
	void *								m_cb_param_process_init_arg;
	CALLBACK_PARAM_PROGRESS				m_cb_param_process;
	void *								m_cb_param_process_arg;

	// format option 
	_BOOL								m_format_enable;
	FORMAT_CONFIG_T						m_format_cfg;
	// format progress callback 
	CALLBACK_FORMAT_PROGRESS_INIT		m_cb_format_report_init;
	void *								m_cb_format_report_init_arg;
	CALLBACK_FORMAT_PROGRESS			m_cb_format_report;
	void *								m_cb_format_report_arg;
	// format statistics callback 
	CALLBACK_FORMAT_STATISTICS			m_cb_format_statistics;
	void *								m_cb_format_statistics_arg;

	// post-process callback 
	CALLBACK_POST_PROCESS_INIT			m_cb_post_process_init;
	void *								m_cb_post_process_init_arg;
	CALLBACK_POST_PROCESS				m_cb_post_process;
	void *								m_cb_post_process_arg;

	// The flag of m_enable_tgt_res_layout_check is used to control whether if target resource 
	// layout checking operation will be performed.                                          
	// _TRUE: Enable target resource layout checking operation.                         
	// _FALSE: Disable target resource layout checking operation.                       
	_BOOL				m_enable_tgt_res_layout_check;

	// The flag to check if target side baseband chip version is corresponding to ROM file on PC side. 
	_BOOL				m_enable_bbchip_ver_check;

	// Download Style : _FALSE	: best effort erase
	//					_TRUE	: sequential erase (sector by sector)
	_BOOL				m_downloadstyle_sequential;

    // Write USB download control bit
	_BOOL				m_write_usb_download_control_bit;   /**< mh: merged from 16 */

	// trigger watch dog timeout after downloading 
	_BOOL				m_enable_wdt_timeout;
	unsigned short		m_ms_wdt_timeout_interval;

	// disable Finish Cmd (for GPS ROM download)
	_BOOL				m_relay_da_enable;
	GPIO_GPS_SETTING	m_gpio_gps;

} FLASHTOOL_DOWNLOAD_ARG;

typedef	struct {

	FormatStatisticsReport_T	m_format_statistics;

	// NFB boot loader info 
	BL_INFO				m_bl_info;

	ROM_INFO			m_rom_info;
	
} FLASHTOOL_DOWNLOAD_RESULT;

_DEPRECATED_API int __stdcall FlashDownload(
        COM_PORT_SETTING                *p_com_setting,
        const FLASHTOOL_ARG             *p_arg,
        FLASHTOOL_RESULT                *p_result,
        const FLASHTOOL_DOWNLOAD_ARG    *p_dl_arg,
        FLASHTOOL_DOWNLOAD_RESULT       *p_dl_result,
        const ExternalMemoryConfig      *p_external_memory_config,
        int                             *p_stopflag);

_DEPRECATED_API int __stdcall FlashDownload_ByhCOM(
        COM_PORT_SETTING                *p_com_setting,
        const FLASHTOOL_ARG             *p_arg,
        FLASHTOOL_RESULT                *p_result,
        const FLASHTOOL_DOWNLOAD_ARG    *p_dl_arg,
        FLASHTOOL_DOWNLOAD_RESULT       *p_dl_result,
        const ExternalMemoryConfig      *p_external_memory_config,
        int                             *p_stopflag);

//------------------------------------------------------------------------------
// FLASHTOOL_READBACK_ARG structure                                                              
//------------------------------------------------------------------------------
typedef	struct {
	
	HW_StorageType_E					m_storage_type;

	RB_HANDLE_T							m_rb_handle;

	// readback progress callback 
	ACCURACY							m_readback_accuracy;
	CALLBACK_READBACK_PROGRESS_INIT		m_cb_readback_flash_init;
	void *								m_cb_readback_flash_init_arg;
	CALLBACK_READBACK_PROGRESS			m_cb_readback_flash;
	void *								m_cb_readback_flash_arg;

} FLASHTOOL_READBACK_ARG;

typedef	struct {

	RB_INFO		m_rb_info;

} FLASHTOOL_READBACK_RESULT;

/**< mh: merged from 16 */
_DEPRECATED_API int __stdcall NandUtil_FlashReadback(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_READBACK_ARG	*p_rb_arg,
		FLASHTOOL_READBACK_RESULT		*p_rb_result,
		int								*p_stopflag);

_DEPRECATED_API int __stdcall FlashReadback(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_READBACK_ARG	*p_rb_arg,
		FLASHTOOL_READBACK_RESULT		*p_rb_result,
		const ExternalMemoryConfig      *p_external_memory_config,
		int								*p_stopflag);

_DEPRECATED_API int __stdcall FlashReadback_ByhCOM(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_READBACK_ARG	*p_rb_arg,
		FLASHTOOL_READBACK_RESULT		*p_rb_result,
		const ExternalMemoryConfig      *p_external_memory_config,
		int								*p_stopflag);

//------------------------------------------------------------------------------
// FLASHTOOL_FORMAT_ARG structure                                                                 
//------------------------------------------------------------------------------
typedef	struct {
	
	HW_StorageType_E					m_storage_type;

	// format config 
	FORMAT_CONFIG_T						m_format_cfg;

	// only take effect on NAND flash 
	NUTL_EraseFlag_E  					m_erase_flag;

	// format progress callback 
	CALLBACK_FORMAT_PROGRESS_INIT		m_cb_format_report_init;
	void *								m_cb_format_report_init_arg;
	CALLBACK_FORMAT_PROGRESS			m_cb_format_report;
	void *								m_cb_format_report_arg;
	// format statistics callback 
	CALLBACK_FORMAT_STATISTICS			m_cb_format_statistics;
	void *								m_cb_format_statistics_arg;

    // Write USB download control bit
	_BOOL				m_write_usb_download_control_bit;   /**< mh: merged from 16 */

	// trigger watch dog timeout after format 
	_BOOL								m_enable_wdt_timeout;
	unsigned short						m_ms_wdt_timeout_interval;

} FLASHTOOL_FORMAT_ARG;

typedef	struct {

	FormatStatisticsReport_T			m_format_statistics;

} FLASHTOOL_FORMAT_RESULT;

_DEPRECATED_API int __stdcall NandUtil_FlashFormat(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_FORMAT_ARG		*p_fmt_arg,
		FLASHTOOL_FORMAT_RESULT			*p_fmt_result,
		int *p_stopflag);

_DEPRECATED_API int __stdcall FlashFormat(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_FORMAT_ARG		*p_fmt_arg,
		FLASHTOOL_FORMAT_RESULT			*p_fmt_result,
		const ExternalMemoryConfig      *p_external_memory_config,
		int *p_stopflag);

_DEPRECATED_API int __stdcall FlashFormat_ByhCOM(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_FORMAT_ARG		*p_fmt_arg,
		FLASHTOOL_FORMAT_RESULT			*p_fmt_result,
		const ExternalMemoryConfig      *p_external_memory_config,
		int								*p_stopflag);

//------------------------------------------------------------------------------
// PARAMETER_ARG structure                                                              
//------------------------------------------------------------------------------

typedef struct {
	// FlashToolCfg 
	_BOOL				m_ft_cfg_enable;
	PARAM_OPERATION		m_ft_cfg_op;
	FlashToolCfg_T		m_ft_cfg;
	// CUST_PARA 
	_BOOL				m_cust_para_enable;
	PARAM_OPERATION		m_cust_para_op;
	CustPara_T			m_cust_para;
	// SEC_RO 
	_BOOL				m_sec_ro_enable;
	PARAM_OPERATION		m_sec_ro_op;
	SecRO_T				m_sec_ro;
	// OTP 
	_BOOL				m_otp_enable;
	OTP_OPERATION		m_otp_op;
	OTP_T				m_otp;
} PARAMETER_ARG;

typedef	struct {

	DL_HANDLE_T			m_dl_handle;
	
	// parameters 
	PARAMETER_ARG		m_param;

	// update parameter progress callback 
	CALLBACK_PARAM_PROGRESS_INIT		m_cb_param_process_init;
	void *								m_cb_param_process_init_arg;
	CALLBACK_PARAM_PROGRESS				m_cb_param_process;
	void *								m_cb_param_process_arg;

	// post-process callback for Update SEC_RO
	CALLBACK_POST_PROCESS_INIT			m_cb_post_process_init;
	void *								m_cb_post_process_init_arg;
	CALLBACK_POST_PROCESS				m_cb_post_process;
	void *								m_cb_post_process_arg;

	// enter relay mode (for GPS ROM download)
	_BOOL				m_relay_da_enable;
	GPIO_GPS_SETTING	m_gpio_gps;

	// trigger watch dog timeout after format 
	_BOOL								m_enable_wdt_timeout;
	unsigned short						m_ms_wdt_timeout_interval;

} FLASHTOOL_PARAMETER_ARG;

typedef	struct {

	// dummy 

} FLASHTOOL_PARAMETER_RESULT;

_DEPRECATED_API int __stdcall NandUtil_FlashParameter(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_PARAMETER_ARG	*p_pm_arg,
		FLASHTOOL_PARAMETER_RESULT		*p_pm_result,
		int								*p_stopflag);

_DEPRECATED_API int __stdcall FlashParameter(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_PARAMETER_ARG	*p_pm_arg,
		FLASHTOOL_PARAMETER_RESULT		*p_pm_result,
		int								*p_stopflag);

_DEPRECATED_API int __stdcall FlashParameter_ByhCOM(
		COM_PORT_SETTING				*p_com_setting,
		const FLASHTOOL_ARG				*p_arg,
		FLASHTOOL_RESULT				*p_result,
		const FLASHTOOL_PARAMETER_ARG	*p_pm_arg,
		FLASHTOOL_PARAMETER_RESULT		*p_pm_result,
		int								*p_stopflag);


//------------------------------------------------------------------------------

#ifdef	__cplusplus
}
#endif

#endif


