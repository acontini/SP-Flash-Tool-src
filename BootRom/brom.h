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
 *  brom.h
 *
 * Project:
 * --------
 *  BootRom Library
 *
 * Description:
 * ------------
 *  Exported C interface APIs for BootRom library.
 *
 * Author:
 * -------
 *  Amos Hsu (mtk00539)
 *
 *******************************************************************************/
 
#ifndef _BROM_H_
#define _BROM_H_

#include "host.h"
#include "flashtool_handle.h"

#ifdef  __cplusplus
extern "C" {
#endif

struct FLASHTOOL_API_HANDLE;
typedef struct FLASHTOOL_API_HANDLE *   FLASHTOOL_API_HANDLE_T;

// The magic value to stop boot process 
#define BOOT_STOP   9876

// boot infinitely
#define BOOT_INFINITE   0xFFFFFFF

// default start cmd retry count 
#define DEFAULT_BROM_START_CMD_RETRY_COUNT  1

//------------------------------------------------------------------------------
// prototype of COM initial stage callback                                          
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_COM_INIT_STAGE)(HANDLE hCOM, void *usr_arg);

//------------------------------------------------------------------------------
// prototype of BootROM stage callback                                          
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_IN_BROM_STAGE)(BROM_HANDLE_T brom_handle, HANDLE hCOM, void *usr_arg);

//------------------------------------------------------------------------------
// prototype of write buf progress callback                                     
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_WRITE_BUF_PROGRESS_INIT)(void *usr_arg);
typedef int (__stdcall *CALLBACK_WRITE_BUF_PROGRESS)(unsigned char finished_percentage, unsigned int finished_bytes, unsigned int total_bytes, void *usr_arg);

//------------------------------------------------------------------------------
// prototype of Serial-Link-Authentication challenge callback                   
//------------------------------------------------------------------------------
typedef int (__stdcall *CALLBACK_SLA_CHALLENGE)(void *usr_arg, const unsigned char  *p_challenge_in, unsigned int  challenge_in_len, unsigned char  **pp_challenge_out, unsigned int  *p_challenge_out_len);
typedef int (__stdcall *CALLBACK_SLA_CHALLENGE_END)(void *usr_arg, unsigned char  *p_challenge_out);

//------------------------------------------------------------------------------
// runtime debug trace                                                          
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall Brom_Debug_SetLogFilename(const char *filename);
LIBEXPORT int __stdcall Brom_DebugOn(void);
LIBEXPORT int __stdcall Brom_DebugOff(void);
LIBEXPORT int __stdcall Brom_DebugClear(void);
LIBEXPORT int __stdcall Brom_Debug_SetLogPrefix(const char *prefix);

//------------------------------------------------------------------------------
// translate enum to string                                                     
//------------------------------------------------------------------------------
LIBEXPORT const char* __stdcall BBChipTypeToName(const BBCHIP_TYPE  bbchip);
LIBEXPORT const char* __stdcall ExtClockToName(const EXT_CLOCK  ext_clock);
LIBEXPORT const char* __stdcall StatusToString(int status);

//------------------------------------------------------------------------------
// Windows message to notify application ready to power on target               
//------------------------------------------------------------------------------
#define WM_BROM_READY_TO_POWER_ON_TGT       (0x8888)
LIBEXPORT int __stdcall GetReadyToPowerOnWinAppMsgID(unsigned int *p_wm_app_id);
LIBEXPORT int __stdcall SetReadyToPowerOnWinAppMsgID(unsigned int wm_app_id);

//------------------------------------------------------------------------------
// boot-up result                                                               
//------------------------------------------------------------------------------
typedef struct {

    BBCHIP_TYPE     m_bbchip_type;
    char            m_bbchip_name[32];
    unsigned short  m_bbchip_hw_ver;
    unsigned short  m_bbchip_sw_ver;
    unsigned short  m_bbchip_hw_code;
    EXT_CLOCK       m_ext_clock;
    unsigned char   m_bbchip_secure_ver;
    unsigned char   m_bbchip_bl_ver;
    unsigned int    m_fw_ver_len;
    char            m_fw_ver[64];

    unsigned char   m_msp_err_code;

} BOOT_RESULT;

//------------------------------------------------------------------------------
// Old boot META mode (Depreciated)                                                             
//------------------------------------------------------------------------------
typedef struct {

    BBCHIP_TYPE     m_bbchip_type;
    EXT_CLOCK       m_ext_clock;

    unsigned int    m_ms_boot_timeout;
    unsigned int    m_max_start_cmd_retry_count;

    // This callback function will be invoke after COM port is opened
    // You can do some initialization by using this callback function.
    CALLBACK_COM_INIT_STAGE     m_cb_com_init_stage;
    void *                      m_cb_com_init_stage_arg;

    // This callback function will be invoke after BootROM start cmd is passed. 
    // You can issue other BootROM command by brom_handle and hCOM which provides callback arguments, 
    // or do whatever you want otherwise. 
    CALLBACK_IN_BROM_STAGE      m_cb_in_brom_stage;
    void *                      m_cb_in_brom_stage_arg;

    // speed-up BootROM stage baudrate 
    _BOOL   m_speedup_brom_baudrate;

    // Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message 
    HWND    m_ready_power_on_wnd_handle;
    void *  m_ready_power_on_wparam;
    void *  m_ready_power_on_lparam;

    // Serial Link Authentication 
    AUTH_HANDLE_T       m_auth_handle;          // AUTH file handle 
    SCERT_HANDLE_T      m_scert_handle;         // CERT file handle
    CALLBACK_SLA_CHALLENGE          m_cb_sla_challenge;
    void *                          m_cb_sla_challenge_arg;
    CALLBACK_SLA_CHALLENGE_END      m_cb_sla_challenge_end;
    void *                          m_cb_sla_challenge_end_arg;

    // use USB Cable
    _BOOL                           m_usb_enable;

} BOOT_META_ARG;

_DEPRECATED_API int  __stdcall Boot_META(
        HANDLE hCOM,
        const BOOT_META_ARG  *p_arg,
        BOOT_RESULT  *p_result,
        int *p_bootstop);

//------------------------------------------------------------------------------
// boot FlashTool download mode                                                 
//------------------------------------------------------------------------------

#define WAIT_STATE_AUTO_DETECT_BY_FREQUENCY     NULL

typedef struct {

    BBCHIP_TYPE     m_bbchip_type;
    EXT_CLOCK       m_ext_clock;
    unsigned int    m_baudrate;

    unsigned int    m_ms_boot_timeout;
    unsigned int    m_max_start_cmd_retry_count;

    // This callback function will be invoke after COM port is opened
    // You can do some initialization by using this callback function.
    CALLBACK_COM_INIT_STAGE     m_cb_com_init_stage;
    void *                      m_cb_com_init_stage_arg;

    // This callback function will be invoke after BootROM start cmd is passed. 
    // You can issue other BootROM command by brom_handle and hCOM which provides callback arguments, 
    // or do whatever you want otherwise. 
    CALLBACK_IN_BROM_STAGE      m_cb_in_brom_stage;
    void *                      m_cb_in_brom_stage_arg;

    // speed-up BootROM stage baudrate 
    _BOOL   m_speedup_brom_baudrate;

    // Application's window handle to send WM_BROM_READY_TO_POWER_ON_TGT message 
    HWND    m_ready_power_on_wnd_handle;
    void *  m_ready_power_on_wparam;
    void *  m_ready_power_on_lparam;

    // Serial Link Authentication 
    AUTH_HANDLE_T       m_auth_handle;          // AUTH file handle 
    SCERT_HANDLE_T      m_scert_handle;         // CERT file handle
    CALLBACK_SLA_CHALLENGE          m_cb_sla_challenge;
    void *                          m_cb_sla_challenge_arg;
    CALLBACK_SLA_CHALLENGE_END      m_cb_sla_challenge_end;
    void *                          m_cb_sla_challenge_end_arg;

    // EMI Control for Bank0 and Bank1 
    unsigned int    *m_p_bank0_mem_cfg;     // bank0 memory config 
    unsigned int    *m_p_bank1_mem_cfg;     // bank0 memory config 

    // Download Agent 
    _BOOL           m_enable_da_start_addr; // Old parameter, no use now. Use DA_SetDefaultStartAddr instead. 
    unsigned int    m_da_start_addr;        // Old parameter, no use now. Use DA_SetDefaultStartAddr instead. 
    DA_HANDLE_T     m_da_handle;

    // da download progress callack 
    CALLBACK_WRITE_BUF_PROGRESS_INIT    m_cb_download_da_init;
    void *                              m_cb_download_da_init_arg;
    CALLBACK_WRITE_BUF_PROGRESS         m_cb_download_da;
    void *                              m_cb_download_da_arg;

    // use USB Cable
    _BOOL                           m_usb_enable;	// useless...
    unsigned int                    m_bmt_block_count;

} BOOT_FLASHTOOL_ARG;

LIBEXPORT int  __stdcall Boot_FlashTool(
        HANDLE hCOM,
        const BOOT_FLASHTOOL_ARG  *p_arg,
        BOOT_RESULT  *p_result,
        int *p_bootstop);


//------------------------------------------------------------------------------
// Boot mode type                                                               
//------------------------------------------------------------------------------
typedef enum 
{
    NORMAL_BOOT         = 0,
    META_BOOT           = 1,
    RECOVERY_BOOT       = 2,    
    SW_REBOOT           = 3,
    FACTORY_BOOT        = 4,
    ADVMETA_BOOT        = 5,
    ATE_FACTORY_BOOT    = 6,
    ALARM_BOOT          = 7,
    DUALTALK_SWITCH          = 8,

    FAST_BOOT               = 99,
    DOWNLOAD_BOOT       = 100,
    UNKNOWN_BOOT
    
} BOOT_MODE;

//------------------------------------------------------------------------------
// New boot mode for SP                                                               
//------------------------------------------------------------------------------
typedef struct 
{
    //Old parameters
    BBCHIP_TYPE m_bbchip_type;
    EXT_CLOCK m_ext_clock;
    unsigned int m_ms_boot_timeout;
    unsigned int m_max_start_cmd_retry_count;

    //New parameters
    unsigned int m_uTimeout;
    unsigned int m_uRetryTime;
    unsigned int m_uInterval;
    unsigned int m_uBaudrate;
    int * m_pStopFlag;
    
    bool m_bIsUSBEnable;
    bool m_bIsSymbolicEnable;
    bool m_bIsCompositeDeviceEnable;
    bool m_bDisableMobileLogService;
    BOOT_MODE m_euBootMode;
    unsigned short m_uMDMode;
    unsigned int m_uPortNumber;
    char m_szPortSymbolic[256];

    // Serial Link Authentication 
    AUTH_HANDLE_T  m_auth_handle;
    SCERT_HANDLE_T m_scert_handle;
    CALLBACK_SLA_CHALLENGE  m_cb_sla_challenge;
    void *  m_cb_sla_challenge_arg;
    CALLBACK_SLA_CHALLENGE_END  m_cb_sla_challenge_end;
    void *  m_cb_sla_challenge_end_arg;
    
} BOOT_ARG_S;

LIBEXPORT int __stdcall BootROM_BootMode(BOOT_ARG_S * pArg);
LIBEXPORT int __stdcall Preloader_BootMode(BOOT_ARG_S * pArg);


//------------------------------------------------------------------------------
// Send certificate file                                                 
//------------------------------------------------------------------------------
typedef struct {

    BBCHIP_TYPE     m_bbchip_type;
    EXT_CLOCK       m_ext_clock;
    unsigned int    m_baudrate;  

    unsigned int    m_ms_boot_timeout;
    unsigned int    m_max_start_cmd_retry_count;

    CALLBACK_COM_INIT_STAGE     m_cb_com_init_stage;
    void *                      m_cb_com_init_stage_arg;
    CALLBACK_IN_BROM_STAGE      m_cb_in_brom_stage;
    void *                      m_cb_in_brom_stage_arg;

    // Serial Link Authentication 
    SCERT_HANDLE_T      m_scert_handle;         // CERT file handle

} SCERT_ARG_S;

//Integrate Connect and SendSCERT function together
LIBEXPORT int  __stdcall Brom_SendSCERT(HANDLE hCOM, const SCERT_ARG_S  * pScertArg, BOOT_RESULT  * pRet, int * pBootstop);
//SendSCERT function only, Brom_Connect() should be called before this API
LIBEXPORT int  __stdcall Brom_SendSCERTWithHandle(BROM_HANDLE_T pBROMHandle, FLASHTOOL_API_HANDLE_T pFTHandle, const SCERT_ARG_S  * pScertArg, BOOT_RESULT  * pRet, int * pBootstop);


//------------------------------------------------------------------------------
// get brom.dll version                                                         
//------------------------------------------------------------------------------
LIBEXPORT int __stdcall Brom_GetDLLVer(unsigned int *major_ver, unsigned int *minor_ver, unsigned int *build_num, unsigned int *patch_num);
LIBEXPORT int __stdcall Brom_GetDLLInfo(const char **pp_version, const char **pp_release_type, const char **pp_build_date, const char **pp_load_path);

//------------------------------------------------------------------------------
// BootROM Command                                                              
//------------------------------------------------------------------------------
typedef struct {
    U64             begin_addr;
    U64             jump_addr;
    U64             dl_data_len;
    const char*     dl_data_buffer;
    bool            is_jump;
    CALLBACK_WRITE_BUF_PROGRESS_INIT    m_cb_init;
    void*                               m_cb_init_arg;
    CALLBACK_WRITE_BUF_PROGRESS         m_cb_progress;
    void*                               m_cb_progress_arg;
}FlashTool_BROM_DL_Arg;

typedef struct {

    // dummy

} FlashTool_Brom_DL_Result;

LIBEXPORT int __stdcall Brom_Create(BROM_HANDLE_T *p_brom_handle, HANDLE hCOM, BBCHIP_TYPE  bbchip_type, EXT_CLOCK  ext_clock, unsigned int baudrate, int *p_bootstop, unsigned int ms_boot_timeout, unsigned int max_start_cmd_retry_count, BOOT_RESULT *p_result);
LIBEXPORT int __stdcall Brom_Connect(BROM_HANDLE_T *p_brom_handle, FLASHTOOL_API_HANDLE_T  *p_ft_handle, unsigned char  com_port, BBCHIP_TYPE  bbchip_type, EXT_CLOCK  ext_clock, int *p_bootstop, unsigned int ms_boot_timeout, unsigned int max_start_cmd_retry_count);
LIBEXPORT int __stdcall Brom_DisConnect(BROM_HANDLE_T *p_brom_handle, FLASHTOOL_API_HANDLE_T  *p_ft_handle);
LIBEXPORT int __stdcall Brom_Destroy(BROM_HANDLE_T *p_brom_handle);
LIBEXPORT int __stdcall Brom_StartCmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, BOOT_RESULT  *p_result);
LIBEXPORT int __stdcall Brom_Write16Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int write_addr, unsigned short *data, unsigned int num_of_word);
LIBEXPORT int __stdcall Brom_Read16Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int read_addr, unsigned short *data, unsigned int num_of_word);
LIBEXPORT int __stdcall Brom_Write32Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int write_addr, unsigned int *data, unsigned int num_of_dword);
LIBEXPORT int __stdcall Brom_Read32Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int read_addr, unsigned int *data, unsigned int num_of_dword);
LIBEXPORT int __stdcall Brom_Checksum16Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int chksum_addr, unsigned int num_of_word, unsigned short *chksum);
LIBEXPORT int __stdcall Brom_JumpCmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int jump_addr, unsigned char bWaitForAck);
LIBEXPORT int __stdcall Brom_JumpCmd_Ex(FLASHTOOL_API_HANDLE_T ft_handle, FlashTool_BROM_DL_Arg *dl_arg, unsigned char bWaitForAck);
/* Add for BROM PMIC WRAP, 2012/09/28 { */
LIBEXPORT int __stdcall Brom_WrapInitCmd(BROM_HANDLE_T brom_handle, HANDLE hCOM);
LIBEXPORT int __stdcall Brom_WrapWrite16Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int write_addr, unsigned short data);
LIBEXPORT int __stdcall Brom_WrapRead16Cmd(BROM_HANDLE_T brom_handle, HANDLE hCOM, unsigned int read_addr, unsigned short *data);
LIBEXPORT int __stdcall Brom_WrapDeInitCmd(BROM_HANDLE_T brom_handle, HANDLE hCOM);
/* Add for BROM PMIC WRAP, 2012/09/28 { */
/* Add BROM CMD API to setup USB, 2012/07/18{ */
typedef enum {
    DISCONNECT_USB_AND_RELEASE_POWERKEY = 0,
    DISCONNECT_USB_ONLY                 = 1,
    KEEP_USB_AND_RELEASE_POWERKEY       = 2,
    RELEASE_POWERKEY_ONLY               = 3,
    STATUS_UNKNOWN                      = 0xFFFF
}DEVICE_STATUS;
LIBEXPORT int __stdcall Brom_Rehabilitate(BROM_HANDLE_T brom_handle, const FLASHTOOL_API_HANDLE_T ft_handle, DEVICE_STATUS device_status);
LIBEXPORT const char* __stdcall BROM_USBEnumToString(DEVICE_STATUS device_status);
/* Add BROM CMD API to setup USB, 2012/07/18} */
//------------------------------------------------------------------------------
// BootROM Efsue write Command                                                              
//------------------------------------------------------------------------------

// efuse key
typedef enum
{
    //k2 KEY 
    C_LOCK,
    C_CTRL1_LOCK,
    C_CTRL0_LOCK,
    C_DATA0_LOCK,
    C_DATA1_LOCK,
    C_DATA2_LOCK,
    C_DATA3_LOCK,
    C_DATA4_LOCK,
    C_DATA5_LOCK,

    CTRL1,
    SEC_CTRL1,

    C_CTRL_0,
    C_CTRL_1,
    C_DATA_0,
    C_DATA_1,
    C_DATA_2,
    C_DATA_3,
    C_DATA_4,
    C_DATA_5,

    SEC_CAP,
    PROD_EN,
	
    CUSTK,

    END_KEY
}EFUSE_KEY;

typedef enum
{
    T_BOOLEAN   = 1,
    T_INT,
    T_BUF
}DATA_TYPE;

// free length buffer structure
typedef struct
{
	unsigned int buf_len;
	char *buf;
}_BUFFER;

// Secure Key structure
// key string char array buffer with NULL character in the end
typedef _BUFFER KeyBufST;

typedef struct
{
    unsigned int key_type; //0, leagcy; 1, pss
	KeyBufST key_e;
	KeyBufST key_n; 
}PublicKeyStringST;

typedef _BUFFER KeyHashValue;

typedef _BUFFER SpareData;

typedef union
{
    struct
    {
        unsigned int key_blow;
        KeyBufST key;
    }key_pair;
    unsigned int     enable;
    struct
    {
        unsigned int      value;
        unsigned int      blow;
    }iPair;
}EFUSE_EXTRA_DATA;

typedef struct
{
    unsigned int key;
    unsigned int type;
    EFUSE_EXTRA_DATA data;
}EFUSE_ITEM;

#ifndef EFUSE_ENABLE
#define EFUSE_ENABLE	0xA59914B3
#endif
#ifndef EFUSE_DISABLE
#define EFUSE_DISABLE	0x241C8F6D
#endif

// Argument for writing Efuse common register
typedef struct {
    // true =  0xA59914B3
    // false = 0x241C8F6D
    
    unsigned int    emmc_boot_dis;
    unsigned int    nand_boot_dis;
    unsigned int    nand_boot_speedup_dis;

    unsigned int    usbdl_type_blow;
    unsigned int    usbdl_type; // valid value is only 0~3 for 2 bit
    
    unsigned int    usb_id_blow;
    unsigned short  usb_pid;    // 16 bit
    unsigned short  usb_vid;    // 16 bit
    
    unsigned int    spare_blow;
    unsigned int    sbc_pub_hash_dis; //-1
    unsigned int    sbc_pub_hash1_dis; //-1
#define spare_num_of_byte spare.buf_len
    SpareData       spare;
    unsigned int    pid_vid_custom_en;
    unsigned int    ufs_boot_dis; //-1
    unsigned int    ext_reserved[29]; //32-1-1-1
} Efuse_Common_Arg;

// Argument for writing Efuse security register
typedef struct{
    // true =  0xA59914B3
    // false = 0x241C8F6D

    unsigned int    acc_en;
    unsigned int    ack_en;
    unsigned int    sbc_en;
    unsigned int    daa_en;
    unsigned int    sla_en;
    unsigned int    root_cert_en;
    unsigned int    jtag_dis;
    
    unsigned int    ac_key_blow;
    unsigned int  pl_ar_en; //-1
    unsigned int pk_cus_en; //-1
	
    KeyBufST        ac_key;         // ACK length 128 bit = 16 byte
    
    unsigned int    sbc_pubk_blow;
    union
    {
        PublicKeyStringST w_sbc_pub_key;  //-1// sbc_public_key argument when using Brom_WriteEfuseSecure()
        KeyHashValue r_sbc_pub_key_hash;  // return sbc_public_key hash value when using Brom_WriteEfuseAll()
    }sbc_pub_key_u;

    unsigned int    sbc_pubk1_blow;
    union
    {
	PublicKeyStringST w_sbc_pub_key1;  //-1// sbc_public_key argument when using Brom_WriteEfuseSecure()
	KeyHashValue r_sbc_pub_key_hash1;  // return sbc_public_key hash value when using Brom_WriteEfuseAll()
    }sbc_pub_key1_u;

    unsigned int 	jtag_sw_con;
    unsigned int  rom_cmd_dis;
    unsigned int  dbgport_lock_dis;
    unsigned int  jtag_sec_passwd_en;
    unsigned int  md1_sbc_en; //-1
    unsigned int  c2k_sbc_en; //-1
    unsigned int  dxcc_kcst_en; //-1 //DXCC_CM_FLAG 
    unsigned int  reserved[57]; //64-2-1-1-1-1-1
} Efuse_Secure_Arg;

// Argument for locking Efuse register
typedef struct{
    // lock =  0xA59914B3
    // no-action = 0x241C8F6D  (can't unlock)
    
    unsigned int common_ctrl_lock;
    unsigned int usb_id_lock;
    unsigned int spare_lock;
    unsigned int sec_ctrl_lock;
    unsigned int ackey_lock;
    unsigned int sbc_pubk_hash_lock;
    unsigned int sec_msc_lock; //-1
    unsigned int custk_lock; //-1
    unsigned int sbc_pubk_hash1_lock; //-1
    unsigned int reserved[29];//32-1-1-1
} Efuse_Lock_Arg;

typedef struct{
    unsigned int key_blow;
    char* key_name;
    KeyBufST    stb_key;
}STB_KEY_PARAM;

typedef struct{
    unsigned int stb_key_g7_lock;
    unsigned int stb_key_g6_lock;
    unsigned int stb_key_g5_lock;
    unsigned int stb_key_g4_lock;
    unsigned int stb_key_g3_lock;
    unsigned int stb_key_g2_lock;
    unsigned int stb_key_g1_lock;
    unsigned int stb_key_g0_lock;
    unsigned int stb_key_operatorid_lock;
    unsigned int stb_key_chipid_lock;
    unsigned int stb_key_sn_lock;
}STB_Lock_PARAM;

// Argument for STB Key Efuse register
typedef struct{
    STB_Lock_PARAM stb_lock;
    unsigned int stb_key_chipid_blow;
    unsigned int stb_key_chipID;
    unsigned int stb_key_chipid_spr;
    unsigned int stb_key_operatorid_blow;
    unsigned int stb_key_operatorid;
    unsigned int stb_key_operatorid_spr;
    STB_KEY_PARAM stb_blow_keys[16];
}Efuse_STB_Key_Arg;

typedef struct{
    EFUSE_ITEM  items[END_KEY];
    unsigned int item_count;
}Efuse_Extra_Arg;

//class BRom_Base;
_DEPRECATED_API int __stdcall Brom_WriteEfuseCommon(unsigned int brom_handle, FLASHTOOL_API_HANDLE_T ft_handle, const Efuse_Common_Arg *common_arg, bool error_log_en);
_DEPRECATED_API int __stdcall Brom_WriteEfuseSecure(unsigned int brom_handle, FLASHTOOL_API_HANDLE_T ft_handle, const Efuse_Secure_Arg *secure_arg, bool error_log_en);
_DEPRECATED_API int __stdcall Brom_WriteEfuseLock(unsigned int brom_handle, FLASHTOOL_API_HANDLE_T ft_handle, const Efuse_Lock_Arg *lock_arg, bool error_log_en);
_DEPRECATED_API int __stdcall Brom_ReadEfuseAll(unsigned int brom_handle, FLASHTOOL_API_HANDLE_T ft_handle, Efuse_Common_Arg *common_arg, Efuse_Secure_Arg *secure_arg, Efuse_Lock_Arg *lock_arg, bool error_log_en);
_DEPRECATED_API int __stdcall Test_WriteEfuse(unsigned int brom_handle, FLASHTOOL_API_HANDLE_T ft_handle);

LIBEXPORT int __stdcall Brom_SetWatchDogTimeout(unsigned int brom_handle, FLASHTOOL_API_HANDLE_T ft_handle, unsigned short timeout);


typedef struct {
    unsigned int    m_baseaddr;
    unsigned char * m_data;
    unsigned int    m_data_len;     // in byte 
    unsigned int    m_data_sig_len; // in byte 
} _DataBlock;

#define BROM_MAX_DATA_BLOCKS    10

typedef struct {
    unsigned short  m_block_count;
    _DataBlock      m_blocks[BROM_MAX_DATA_BLOCKS];
} BromWriteDataBlocks;

LIBEXPORT int __stdcall Brom_DownloadData(unsigned int brom_handle, HANDLE hCOM, const BromWriteDataBlocks *p_data_blocks, CALLBACK_WRITE_BUF_PROGRESS_INIT  cb_init,  void *cb_init_arg, CALLBACK_WRITE_BUF_PROGRESS  cb,  void *cb_arg);
LIBEXPORT int __stdcall Brom_DownloadData_Ex(FLASHTOOL_API_HANDLE_T ft_handle,
                                             FlashTool_BROM_DL_Arg *p_dl_arg,
                                             FlashTool_Brom_DL_Result *p_result);
//------------------------------------------------------------------------------
// BootROM Force Charge Command
//------------------------------------------------------------------------------
typedef enum {
    FORCE_CHARGE_OFF = 0,	// with battery
    FORCE_CHARGE_ON  = 1,	// no battery, powered by usb
    FORCE_CHARGE_AUTO= 2
} FC_TYPE;

#ifdef  __cplusplus
}
#endif

#endif


