#include "DADownloadAll.h"
#include "../BootRom/flashtool.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Conn/Connection.h"
#include "../Utility/Utils.h"

#include "../Logger/Log.h"
#include "../Err/Exception.h"

#ifdef QT_DEBUG
int __stdcall cb_download_progress(
    unsigned char finished_percent,
    unsigned int finished_bytes,
    unsigned int total_bytes, void *)
{
    static bool started = false;
    static unsigned char last_percent = 0;

    if(!started)
    {
        LOGI("Starting images download...");
        started = true;
    }
    if(finished_percent != last_percent && finished_percent % 5 == 0)
    {
        LOGI("%d of %d bytes were sent... %f%",
             finished_bytes, total_bytes, finished_percent);

        last_percent = finished_percent;
    }
    return 0;
}

#endif
namespace APCore
{

DADownloadAll::DADownloadAll(APKey key)
    : ICommand(key)
{
}

DADownloadAll::~DADownloadAll()
{
}

void DADownloadAll::exec(const QSharedPointer<Connection> &conn)
{
    LOGI("executing DADownloadAll...");

    conn->ConnectDA();

    download_arg.set_dl_handle(GET_DL_HANDLE_T(key_));

    FlashTool_Download_Arg fdl_arg;
    FlashTool_Download_Result fdl_res;

    memset( &fdl_arg,    0, sizeof(fdl_arg));
    memset( &fdl_res, 0, sizeof(fdl_res));

    fdl_arg = download_arg.GetBromDownloadArg();

    LOGD("Start downloading via DA...");

    download_clock.restart();

    int ret = FlashTool_Download(
                conn->FTHandle(),&fdl_arg,&fdl_res);

    LOGD("Download result(%d)",ret);

    if(ret != S_DONE)
    {
#ifndef _WIN32
        /*
          sometimes the exception won't be caught
          call 'sleep' here to make it better...
        */
        //sleep(1);
#endif
        LOGI("Download failed.");
        THROW_BROM_EXCEPTION(ret,0);
    }

    unsigned long download_time = download_clock.elapsed();
    LOGD("download time: %d s", download_time/1000);
    U64 file_size(0);
    std::string download_speed;

    ret = Utils::GetRomFilesTotalSize(fdl_arg.m_dl_handle, &file_size);
    if(ret != S_DONE)
    {
        LOGE("Get File Total Size failed.(%d)", ret);
        THROW_BROM_EXCEPTION(ret,0);
    }

    if(download_time>0)
    {
         download_speed =Utils::PrettyByteString(file_size * 1000 /download_time);
         LOGI("download speed: %sB/s.", download_speed.c_str());
    }
}

}
