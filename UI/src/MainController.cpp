#include "MainController.h"
#include "MainWindow.h"
#include "../../Logger/Log.h"
#include "../../Err/Exception.h"
#include "../../Err/BromException.h"
#include "../../Err/FlashToolErrorCodeDef.h"
#include "ICallback.h"
#include "AsyncResourceLoader.h"
#include "BackgroundWorker.h"
#include "MainWindowCallback.h"
#include "../../Utility/FileUtils.h"
#include "../../Utility/IniItem.h"
#include "../../Conn/Connection.h"
#include "../../BootRom/brom.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "connob.h"

#include <QMetaType>

//MainController implementation...
MainController::MainController(QObject *parent, MainWindow *window) :
    QObject(parent),
    main_window(window),
    app_core(new AppCore()),
    advance_on_(false),
    markblocks_on_(false),
    platform_setting(new APCore::PlatformSetting()),
    background_worker(NULL)
{
    session_id = app_core->NewKey();
    LOG("session id = %d", session_id);

    async_loader = new AsyncResourceLoader(session_id,app_core);

    qRegisterMetaType<std::string>("std::string");
    //redirecting the err signal to MainController's
    connect(async_loader, SIGNAL(signal_err_msg(int, const std::string&)),
            this,SIGNAL(signal_err_msg(int, const std::string&)));
    connect(this, SIGNAL(signal_GetDAReport(const DA_REPORT_T*, const std::string&, const int)),
            main_window, SLOT(slot_GetDAReport(const DA_REPORT_T*,const std::string&, const int)));

    connect(this, SIGNAL(signal_GetBootResult(const BOOT_RESULT*,const std::string&)),
            main_window, SLOT(slot_GetBootResult(const BOOT_RESULT*,const std::string&)));

    //create the thread first
    CreateNewThread();
}

MainController::~MainController()
{
    if(background_worker != NULL)
    {
        delete background_worker;
    }

    delete app_core;
    delete async_loader;
}

bool MainController::LoadDA(const QString& file_name)
{
    try
    {
        app_core->LoadDA(session_id, file_name.toLocal8Bit().constData());
    }
    catch(const BaseException& exception)
    {
        emit signal_err_msg(FT_LOAD_DA_ERROR, exception.err_msg());
        return false;
    }
    return true;
}

bool MainController::GetDAInfo(DA_INFO *da_info)
{
    try
    {
        app_core->GetDAInfo(session_id, da_info);
    }
    catch(const BaseException& exception)
    {
        emit signal_err_msg(FT_GET_DA_INFO_ERROR, exception.err_msg());
        return false;
    }
    return true;
}

bool MainController::IsDALoaded()
{
    try{
        return app_core->IsDALoaded(session_id);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return false;
    }
}

bool MainController::LoadCertFile(const QString &file_name)
{
    try
    {
        app_core->LoadCertFile(session_id, file_name.toLocal8Bit().constData());
    }
    catch(const BaseException& exception)
    {
        emit signal_err_msg(FT_LOAD_CERT_ERROR, exception.err_msg());
        return false;
    }

    return true;
}

bool MainController::IsSecurityFileLoaded()
{
    try{
        return app_core->IsSecuLoaded(session_id);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return false;
    }
}

bool MainController::LoadAuthFile(const QString &file_name)
{
    try
    {
        app_core->LoadAuthFile(session_id, file_name.toLocal8Bit().constData());
    }
    catch(const BaseException& exception)
    {
        emit signal_err_msg(FT_LOAD_AUTH_ERROR, exception.err_msg());
        return false;
    }

    return true;
}

bool MainController::IsAuthFileLoaded()
{
    try{
        return app_core->IsAuthLoaded(session_id);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return false;
    }
}

int MainController::CheckSecUnlockSecro(const int index)
{
    try{
        return app_core->CheckSecUnlock(session_id,index);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return 1;
    }
}

bool MainController::GetScatterVersion(char* version)
{
    try{
        return app_core->GetScatterVersion(session_id, version);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return false;
    }
}

bool MainController::GetScatterHeadInfo(SCATTER_Head_Info *p_info)
{
    try{
        return app_core->GetScatterHeadInfo(session_id, p_info);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return false;
    }
}

bool MainController::IsComboSCIDownload(){
    try{
        return app_core->IsComboSCIDL(session_id);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return false;
    }
}

void MainController::LoadScatterAsync(const QString& file_name, ICallback *callback,
                                      ICallback *failed_callback,ICallback *cancel_callback)
{
    try{
        async_loader->LoadAsyncByScatter(file_name,callback,failed_callback,cancel_callback);

        //TODO: urly code here, change a place
        emit signal_conn_init();
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::LoadSCIScatterAsync(const QString &file_name, ICallback *cb, ICallback *failed_cb)
{
    try{
        async_loader->LoadSCIScatterByAsync(file_name, cb, failed_cb);

        emit signal_conn_init();
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::LoadROMAsync(const QString& file_name, unsigned int index,
                  ICallback *callback, ICallback *failed_callback)
{
    try{
        async_loader->LoadAsyncROMFile(file_name,index,callback,failed_callback);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

unsigned int MainController::QueryROMIndex(int visible_index)
{
    try{
        //get ROM index ...
        std::list<ImageInfo> image_list;
        GetImageInfoList(image_list);
        int item_index = -1;
        int visible_item_count = 0;
        for(std::list<ImageInfo>::const_iterator it = image_list.begin();
            it != image_list.end(); ++it)
        {
            if(it->visible)
            {
                if(visible_item_count == visible_index)
                {
                    item_index = it->index;
                    break;
                }
                visible_item_count++;
            }
        }

        return item_index;
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
        return -1;
    }
}

void MainController::GetImageInfoList(std::list<ImageInfo> &image_info_list, Download_Scene scene)
{
    try{
        app_core->GetROMList(session_id,image_info_list, scene);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::EnableROM(const int index, bool enable)
{
    try{
        app_core->EnableROM(session_id,index,enable);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::EnableROM(const std::string &rom_name, bool enable)
{
    try{
        app_core->EnableROM(session_id, rom_name, enable);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::EnableDAChksum(int chksum)
{
    try{
        app_core->EnableDAChksum(session_id, chksum);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::ClearAllRBItem()
{
    try{
        app_core->ClearAllRBItem(session_id);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::AppendRBItem(const ReadbackItem &item)
{
    try{
        app_core->AppendRBItem(session_id, item);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::UpdateRBItem(const ReadbackItem &item)
{
    try{
        app_core->UpdateRBItem(session_id, item);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::RemoveRBItem(const int index)
{
    try{
        app_core->DeleteRBItem(session_id, index);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::EnableRBItem(const int index, bool enable)
{
    try{
        app_core->EnableRBItem(session_id, index, enable);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void  MainController::GetPlatformInfo(DL_PlatformInfo &info)
{
    try{
        app_core->GetPlatformInfo(session_id, info);
    }catch(const BaseException& e)
    {
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::SetConnSetting(const QSharedPointer<APCore::ConnSetting> &setting)
{
    Q_ASSERT(background_worker != NULL);
    background_worker->SetConnSetting(setting);
}

void MainController::SetChksumSetting(const QSharedPointer<APCore::ChksumSetting> &setting)
{
    Q_ASSERT(async_loader != NULL);
    async_loader->SetChksumSetting(setting);
}

void MainController::SetPlatformSetting()
{
    Q_ASSERT(background_worker != NULL);
    background_worker->SetPlatformSetting(platform_setting);
}

void MainController::QueueAJob(const QSharedPointer<ISetting> &job)
{
    Q_ASSERT(background_worker != NULL);
    background_worker->AddJob(job);
}

void MainController::StartExecuting(ICallback *callback)
{
    Q_ASSERT(background_worker != NULL);
    background_worker->SetCallback(callback);
    background_worker->start();
}

void MainController::StopByUser()
{
    Q_ASSERT(background_worker!= NULL);
    LOG("emit stop signal to background thread.");
    emit signal_stop_working();

    LOG("wait for thread finishing...");
    background_worker->wait();

    main_window->ResetStatus();

    CreateNewThread();
}

bool MainController::isStop()
{
    return background_worker->is_stopping();
}

void MainController::StopByTool()
{

    LOG("emit stop signal to background thread.");
    emit signal_stop_working();

    LOG("wait for thread finishing...");
    background_worker->wait();
}

void MainController::CreateNewThread()
{
    LOG("creating a new working thread.");
    if(background_worker != NULL)
    {
        Q_ASSERT(background_worker->isFinished());

        background_worker->disconnect();
        background_worker->deleteLater();
    }

    background_worker = new BackgroundWorker(session_id,app_core,this,
                            main_window->main_callbacks()->OperationFinished,
                            main_window->main_callbacks()->OperationOK);

    connect(this,SIGNAL(signal_stop_working()),
            background_worker, SLOT(slot_stop_executing()), Qt::DirectConnection);
    connect(background_worker, SIGNAL(signal_err_msg(int, const std::string&)),
            this,SIGNAL(signal_err_msg(int, const std::string&)));
}

void MainController::SetStopLoadFlag(int* stop_flag)
{
    try{
        Q_ASSERT(async_loader != NULL);
        async_loader->SetStopFlag(stop_flag);
    }catch(const BaseException& e){
        emit signal_err_msg(e.err_code(), e.err_msg());
    }
}

void MainController::ToggleLogging(bool enable_log)
{
    if(enable_log == true)
    {
        app_core->EnableLog(AppCore::EnableLogging);
    }
    else
    {
        app_core->EnableLog(AppCore::DisableLogging);
    }
}

void MainController::EnableAdvanceMode()
{
    advance_on_ = !advance_on_;
}

void MainController::EnableMarkBadBlocksMode()
{
    markblocks_on_ = !markblocks_on_;
}

void MainController::get_da_report(const DA_REPORT_T &da_report,
                                   const std::string &friend_name,
                                   const int usb_status)
{
    emit signal_GetDAReport(&da_report, friend_name, usb_status);
}

void MainController::get_brom_result(const BOOT_RESULT &brom_result, const std::string &friend_name)
{
    emit signal_GetBootResult(&brom_result, friend_name);
}

void MainController::set_loop_dl_ok(bool is_ok)
{
    main_window->set_polling_is_ok(is_ok);
}
