#ifndef __FLASHTOOL_EX_STRUCT__
#define __FLASHTOOL_EX_STRUCT__

#include "common_struct.h"

#if defined(_LINUX)  
   #define __stdcall
#endif

typedef enum logging_level
{
    ktrace = 0,
    kdebug,
    kinfo,
    kwarning,
    kerror,
    kfatal
}logging_level_e;

typedef enum upgrade_type
{
	STRATEGY_FORMAT_ALL_DL,
	STRATEGY_DOWNLOAD_ONLY,
	STRATEGY_FIRMWARE_UPGRADE,
}download_strategy;

enum rom_operation_type
{
    ROM_OPER_BOOTLOADERS = 0,
    ROM_OPER_UPDATE,
    ROM_OPER_INVISIBLE,
    ROM_OPER_PROTECTED,
    ROM_OPER_RESERVED,
    ROM_OPER_BINREGION,
    ROM_OPER_UNKNOWN,
    ROM_OPER_END
};

enum rom_type
{
    TYPE_NORMAL_ROM      = 0x01,     
    TYPE_SV5_BL_BIN      = 0x07,
    TYPE_YAFFS_IMG       = 0x08,
    /* Add UBIFS image support, 2012/12/18 {*/
    TYPE_UBI_IMG         = 0x10,
    /* Add UBIFS image support, 2012/12/18 }*/
    /* Add EXT4 image support for eMMC, 2013/05/13 {*/
    TYPE_EXT4_IMG        = 0x11,
    /* Add EXT4 image support for eMMC, 2013/05/13 }*/
    /* SD Card on EMMC {*/
    TYPE_FAT_IMG         = 0x12,
    /* SD Card on EMMC }*/
    /* Add F2FS image {*/
    TYPE_F2FS_IMG      = 0x13,

    TYPE_FTL20_IMG      = 0x20,
    /* Add F2FS image {*/
     TYPE_UNKNOWN_BIN = 0xFF
};

#define PARAM_BUF_SIZE 64
#define MAX_FILE_NAME_LEN 512
typedef struct partition_info_t
{
   char8 part_name[PARAM_BUF_SIZE];
   char8 file_name[MAX_FILE_NAME_LEN];
   uint64 start_address;
   uint64 size;
   uint32 storage;
   uint32 region;
   uint32 operation_type;
   BOOL is_download;
   BOOL enable;
   BOOL is_reserved;
   /*information in attribute will be record by bit*/
   //bit1:
   //bit2:...
   uint32 attribute;
   uint32 d_type; //NAND scatter file, FULL_PAGE/LOW_PAGE
   uint32 slc_percentage;
   uint32 rom_type; 
}partition_info_t;

typedef struct partition_info_list_t
{
   uint32 count;
   partition_info_t part[MAX_PARTITION_COUNT];
}partition_info_list_t;

typedef struct scatter_file_info_t
{
   struct header_t
   {
      char8 version[PARAM_BUF_SIZE];
      char8 platform[PARAM_BUF_SIZE];
	  char8 chip_version[PARAM_BUF_SIZE];
      char8 storage[PARAM_BUF_SIZE];
      uint32 partition_count;
   }hdr;
   partition_info_t part[MAX_PARTITION_COUNT]; //only visable partitions
}scatter_file_info_t;

typedef struct op_part_list_t
{
   char8 part_name[PARAM_BUF_SIZE];   
   char8 file_path[MAX_FILE_NAME_LEN];
}op_part_list_t;

typedef enum op_data_source
{
    OP_SOURCE_FILE = 0,
    OP_SOURCE_MEMORY,
    OP_SOURCE_REMOTE
}op_data_source_e;

typedef struct op_data_list_t
{
   enum storage_type storage;
   uint32 section;
   uint64 address;   
   uint64 length;
   enum op_data_source source;// FILE use file_path, MEM use user_buffer
   uint8* user_buffer;
   char8 file_path[MAX_FILE_NAME_LEN];   
   struct partition_extension_struct extension;
}op_data_list_t;

typedef struct format_params_struct
{
   enum storage_type storage;
   uint32 section;
   uint64 address;
   uint64 length;
   struct partition_extension_struct extension;
}format_params_struct;

typedef enum transfer_phase
{
    TPHASE_INIT = 0
   ,TPHASE_DA
   ,TPHASE_LOADER
   ,TPHASE_IMAGE
   ,TPHASE_HOST_CHECKSUM
   ,TPHASE_TARGET_CHECKSUM
   ,TPHASE_FORMAT
   ,TPHASE_READBACK
   ,TPHASE_WRITE_MEMORY
   ,TPHASE_MEMORY_TEST
}transfer_stage_e;

struct cbs_additional_info
{
   char image_name[MAX_FILE_NAME_LEN]; //add for checksum initilize callback
};

typedef void (__stdcall *CB_OPERATION_PROGRESS)(pvoid _this, enum transfer_phase phase, 
                                                uint32 progress, 
                                                uint64 data_xferd,
                                                uint64 data_total,
                                                const struct cbs_additional_info *additional_info);
typedef void (__stdcall *CB_STAGE_MESSAGE)(pvoid _this, const char8* message);
typedef BOOL (__stdcall *CB_NOTIFY_STOP)(pvoid _this);
typedef int (__stdcall *CB_REMOTE_READ_FILE)(const char *filepath, unsigned long long offset, unsigned char * const p_file_content, unsigned int package_length, void *user_arg);
typedef int (__stdcall *CB_REMOTE_GET_FILE_LENGTH)(const char *filepath, unsigned long long * const p_file_length, void *user_arg);
typedef int (__stdcall *CB_CHECKSUM_VERIFY)(const char* partition_name, unsigned int checksum);
typedef int (__stdcall *CB_SLA_CHALLENGE)
(
 void *usr_arg ,
 const unsigned char *p_challenge_in,
 unsigned int challenge_in_len,
 unsigned char **pp_challenge_out,
 unsigned int *p_challenge_out_len
 );
 typedef int (__stdcall *CB_SLA_CHALLENGE_END)
 (
 void *usr_arg,
 unsigned char *p_challenge_out
 );


struct remote_file_callbacks
{
   CB_REMOTE_READ_FILE cb_read_file;
   CB_REMOTE_GET_FILE_LENGTH cb_get_file_length;
};

typedef struct sla_callbacks_t
{
   CB_SLA_CHALLENGE cb_start;
   CB_SLA_CHALLENGE_END cb_end;
   void* start_user_arg;
   void* end_user_arg;
}sla_callbacks_t;

typedef struct callbacks_struct_t
{
   pvoid _this;
   CB_OPERATION_PROGRESS cb_op_progress;
   CB_STAGE_MESSAGE cb_stage_message;
   CB_NOTIFY_STOP cb_notify_stop;      
   sla_callbacks_t cb_sla;
}callbacks_struct_t;

typedef enum
{
   CONN_FIRST_DA = 1,
   CONN_SECOND_DA = 2
}connect_da_end_stage_e;

typedef struct connect_params_struct_t
{
   uint32 checksum_level;
   uint32 battery_setting;
   uint32 reset_key_setting;
   uint32 connect_da_end_stage;
   BOOL   enable_dram_in_1st_da;
   uint32 da_log_level;
}connect_params_struct_t;

//The device info struct size is 8K.
//If want to add more info in this, add at the tail.
//Do NOT insert.
typedef struct device_info_t
{
   union
   {
      char8 STUB[8192];
      struct
      {
         struct mmc_info_struct mmc;
         struct nand_info_struct nand;
         struct nor_info_struct nor;
         struct ufs_info_struct ufs;
         struct ram_info_struct ram;
         struct chip_id_struct chip_id;
         unsigned int random_id[4];
         //add more info here if needed.
      };
   };
}device_info_t;

typedef enum erase_type
{
    AUTO_ALL = 0
   ,AUTO_EXCEPT_BL
   ,AUTO_PHYSICAL_ALL
   ,AUTO_PHYSICAL_EXC_BL
   ,AUTO_FORCE_ALL
   ,AUTO_FORCE_EXC_BL
   ,MANUAL
   ,ERASE_UNKNOWN_TYPE
}erase_str_e;

typedef struct dll_info_t
{
   char8* version;
   char8* release_type;
   char8* build_data;
   char8* change_list;
   char8* builder;
}dll_info_t;

typedef enum emmc_hwreset_value
{
	EMMC_HWRST_TMEP_DISABLE = 0,
	EMMC_HWRST_PERM_ENABLE = 1,
	EMMC_HWRST_PERM_DISABLE = 2
}emmc_hwreset_value;

//brom & DA device control 
typedef enum device_control_code
{
    DEV_GET_CHIP_ID = 1,
    DEV_BROM_WDT_RESET,
    DEV_BROM_SEND_DATA_TO,
    DEV_BROM_JUMP_TO,
    DEV_BROM_GET_MODEM_INFO,
    DEV_BROM_WRITE_MEM16,
    DEV_BROM_WRITE_MEM32,
    DEV_BROM_READ_MEM16,
    DEV_BROM_READ_MEM32,
    DEV_DA_GET_USB_SPEED = 0x100,
    DEV_DA_GET_BAT_VOLTAGE,
    DEV_DA_DISABLE_EMMC_HWRESET_PIN,
    DEV_DA_SET_EMMC_HWRESET_PIN,
}device_control_code_e;

struct brom_send_data_param
{	
   uint8*   data;
   uint32   length;
   uint32   at_address;
};

struct brom_wdt_reg_addr
{
	uint32 reg_wdt_mode;
	uint32 reg_wdt_length;
	uint32 reg_wdt_restart;
	uint32 reg_wdt_swrst;
};

struct write_memory_parameter
{
   uint32 address;
   pvoid data;
   uint32 count;
};

struct read_memory_parameter
{
   uint32 address;
   pvoid data;
   uint32 count;
};
//same as internal section_block_t. do not modify this struct alone.
struct execute_data_parameter
{	
   pvoid data;
   uint32 length;
   uint32 at_address;
   uint32 sig_offset;
   uint32 sig_length;
};

enum customized_type
{
   TYPE_LGE = 0
   ,TYPE_UNKNOWN
};

struct file_content_buffer
{
   uint8* buffer;
   uint32 length;
};

struct file_info
{
   op_data_source input_type;
   char8 file_path[MAX_FILE_NAME_LEN];
   struct remote_file_callbacks cbs;
   CB_CHECKSUM_VERIFY cb_checksum_verify;
   struct file_content_buffer buffer;
};

typedef struct chksum_info_struct
{
   char version[4];
   char tool_version[16];
}chksum_info_struct_t;

typedef struct gpx_struct
{
   uint8 index; // 0,1,2,3 --> gp1, gp2, gp3, gp4
   uint64 start_address;
   uint64 length;
}gpx_struct;

typedef enum DATA_DIRECTION_E{
	DATA_DIRECT_E_INPUT = 0,
	DATA_DIRECT_E_OUTPUT
}DATA_DIRECTION_E;

#endif
