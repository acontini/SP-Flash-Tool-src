#include <memory.h>
#include <iostream>

#include "../Arg/BromDownloadArg.h"
#include "../Logger/Log.h"
#include "../BootRom/flashtool.h"
#include "../Utility/Utils.h"

//Brom Download Argument
BromDownloadArg::BromDownloadArg()
{
    this->FillDefaultVaule();
}

BromDownloadArg::BromDownloadArg(const FlashTool_Download_Arg& download_arg)
{
    memset( &flashtool_download_arg_, 0, sizeof(FlashTool_Download_Arg));

    this->flashtool_download_arg_.m_dl_handle          = download_arg.m_dl_handle;
    this->flashtool_download_arg_.m_dl_handle_list     = download_arg.m_dl_handle_list;

    //DA report callback
    this->flashtool_download_arg_.m_cb_da_report        = download_arg.m_cb_da_report;
    this->flashtool_download_arg_.m_cb_da_report_arg    = download_arg.m_cb_da_report_arg;

    // download progress callback
    this->flashtool_download_arg_.m_download_accuracy                   = download_arg.m_download_accuracy;
    this->flashtool_download_arg_.m_cb_download_flash_init              = download_arg.m_cb_download_flash_init;
    this->flashtool_download_arg_.m_cb_download_flash_init_arg          = download_arg.m_cb_download_flash_init_arg;
    this->flashtool_download_arg_.m_cb_download_flash                   = download_arg.m_cb_download_flash;
    this->flashtool_download_arg_.m_cb_download_flash_arg               = download_arg.m_cb_download_flash_arg;
    this->flashtool_download_arg_.m_cb_security_post_process_notify     = download_arg.m_cb_security_post_process_notify;
    this->flashtool_download_arg_.m_cb_security_post_process_notify_arg = download_arg.m_cb_security_post_process_notify_arg;

    // Boot-Loader download progress callback
    this->flashtool_download_arg_.m_cb_download_bloader_init       = download_arg.m_cb_download_bloader_init;
    this->flashtool_download_arg_.m_cb_download_bloader_init_arg   = download_arg.m_cb_download_bloader_init_arg;
    this->flashtool_download_arg_.m_cb_download_bloader            = download_arg.m_cb_download_bloader;
    this->flashtool_download_arg_.m_cb_download_bloader_arg        = download_arg.m_cb_download_bloader_arg;
    this->flashtool_download_arg_.m_enable_tgt_res_layout_check   = download_arg.m_enable_tgt_res_layout_check;
    this->flashtool_download_arg_.m_enable_bbchip_ver_check       = download_arg.m_enable_bbchip_ver_check;
    this->flashtool_download_arg_.m_downloadstyle_sequential      = download_arg.m_downloadstyle_sequential;

}

void BromDownloadArg::FillDefaultVaule(void)
{
    memset( &flashtool_download_arg_, 0, sizeof(FlashTool_Download_Arg));

    this->flashtool_download_arg_.m_dl_handle          = NULL;
    this->flashtool_download_arg_.m_dl_handle_list     = NULL;

    //DA report callback
    this->flashtool_download_arg_.m_cb_da_report        = cb_da_report;
    this->flashtool_download_arg_.m_cb_da_report_arg    = NULL;

    // download progress callback
    this->flashtool_download_arg_.m_download_accuracy                   = ACCURACY_AUTO;
    this->flashtool_download_arg_.m_cb_download_flash_init              = cb_download_flash_init;
    this->flashtool_download_arg_.m_cb_download_flash_init_arg          = NULL;
    this->flashtool_download_arg_.m_cb_download_flash                   = cb_download_flash;
    this->flashtool_download_arg_.m_cb_download_flash_arg               = NULL;
    this->flashtool_download_arg_.m_cb_security_post_process_notify     = NULL;
    this->flashtool_download_arg_.m_cb_security_post_process_notify_arg = NULL;

    // Boot-Loader download progress callback
    this->flashtool_download_arg_.m_cb_download_bloader_init      = cb_download_bloader_init;
    this->flashtool_download_arg_.m_cb_download_bloader_init_arg  = NULL;
    this->flashtool_download_arg_.m_cb_download_bloader           = cb_download_bloader;
    this->flashtool_download_arg_.m_cb_download_bloader_arg       = NULL;
    this->flashtool_download_arg_.m_enable_tgt_res_layout_check   = _FALSE;
    this->flashtool_download_arg_.m_enable_bbchip_ver_check       = _FALSE;
    this->flashtool_download_arg_.m_downloadstyle_sequential      = _FALSE;

    //chksum callback
    this->flashtool_download_arg_.m_cb_checksum_init = cb_da_chksum_init;
    this->flashtool_download_arg_.m_cb_checksum = cb_da_chksum_progress;
    this->flashtool_download_arg_.m_cb_checksum_init_arg = NULL;
    this->flashtool_download_arg_.m_cb_checksum_arg = NULL;
}

int BromDownloadArg::cb_da_report(const DA_REPORT_T *p_da_report, void *usr_arg)
{
    LOGI("cb_da_report(): DA report: m_da_report addr: %x, chip name: %s, chip type: %d. ext ram size: %x",
            p_da_report, p_da_report->m_bbchip_name,
            p_da_report->m_bbchip_type, p_da_report->m_ext_ram_size);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromDownloadArg::cb_download_bloader_init(void* usr_arg)
{
    LOGI("Downloading bootloader...");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromDownloadArg::cb_download_bloader(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void * /*usr_arg*/)
{
    static unsigned char lastReportedProgress = 0;

    if ((finished_percentage != lastReportedProgress) &&
        (finished_percentage % 10 == 0))
    {
        LOGI("%d%% of bootloader has been sent (%s of %s)",
             (int) finished_percentage,
             Utils::PrettyByteString(finished_bytes).c_str(),
             Utils::PrettyByteString(total_bytes).c_str());

        lastReportedProgress = finished_percentage;
    }

    return 0;
}

int BromDownloadArg::cb_download_flash_init(void* usr_arg)
{
    LOGI("Downloading images...");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromDownloadArg::cb_download_flash(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void * /*usr_arg*/)
{
    static unsigned char lastReportedProgress = 0;

    if (finished_percentage != lastReportedProgress)
    {
        // update progress for about every 5M
        unsigned char freq =
                (5*1024*1024)*100 / total_bytes;

        freq = freq > 10 ? 10 : freq;
        freq = freq < 1  ? 1  : freq;

        if (finished_percentage % freq == 0)
        {
            LOGI("%d%% of image data has been sent (%s of %s)",
                 (int) finished_percentage,
                 Utils::PrettyByteString(finished_bytes).c_str(),
                 Utils::PrettyByteString(total_bytes).c_str());

            lastReportedProgress = finished_percentage;
        }
    }

    return 0;
}

int BromDownloadArg::cb_da_chksum_init(void* usr_arg, const char* image_name)
{
    LOGI("Caculate checksum for image(%s)...", image_name);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromDownloadArg::cb_da_chksum_progress(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void * /*usr_arg*/)
{
    static unsigned char lastReportedProgress = 0;

    if ((finished_percentage != lastReportedProgress) &&
        (finished_percentage % 10 == 0))
    {
        LOGI("%d%% of image data has been caculated (%s of %s)",
             (int) finished_percentage,
             Utils::PrettyByteString(finished_bytes).c_str(),
             Utils::PrettyByteString(total_bytes).c_str());

        lastReportedProgress = finished_percentage;
    }

    return 0;
}


void BromDownloadArg::Dump()
{
    LOG("m_dl_handle(0x%x).", flashtool_download_arg_.m_dl_handle);
    LOG("m_dl_handle_list(0x%x).", flashtool_download_arg_.m_dl_handle_list);

    //DA report callback
    LOG("m_cb_da_report(0x%x).", flashtool_download_arg_.m_cb_da_report);
    LOG("m_cb_da_report_arg(0x%x).", flashtool_download_arg_.m_cb_da_report_arg);

    // download progress callback
    LOG("m_download_accuracy(0x%x).", flashtool_download_arg_.m_download_accuracy);
    LOG("m_cb_download_flash_init(0x%x).", flashtool_download_arg_.m_cb_download_flash_init);
    LOG("m_cb_download_flash_init_arg(0x%x).", flashtool_download_arg_.m_cb_download_flash_init_arg);
    LOG("m_cb_download_flash(0x%x).", flashtool_download_arg_.m_cb_download_flash);
    LOG("m_cb_download_flash_arg(0x%x).", flashtool_download_arg_.m_cb_download_flash_arg);
    LOG("m_cb_security_post_process_notify(0x%x).", flashtool_download_arg_.m_cb_security_post_process_notify);
    LOG("m_cb_security_post_process_notify_arg(0x%x).", flashtool_download_arg_.m_cb_security_post_process_notify_arg);

    // Boot-Loader download progress callback
    LOG("m_cb_download_bloader_init(0x%x).", flashtool_download_arg_.m_cb_download_bloader_init);
    LOG("m_cb_download_bloader_init_arg(0x%x).", flashtool_download_arg_.m_cb_download_bloader_init_arg);
    LOG("m_cb_download_bloader(0x%x).", flashtool_download_arg_.m_cb_download_bloader);
    LOG("m_cb_download_bloader_arg(0x%x).", flashtool_download_arg_.m_cb_download_bloader_arg);
    LOG("m_enable_tgt_res_layout_check(%d).", flashtool_download_arg_.m_enable_tgt_res_layout_check);
    LOG("m_enable_bbchip_ver_check(%d).", flashtool_download_arg_.m_enable_bbchip_ver_check);
    LOG("m_enable_bbchip_ver_check(%d).", flashtool_download_arg_.m_downloadstyle_sequential);
}
