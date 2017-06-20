#ifndef BROMDOWNLOADARG_H
#define BROMDOWNLOADARG_H


#include "../BootRom/flashtool_api.h"

class BromDownloadArg
{
public:
    BromDownloadArg(void);
    explicit BromDownloadArg(const FlashTool_Download_Arg& download_arg);

    FlashTool_Download_Arg GetBromDownloadArg() const
    {
        return flashtool_download_arg_;
    }
    //Setters
    void set_dl_handle(const DL_HANDLE_T dl_handle) {
        this->flashtool_download_arg_.m_dl_handle = dl_handle;
    }
    void set_dl_handle_list(const DL_HANDLE_LIST_T dl_handle_list) {
        this->flashtool_download_arg_.m_dl_handle_list = dl_handle_list;
    }

    void set_cb_da_report(const CALLBACK_DA_REPORT cb_da_report) {
        this->flashtool_download_arg_.m_cb_da_report = cb_da_report;
    }
    void set_cb_da_report_arg(void* host_pointer) {
        this->flashtool_download_arg_.m_cb_da_report_arg = host_pointer;
    }
    void set_cb_download_flash_init(const CALLBACK_DOWNLOAD_PROGRESS_INIT cb_download_flash_init) {
        this->flashtool_download_arg_.m_cb_download_flash_init = cb_download_flash_init;
    }
    void set_cb_download_flash_init_arg(void* host_pointer) {
        this->flashtool_download_arg_.m_cb_download_flash_init_arg = host_pointer;
    }
    void set_cb_download_flash(const CALLBACK_DOWNLOAD_PROGRESS cb_download_flash) {
        this->flashtool_download_arg_.m_cb_download_flash = cb_download_flash;
    }
    void set_cb_download_flash_arg(void* host_pointer) {
        this->flashtool_download_arg_.m_cb_download_flash_arg = host_pointer;
    }
    void set_cb_da_chksum_init(const CALLBACK_CHECKSUM_PROGRESS_INIT cb)
    {
        this->flashtool_download_arg_.m_cb_checksum_init = cb;
    }   
    void set_cb_da_chksum_progress(const CALLBACK_CHECKSUM_PROGRESS cb)
    {
        this->flashtool_download_arg_.m_cb_checksum = cb;
    }
    void set_cb_da_chksum_init_arg(void* host_pointer)
    {
        this->flashtool_download_arg_.m_cb_checksum_init_arg = host_pointer;
    }
    void set_cb_da_chksum_arg(void* host_pointer)
    {
        this->flashtool_download_arg_.m_cb_checksum_arg = host_pointer;
    }
    void set_cb_security_post_process_notify(const CALLBACK_SECURITY_POST_PROCESS_NOTIFY cb_security_post_process_notify) {
        this->flashtool_download_arg_.m_cb_security_post_process_notify = cb_security_post_process_notify;
    }
    void set_cb_security_post_process_notify_arg(void* host_pointer) {
        this->flashtool_download_arg_.m_cb_security_post_process_notify_arg = host_pointer;
    }
    void set_download_bloader_init(const CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS_INIT download_bloader_init) {
        this->flashtool_download_arg_.m_cb_download_bloader_init = download_bloader_init;
    }
    void set_cb_download_bloader_init_arg(void* host_pointer) {
        this->flashtool_download_arg_.m_cb_download_flash_init_arg = host_pointer;
    }
    void set_cb_download_bloader(const CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS cb_download_bloader) {
        this->flashtool_download_arg_.m_cb_download_bloader = cb_download_bloader;
    }
    void set_cb_download_bloader_arg(void* host_pointer) {
        this->flashtool_download_arg_.m_cb_download_flash_arg = host_pointer;
    }

    void Dump(void);
public:
    //Console callback functions
    static int __stdcall cb_download_bloader_init(void* usr_arg);
    static int __stdcall cb_download_bloader(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);
    static int __stdcall cb_download_flash_init(void* usr_arg);
    static int __stdcall cb_download_flash(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);
    static int __stdcall cb_da_chksum_init(
        void* usr_arg, const char* image_name);
    static int __stdcall cb_da_chksum_progress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);
    static int __stdcall cb_da_report(const DA_REPORT_T *p_da_report,
                                      void *usr_arg);


private:
    void FillDefaultVaule(void);

private:
    FlashTool_Download_Arg flashtool_download_arg_;
};

#endif // BROMDOWNLOADARG_H
