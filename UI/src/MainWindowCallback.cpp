#include "MainWindowCallback.h"
#include "MainWindow.h"
#include "MainController.h"
#include "ui_MainWindow.h"
#include "../../Utility/Utils.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include <QtDebug>
#include <QTime>

static const QString PColorRed (" QProgressBar::chunk{background-color: red} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorGreen (" QProgressBar::chunk{background-color: green} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorBlue (" QProgressBar::chunk{background-color: blue} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorYellow (" QProgressBar::chunk{background-color: yellow} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorDarkRed (" QProgressBar::chunk{background-color: darkRed} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorDarkMagenta (" QProgressBar::chunk{background-color: darkMagenta} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorDarkCyan (" QProgressBar::chunk{background-color: darkCyan} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorDarkBlue (" QProgressBar::chunk{background-color: darkBlue} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorMagenta (" QProgressBar::chunk{background-color: magenta} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorDarkGray (" QProgressBar::chunk{background-color: darkGray} QProgressBar {text-align: center; border-radius: 0px}");
static const QString PColorOrange("QProgressBar::chunk{background-color:rgba(255,170,127,255} QProgressBar {text-align:center; border-radius:0px}");
static const QString PColorBrown("QProgressBar::chunk{background-color:rgba(85,0,0,255} QProgressBar {text-align:center; border-radius:0px}");

MainWindowCallback *MainWindowCallback::instance_ = NULL;

MainWindowCallback::MainWindowCallback(MainWindow *window, QObject *parent) :
    QObject(parent),
    main_window(window)

{
    instance_ = this;

    time_label_timer = new QTimer(this);
    connect(time_label_timer, SIGNAL(timeout()), this, SLOT(slot_UpdateTimeLabel()));

    InstallSignals();
}

MainWindowCallback::~MainWindowCallback()
{
    if(time_label_timer != NULL)
    {
        delete time_label_timer;
    }
}

void MainWindowCallback::InstallSignals()
{
    qRegisterMetaType<HW_StorageType_E>("HW_StorageType_E");
    qRegisterMetaType<U64>("U64");

    connect(this, SIGNAL(signal_OperationFinished()),SLOT(slot_OperationFinished()));
    connect(this, SIGNAL(signal_OperationOK()), SLOT(slot_OperationOK()));
#if 0 //2013/08/21: Remove signals and slots to align with observers notification
    connect(this, SIGNAL(signal_GetDAReport(const DA_REPORT_T*,const std::string& friend_name)),
            SLOT(slot_GetDAReport(const DA_REPORT_T*,const std::string& friend_name)));
#endif
    connect(this,SIGNAL(signal_DownloadDAInit(void*)),SLOT(slot_DownloadDAInit(void*)));
    connect(this,SIGNAL(signal_DownloadDAProgress(unsigned char,uint,uint,void*)),
            SLOT(slot_DownloadDAProgress(unsigned char,uint,uint,void*)));
    connect(this,SIGNAL(signal_FormatProgressInit(HW_StorageType_E,U64,U64,void*)),
            SLOT(slot_FormatProgressInit(HW_StorageType_E,U64,U64,void*)));
    connect(this,SIGNAL(signal_FormatProgressCallback(unsigned char,void*)),
            SLOT(slot_FormatProgressCallback(unsigned char,void*)));
    connect(this,SIGNAL(signal_DownloadBLoaderInit(void*)),
            SLOT(slot_DownloadBLoaderInit(void*)));
    connect(this,SIGNAL(signal_DownloadBLoaderProgress(unsigned char,uint,uint,void*)),
            SLOT(slot_DownloadBLoaderProgress(unsigned char,uint,uint,void*)));
    connect(this,SIGNAL(signal_DownloadFlashInit(void*)),
            SLOT(slot_DownloadFlashInit(void*)));
    connect(this,SIGNAL(signal_DownloadFlashProgress(unsigned char,U64,U64,void*)),
            SLOT(slot_DownloadFlashProgress(unsigned char,U64,U64,void*)));
    connect(this, SIGNAL(signal_DADLChksumInit(void*, const char*)),
            SLOT(slot_DADLChksum_Init(void*, const char*)));
    connect(this, SIGNAL(signal_DADLChksumProgress(unsigned char,U64,U64,void*)),
            SLOT(slot_DADLChksum_Progress(unsigned char,U64,U64,void*)));
    connect(this, SIGNAL(signal_CERTProcessInit(void*)), SLOT(slot_CERTProcess_Init(void*)));
    connect(this, SIGNAL(signal_CERTProcessProgress(void*)), SLOT(slot_CERTProcess_Progress(void*)));
    connect(this,SIGNAL(signal_ReadbackInit(HW_StorageType_E,U64,U64,const char*,void*)),
            SLOT(slot_ReadbackInit(HW_StorageType_E,U64,U64,const char*,void*)));
    connect(this,SIGNAL(signal_ReadbackProgress(unsigned char,U64,U64,void*)),
            SLOT(slot_ReadbackProgress(unsigned char,U64,U64,void*)));

    connect(this, SIGNAL(signal_BromDownloadInit(void*)), SLOT(slot_BromDownloadInit(void*)));
    connect(this, SIGNAL(signal_BromDownloadProgress(unsigned char,U64,U64,void*)),
            SLOT(slot_BromDownloadProgress(unsigned char,U64,U64,void*)));

    connect(this,SIGNAL(signal_MemoryTestCallback(const QString&,QColor)),
            main_window, SIGNAL(signal_MemoryTestCallback(const QString&,QColor)));

    connect(this,SIGNAL(signal_DeviceTestCallback(const QString&,QColor)),
            main_window, SIGNAL(signal_DeviceTestCallback(const QString&,QColor)));

    connect(this, SIGNAL(signal_WriteMemoryInit()),
            SLOT(slot_WriteMemoryInit()));
    connect(this, SIGNAL(signal_WriteMemoryProgress(unsigned char,U64,U64,void*)),
            SLOT(slot_WriteMemoryProgress(unsigned char,U64,U64,void*)));
    connect(this, SIGNAL(signal_memChecksumInit(void*,const QString)),
            SLOT(slot_MemChksumIni(void*,const QString)));
    connect(this, SIGNAL(signal_memChecksumPro(unsigned char,U64,U64,void*)),
            SLOT(slot_MemChksum(unsigned char,U64,U64,void*)));
    connect(this, SIGNAL(signal_CheckUSBStatusInit(void *, const char* )),
            SLOT(slot_CheckUSBStatusInit(void *, const char*)));

}

void MainWindowCallback::slot_UpdateTimeLabel()
{
    int seconds = start_clock.elapsed()/1000;
    QString zero;
    zero = (10 > (seconds % 60)) ? "0" : "";

    main_window->ui->label_time->setText(QString::number(seconds / 60) + ":" + zero + QString::number(seconds % 60));
}

void MainWindowCallback::UpdateSpeedLabel(U64 bytes)
{
    std::string speedInfo;
    std::string totalByte;

    unsigned long timeUseMsec = speed_clock.elapsed();

    if(timeUseMsec > 0)
    {
        speedInfo = Utils::PrettyByteString(bytes*1000/timeUseMsec);
        speedInfo += "/s";
    }

    totalByte = Utils::PrettyByteString(bytes);

    main_window->ui->label_SpeedInfo->setText(speedInfo.c_str());
    main_window->ui->label_total->setText(totalByte.c_str());
}

void __stdcall MainWindowCallback::OperationFinished()
{
    emit instance_->signal_OperationFinished();
}

void __stdcall MainWindowCallback::OperationOK()
{
    emit instance_->signal_OperationOK();
}

int __stdcall MainWindowCallback::GetDAReport(
    const DA_REPORT_T *p_da_report, void *usr_arg)
{
    Q_UNUSED(usr_arg);

    emit instance_->signal_GetDAReport(p_da_report, "", "");

    return 0;
}

int __stdcall MainWindowCallback::DownloadDAInit(void *usr_arg)
{
    emit instance_->signal_DownloadDAInit(usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::DownloadDAProgress(unsigned char percent, unsigned int send_bytes,
                                    unsigned int total_bytes, void *usr_arg)
{
    emit instance_->signal_DownloadDAProgress(percent,send_bytes,total_bytes,usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::FormatProgressInit(HW_StorageType_E storage_type,
                                                      U64 begin_addr,
                                                      U64 length,
                                                      void *usr_arg)
{
    emit instance_->signal_FormatProgressInit(storage_type,begin_addr,length,usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::FormatProgressCallback(
        unsigned char finished_percentage, void *usr_arg)
{
    emit instance_->signal_FormatProgressCallback(finished_percentage, usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::DownloadBLoaderInit(void* usr_arg)
{
    emit instance_->signal_DownloadBLoaderInit(usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::DownloadBLoaderProgress(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void *usr_arg)
{
    emit instance_->signal_DownloadBLoaderProgress(finished_percentage,
                                                   finished_bytes,total_bytes,usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::DownloadFlashInit(void* usr_arg)
{
    emit instance_->signal_DownloadFlashInit(usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::DownloadFlashProgress(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void *usr_arg)
{
    emit instance_->signal_DownloadFlashProgress(finished_percentage,
                                                 finished_bytes,total_bytes,usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::DADLChksumInit(void* usr_arg, const char* image_name)
{
    emit instance_->signal_DADLChksumInit(usr_arg, image_name);
    return 0;
}

int __stdcall MainWindowCallback::DADLChksumProgress(unsigned char percent, U64 sent_bytes,
                                                      U64 total_bytes, void *usr_arg)
{
    emit instance_->signal_DADLChksumProgress(percent, sent_bytes, total_bytes, usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::CheckUSBStatusInit(void * arg, const char* usb_status)
{
   emit instance_->signal_CheckUSBStatusInit(arg, usb_status);
   return 0;
}

int __stdcall MainWindowCallback::CERT_Pre_Process_Init(void *usr_arg)
{
    emit instance_->signal_CERTProcessInit(usr_arg);

    return 0;
}

int __stdcall MainWindowCallback::CERT_Post_Process_Progress(void *usr_arg)
{
    emit instance_->signal_CERTProcessProgress(usr_arg);

    return 0;
}

int __stdcall MainWindowCallback::ReadbackInit(HW_StorageType_E storage,
                       U64 rb_addr,
                       U64 rb_len,
                       const char *file,
                       void* usr_arg)
{
    emit instance_->signal_ReadbackInit(storage, rb_addr,rb_len,file,usr_arg);
    return 0;
}

int __stdcall  MainWindowCallback::ReadbackProgress(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void *usr_arg)
{
    emit instance_->signal_ReadbackProgress(finished_percentage, finished_bytes,
                                            total_bytes, usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::BromDownload_Progress_Init(
    void *usr_arg)
{
    emit instance_->signal_BromDownloadInit(usr_arg);

    return 0;
}

int __stdcall MainWindowCallback::BromDownload_Progress(unsigned char finished_percentage,
                                                        unsigned int sent_bytes,
                                                        unsigned int total_bytes,
                                                        void *usr_arg)
{
    emit instance_->signal_BromDownloadProgress(finished_percentage,
                                                sent_bytes,
                                                total_bytes, usr_arg);

    return 0;
}

void MainWindowCallback::MemoryTestUICallback(const QString &msg, QColor color)
{
    emit instance_->signal_MemoryTestCallback(msg, color);
}

void MainWindowCallback::DeviceTestUICallback(const QString &msg, QColor color)
{
    emit instance_->signal_DeviceTestCallback(msg, color);
}

void __stdcall MainWindowCallback::WriteMemoryInit()
{
    emit instance_->signal_WriteMemoryInit();
}

int __stdcall MainWindowCallback::WriteMemoryProcess(unsigned char percent,
                                                     U64 sent_bytes,
                                                     U64 total_bytes,
                                                     void*         usr_arg)
{
    emit instance_->signal_WriteMemoryProgress(percent, sent_bytes, total_bytes, usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::updateParaInit(void *usr_arg)
{
    emit instance_->signal_updateParaInit(usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::updatePara(unsigned char percent, void *usr_arg)
{
    emit instance_->signal_updatePara(percent, usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::postProcessInit(void *usr_arg)
{
    emit instance_->signal_postProcessInit(usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::postProcess(unsigned char percent, void *usr_arg)
{
    emit instance_->signal_postProcess(percent, usr_arg);
    return 0;
}

int __stdcall MainWindowCallback::memChecksumInit(void *usr_arg, const char*image_name)
{
    emit instance_->signal_memChecksumInit(usr_arg, QString(image_name));
    return 0;
}

int __stdcall MainWindowCallback::memChecksumPro(unsigned char finished_percentage, U64 finished_bytes,
                                                  U64 total_bytes, void *usr_arg)
{
    emit instance_->signal_memChecksumPro(finished_percentage, finished_bytes, total_bytes, usr_arg);
    return 0;
}

//slot functions to update UI in main thread.
void MainWindowCallback::slot_OperationFinished()
{
    time_label_timer->stop();
}

void MainWindowCallback::slot_OperationOK()
{
    bool isFinished = false;

    if(!main_window->isAutoPollingEnable() ||
            (main_window->isAutoPollingEnable() && main_window->isPollingFinish()))
    {
        main_window->GetOkDialog()->setModal(true);
        main_window->GetOkDialog()->exec();

        main_window->ResetStatus();

        isFinished = true;

        main_window->reset_polling_finish();
    }

    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr(""));

    if(!isFinished)
    {
        QTime time;
        time.start();
        LOGD("wait 1s start!");
        while(time.elapsed()<1000)
        {
           QCoreApplication::processEvents();
        }
        LOGD("wait 1s end!");
        main_window->startDownload();
}
}

void MainWindowCallback::slot_GetDAReport(const DA_REPORT_T *p_da_report,
                                          const std::string& friend_name,
                                          const std::string& usb_status)
{
    Q_UNUSED(friend_name);
    Q_UNUSED(usb_status);

    char device_hint[512];
    char detail_report[2048];

    ConvertDAReportToString(p_da_report, device_hint, sizeof(device_hint), detail_report, sizeof(detail_report));
}

void MainWindowCallback::slot_DownloadDAInit(void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setStyleSheet(PColorRed);
    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr("Download DA %1").arg(0));
    speed_clock.restart();

    start_clock.restart();
    time_label_timer->start(1000);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_DownloadDAProgress(unsigned char percent, unsigned int send_bytes,
                             unsigned int total_bytes, void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setValue(percent);
    main_window->ui->progressBar->setFormat(tr("Download DA %1%").arg(percent));
    this->UpdateSpeedLabel(send_bytes);
    total_bytes = 0;
    usr_arg = 0;
    Q_UNUSED(total_bytes);
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_CheckUSBStatusInit(void *arg, const char* /* usb_status*/)
{
    //main_window->ui->label_status_item2->setText(QString(usb_status));
    arg = 0;
    Q_UNUSED(arg);
}

void MainWindowCallback::slot_DADLChksum_Init(void* usr_arg, const char* /*image_name*/)
{
    if(!main_window->main_controller()->isStop())
    {
        LOGD("chksum initial...");

        main_window->ui->progressBar->setStyleSheet(PColorDarkGray);
        main_window->ui->progressBar->setValue(0);
        main_window->ui->progressBar->setFormat(tr("DA DL CheckSum %1%").arg(0));
        usr_arg = 0;
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_DADLChksum_Progress(unsigned char percent, U64  sent_bytes,
                                                  U64 total_bytes, void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
        main_window->ui->progressBar->setValue(percent);
        main_window->ui->progressBar->setFormat(tr("DA DL CheckSum %1%").arg(percent));
        this->UpdateSpeedLabel(sent_bytes);
        total_bytes = 0;
        usr_arg = 0;
        Q_UNUSED(total_bytes);
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_CERTProcess_Init(void *usr_arg)
{
    main_window->ui->progressBar->setStyleSheet(PColorOrange);
    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr("CERT DL %1").arg(0));
    usr_arg = 0;
    Q_UNUSED(usr_arg);
}

void MainWindowCallback::slot_CERTProcess_Progress(void * /*usr_arg*/)
{

}

void MainWindowCallback::slot_FormatProgressInit(HW_StorageType_E storage_type,
                               U64 begin_addr,
                               U64 length,
                               void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setStyleSheet(PColorGreen);
    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr("Format %1%").arg(0));
    LOG("The storage_type is %d, The begin_addr is %016llx, The length is %016llx.",storage_type, begin_addr, length);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_FormatProgressCallback(unsigned char finished_percentage,
                            void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setValue(finished_percentage);
    main_window->ui->progressBar->setFormat(tr("Format %1%").arg(finished_percentage));
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_DownloadBLoaderInit(void* usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
        main_window->ui->progressBar->setStyleSheet(PColorDarkMagenta);
        main_window->ui->progressBar->setValue(0);
        main_window->ui->progressBar->setFormat(tr("Download Boot Loader %1%").arg(0));
        speed_clock.restart();
        usr_arg = 0;
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_DownloadBLoaderProgress(
    unsigned char finished_percentage,
    unsigned int finished_bytes,
    unsigned int total_bytes,
    void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
        main_window->ui->progressBar->setValue(finished_percentage);
        main_window->ui->progressBar->setFormat(tr("Download Boot Loader %1%").arg(finished_percentage));
        this->UpdateSpeedLabel(finished_bytes);
        LOG("The total_bytes is %d", total_bytes);
        usr_arg = 0;
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_BromDownloadInit(void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
        main_window->ui->progressBar->setStyleSheet(PColorBrown);
        main_window->ui->progressBar->setValue(0);
        main_window->ui->progressBar->setFormat(tr("Download Brom load %1%").arg(0));
        usr_arg = 0;
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_BromDownloadProgress(unsigned char finished_percentage,
                                                   U64 sent_bytes,
                                                   U64 total_bytes, void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setValue(finished_percentage);
    main_window->ui->progressBar->setFormat(tr("Download Brom load %1%").arg(finished_percentage));
    sent_bytes = 0;
    total_bytes = 0;
    usr_arg = 0;
    Q_UNUSED(sent_bytes);
    Q_UNUSED(total_bytes);
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_DownloadFlashInit(void* usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setStyleSheet(PColorYellow);
    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr("Download Flash %1%").arg(0));
    speed_clock.restart();
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_DownloadFlashProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
        main_window->ui->progressBar->setValue(finished_percentage);
        main_window->ui->progressBar->setFormat(tr("Download Flash %1%").arg(finished_percentage));
        this->UpdateSpeedLabel(finished_bytes);
        total_bytes = 0;
        usr_arg = 0;
        Q_UNUSED(total_bytes);
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_ReadbackInit(HW_StorageType_E storage,
                       U64 rb_addr,
                       U64 rb_len,
                       const char *file,
                       void* usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setStyleSheet(PColorBlue);
    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr("Read back %1%").arg(0));
    speed_clock.restart();

    QString tip;
    switch(storage)
    {
    case HW_STORAGE_NAND:
        tip.append("storage:NAND,");
        break;
    case HW_STORAGE_EMMC:
        tip.append("storage:EMMC,");
        break;
    case HW_STORAGE_SDMMC:
        tip.append("storage:SDMMC,");
        break;
    default:
        tip.append("storage:UNKNOWN,");
        break;
    }
    tip.append(QString().sprintf("addr:0x%llx, len:0x%llx, file:%s",
                                 rb_addr, rb_len, file));

    main_window->ui->progressBar->setToolTip(tip);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_ReadbackProgress(
    unsigned char finished_percentage,
    U64 finished_bytes,
    U64 total_bytes,
    void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
        main_window->ui->progressBar->setValue(finished_percentage);
        main_window->ui->progressBar->setFormat(tr("Read back %1%").arg(finished_percentage));
        this->UpdateSpeedLabel(finished_bytes);
        total_bytes = 0;
        usr_arg = 0;
        Q_UNUSED(total_bytes);
        Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_WriteMemoryInit()
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setStyleSheet(PColorDarkCyan);
    main_window->ui->progressBar->setFormat(tr("Write Memory %1%").arg(0));
    main_window->ui->progressBar->setValue(0);
    speed_clock.restart();
    }
}

void MainWindowCallback::slot_WriteMemoryProgress(unsigned char percent,
                                                  U64  sent_bytes,
                                                  U64  total_bytes,
                                                  void*         usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setValue(percent);
    main_window->ui->progressBar->setFormat(tr("Write Memory %1%").arg(percent));
    this->UpdateSpeedLabel(sent_bytes);
    total_bytes = 0;
    usr_arg = 0;
    Q_UNUSED(total_bytes);
    Q_UNUSED(usr_arg);
    }
}

void MainWindowCallback::slot_updateParaInit(void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setStyleSheet(PColorDarkBlue);

    main_window->ui->progressBar->setValue(0);
    main_window->ui->progressBar->setFormat(tr("Update Parameter %1%").arg(0));
    speed_clock.restart();
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}
void MainWindowCallback::slot_updatePara(unsigned char percent, void *usr_arg)
{
    if(!main_window->main_controller()->isStop())
    {
    main_window->ui->progressBar->setValue(percent);
    main_window->ui->progressBar->setFormat(tr("Update Parameter %1%").arg(percent));
    this->UpdateSpeedLabel(percent);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    }
}
void MainWindowCallback::slot_postProcessInit(void *usr_arg)
{
    main_window->ui->progressBar->setStyleSheet(PColorDarkRed);
    main_window->ui->progressBar->setValue(0);
    speed_clock.restart();
    usr_arg = 0;
    Q_UNUSED(usr_arg);
}
void MainWindowCallback::slot_postProcess(unsigned char percent, void *usr_arg)
{
    main_window->ui->progressBar->setValue(percent);
    this->UpdateSpeedLabel(percent);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
}
void MainWindowCallback::slot_MemChksumIni(void *usr_arg, const QString image_name)
{
   main_window->ui->progressBar->setStyleSheet(PColorMagenta);
   main_window->ui->progressBar->setValue(0);
   imageName = image_name;

   usr_arg = 0;
   Q_UNUSED(usr_arg);
}

void MainWindowCallback::slot_MemChksum(unsigned char finished_percentage, U64 finished_bytes,
                                          U64 total_bytes, void *usr_arg)
{
   main_window->ui->progressBar->setValue(finished_percentage);
   main_window->ui->progressBar->setFormat(tr("CheckSum %1 %2%").arg(imageName).arg(finished_percentage));
   this->UpdateSpeedLabel(finished_bytes);
   total_bytes = 0;
   usr_arg = 0;
   Q_UNUSED(total_bytes);
   Q_UNUSED(usr_arg);
}
