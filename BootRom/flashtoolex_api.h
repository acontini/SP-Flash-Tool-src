#ifndef __FLASHTOOL_EX_API__
#define __FLASHTOOL_EX_API__

#if defined(_WIN32)
   #if defined(FLASHTOOLLIBEX_EXPORTS)
       #define LIBEX_EXPORT __declspec(dllexport)
   #else
       #define LIBEX_EXPORT __declspec(dllimport)
   #endif
#elif defined(_LINUX)
   #if defined(FLASHTOOLLIBEX_EXPORTS)
       #define LIBEX_EXPORT __attribute__ ((visibility("default")))
   #else
       #define LIBEX_EXPORT
   #endif
   
   #define __stdcall
#else
   #error "win32 or linux? platform is not specified."
#endif

#define LIB_API __stdcall

#include "flashtoolex_struct.h"


#ifdef  __cplusplus
extern "C" {
#endif

//! <b>Effects</b>: Setup dll environment.
//! such as locale or others.
//!
//! <b>Parameters</b>: none
//!
//! <b>Returns</b>:  0 if success, or other status indicate error.
LIBEX_EXPORT int LIB_API flashtool_startup();

//! <b>Effects</b>: Cleanup dll environment.
//!
//! <b>Parameters</b>: none
//!
//! <b>Returns</b>:  none.
LIBEX_EXPORT void LIB_API flashtool_cleanup();

//! <b>Effects</b>: set the dll logging level.
//! there are 6 levels, default is level 3: kinfo level. 
//!
//! <b>Parameters</b>: level: log level enum.
//!
//! <b>Returns</b>:  none.
LIBEX_EXPORT void LIB_API flashtool_set_log_level(enum logging_level level);

//! <b>Effects</b>: Parsing the scatter file.
//! get the information from the scatter file text.
//!
//! <b>Parameters</b>: scatter: the scatter file path name & remote file callbacks if need.
//!   info: the output variable, the information of the scatter file is put in here.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_get_scatter_info(const file_info* scatter, /*IN OUT*/struct scatter_file_info_t* info);

//! <b>Effects</b>: wait for device and get the COM port name.
//! If the correct device connected PC with USB via virtual COM,
//! the function will return, or it will block until timeout.
//!
//! <b>Parameters</b>: filter: the clue for device searching. like "USB\\VID_0E8D&PID_0003" array.
//!   count: the array count. 
//!   port_name: the COM port name returned in here.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_waitfor_com(const char8** filter, uint32 count, /*OUT*/char8* port_name);

//! <b>Effects</b>: the session of unique device.
//! one connected device has single session, all operations must related with a session. 
//!
//! <b>Parameters</b>: NULL
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT HSESSION LIB_API flashtool_create_session();

//! <b>Effects</b>: destroy the session.
//! If a device is disconnected, the session must be destroyed to release resources.
//!
//! <b>Parameters</b>: hs: the session handle.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_destroy_session(HSESSION hs);

//! <b>Effects</b>: connect with brom or loader.
//! handshake with loader, notify the loader to enter BROM download mode. 
//!
//! <b>Parameters</b>: hs: the session handle.
//!   port_name: the port name such as COM name with which to connect the device. 
//!   auth: the authentication file, used to determine whether the tool having the right to connect loader.
//!         if the chip is set to ignore this secure policy, this parameter can be NULL.
//!   cert: security switch. for special guy debug.
//!   callbacks: the callback functions used in brom operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_connect_brom(HSESSION hs, const char8* port_name, 
                                                     const file_info* auth, const file_info* cert,
                                                     const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: download the Download-Agent file and enter DA download mode.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   da_name: Download-Agent file path name.
//!   emi_container:  this file is used for initialize external RAM. normally is preloader.
//!   cfg_file: the modules description file.
//!             usually named device_parameters.xml
//!   connect_params: the parameters used in setting dll and DA environment.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_connect_da(HSESSION hs,                                       
                                      const file_info* da_name,
                                      const file_info* emi_container,
                                      const file_info* dev_parameters_cfg,
                                      const struct connect_params_struct_t* connect_params,
                                      const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: some commands can be executed by loader or DA.
//!  this is extension for API.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   ctrl_code: function control code indicate the action.
//!   inbuffer: input parameter.
//!   inbuffer_size: input buffer size.
//!   outbuffer: output parameter.
//!   outbuffer_size: output buffer size.
//!   bytes_returned: if some value is returned in outbuffer, the actual size is put here.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_device_control(HSESSION hs, 
                                               enum device_control_code ctrl_code,
                                               const pvoid inbuffer,
                                               uint32 inbuffer_size,
                                               pvoid outbuffer,
                                               uint32 outbuffer_size,
                                               uint32* bytes_returned);

//! <b>Effects</b>: Get device hw parameters.
//! include externam RAM, emmc, nand, nor, ufs storage. 
//!
//! <b>Parameters</b>: hs: the session handle.
//!   info: the hw parameters are returned here. 
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_get_device_info(HSESSION hs, /*IN OUT*/struct device_info_t* info);

//! <b>Effects</b>: when connect to BROM mode, the USB speed is Full Speed.
//!      this function switch USB to High Speed in DA mode. 
//!
//! <b>Parameters</b>: hs: the session handle.
//!   filter: the clue for new USB searching. like "USB\\VID_0E8D&PID_2001" array.
//!   count: the filter count.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_switch_com(HSESSION hs, const char8** filter, uint32 count);

//! <b>Effects</b>: when finish action with device.
//!      Call this function to disconnect with device, or reboot device.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   method: reboot device or just disconnect USB.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_shutdown_device(HSESSION hs, struct reboot_option option);

//! <b>Effects</b>: Download images to device.
//! Could download images according to scatter file. 
//! Or just download some file to specified partition.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   scatter: the scatter file path name & remote file callbacks if need.
//!                      If just want to download some partitions. set this NULL. 
//!   flist: the files infomation, include file path and the partition name to which the file is written.
//!   count: the flist count.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_download(HSESSION hs, const file_info* scatter, 
                                      const struct op_part_list_t* flist, uint32 count
                                      ,const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: Upgrade images to device.
//! Could download images according to scatter file. 
//! Or just download some file to specified partition.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   scatter_file_name: put the scatter file path in this.
//!                      Can not be NULL.
//!   flist: the files infomation, include file path and the partition name to which the file is written.
//!   count: the flist count.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_upgrade(HSESSION hs, const scatter_file_info_t *scatter_info, enum upgrade_type type, 
                                                  const struct callbacks_struct_t* callbacks, const struct remote_file_callbacks* remote_cbs,
                                                  const CB_CHECKSUM_VERIFY cb_checksum_verify);

//! <b>Effects</b>: upload partitions data from device.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   flist: the files infomation, include the partition name and file path to which the file is written.
//!   count: the flist count.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_upload(HSESSION hs, struct partition_info_list_t* flist, 
                                               const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: readback partitions data from device.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   wlist: include more detailed control information.
//!   count: the wlist count.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_readback(HSESSION hs, op_data_list_t* rlist, 
                                                 uint32 count
                                                 ,const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: Write images to device.
//!        Write data to specified aera of device.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   wlist: include detailed control information.
//!   count: the wlist count.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_write_data(HSESSION hs, struct op_data_list_t* wlist, 
                                                   uint32 count
                                                   ,const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: Format specified partition of device.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   part_name: partition name to format. if input "all", logical format whole flash will be done.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_format_partition(HSESSION hs, const char8* part_name
                                                      ,const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: Format specified aera of device storage.
//!
//! <b>Parameters</b>: hs: the session handle.
//!   type: Indicate the format method.
//!         If this is MANUAL, 'para' argument must be filled. 
//!         If this is AUTO_xx, 'para' argument is NULL.
//!   para: the control information.
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_format(HSESSION hs, enum erase_type type, 
                                        const struct format_params_struct* para
                                        ,const struct callbacks_struct_t* callbacks);

//! <b>Effects</b>: Do test for both RAM and storage
//!
//! <b>Parameters</b>: hs: the session handle.
//!   type: Indicate test method.
//!         If this is TEST_MEMORY, do test on memory(RAM) 
//!         If this is TEST_STORAGE, do test on storage(eMMC, NAND. NOR...)
//!   addr: memory test start addr
//!   len:   memory test length
//!   callbacks: the callback functions used in device operations.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_memory_test(HSESSION hs, memory_type type,
													uint64 addr, uint64 len,
                                                    const struct callbacks_struct_t* callbacks);
 
//! <b>Effects</b>: Test class function.
//!
//! <b>Parameters</b>: class_name: the class name to test.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_test(const char8* class_name);

//! <b>Effects</b>: read the partition table from device.
//!             It can be data in GPT or PMT
//! <b>Parameters</b>: hs: the session handle.
//!            pt_list: the partitions information returned here.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_read_partition_table(HSESSION hs, partition_info_list_t* pt_list);

//! <b>Effects</b>: Get this DLL's info.
//! 
//! <b>Parameters</b>: info: the information returned here
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_get_lib_info(dll_info_t* info);

//! <b>Effects</b>: Read, write and lock OTP(one time program zone).
//! 
//! <b>Parameters</b>: hs: the session handle. 
//!                    otp_action: Read, Write, Lock action.
//!                    data:  buffer contain the data to write or read back.
//!                    param: contain the data length and R/W offset to otp group block.
//!
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_otp_zone_operation(HSESSION hs, enum otp_action, uint8* data, uint32 length, uint32 offset);

//! <b>Effects</b>: Write Efuse register.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_efuse_read_all(HSESSION hs,
                                                       struct efuse_argument *efuse_arg);

//! <b>Effects</b>: Read Efuse register values.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_efuse_write_all(HSESSION hs,
                                                        struct efuse_argument *efuse_arg);
//! <b>Effects</b>: Setup LAN Port callback functions
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.

LIBEX_EXPORT status_t LIB_API flashtool_customize_impl(enum customized_type type, void *callbacks);
//! <b>Effects</b>: Calculate all checksum of ALPS load.
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.

LIBEX_EXPORT status_t LIB_API flashtool_generate_checksum(const struct chksum_info_struct* info, const char *scatter_file_path, const struct callbacks_struct_t* callbacks);
//! <b>Effects</b>: Checksum verification 
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.

LIBEX_EXPORT status_t LIB_API flashtool_image_checksum(const file_info *info, const struct callbacks_struct_t* callbacks);
//! <b>Effects</b>: set boot mode flag in PRO_INFO, 
//!                           lk will use this flag to decide if need do handshake when entering Meta Mode.
//! <b>Parameters</b>: hs: the session handle. 
//!                               mode: 0---normal mode
//!                                       1---meta mode
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.

LIBEX_EXPORT status_t LIB_API flashtool_set_boot_flag(HSESSION hs, unsigned char mode, unsigned char com_type, unsigned char com_id);

//! <b>Effects</b>: check if RPMB partition has been written
//! <b>Parameters</b>: hs:       the session handle. 
//!                                rpmb_status: the result para
//!
//! <b>Returns</b>:  STATUS_OK if success, or other status indicate specified error.
LIBEX_EXPORT status_t LIB_API flashtool_get_rpmb_status(HSESSION hs, flash_hw_status_e* rpmb_status);


LIBEX_EXPORT void LIB_API flashtool_get_last_errmsg(HSESSION hs, char* err_msg);

LIBEX_EXPORT status_t LIB_API flashtool_generate_gpx(HSESSION hs, gpx_struct* gpx, uint8 gpx_count);

LIBEX_EXPORT status_t LIB_API flashtool_pl_chanllenge(HSESSION hs, DATA_DIRECTION_E direct, unsigned char* databuf, unsigned int databuf_len);

#ifdef  __cplusplus
}
#endif

#endif
