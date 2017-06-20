#ifndef MAINWINDOWCALLBACK_H
#define MAINWINDOWCALLBACK_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QColor>

#include "../../BootRom/flashtool.h"

class MainWindow;

/*****
  *   Callback class for MainWindow
  *   Callback flow:
  *         -> static callback function(pass to FlashToolLib.dll)
  *         -> emit signal(from background thread to main UI thread)
  *         -> slot function(updating UI)
  ***/

class MainWindowCallback : public QObject
{
    Q_OBJECT
public:
    typedef void (__stdcall * void_callback)();
    explicit MainWindowCallback(MainWindow *window, QObject *parent = 0);
    ~MainWindowCallback();

private:
    QTime speed_clock;
    QTime start_clock;
    QTimer *time_label_timer;

    MainWindow *main_window;
    static MainWindowCallback *instance_;
    QString imageName;

    void InstallSignals();

    void UpdateSpeedLabel(U64 bytes);



    //static callback entrances
public:
    static void __stdcall OperationFinished();
    static void __stdcall OperationOK();
    static int __stdcall GetDAReport(const DA_REPORT_T *p_da_report, void *usr_arg);
    static int __stdcall DownloadDAInit(void *usr_arg);
    static int __stdcall DownloadDAProgress(unsigned char percent, unsigned int send_bytes,
                                        unsigned int total_bytes, void *usr_arg);

    static int __stdcall DADLChksumInit(void* usr_arg, const char* image_name);
    static int __stdcall DADLChksumProgress(unsigned char percent, U64 send_bytes,
                                            U64 total_bytes, void *usr_arg);
    static int __stdcall CheckUSBStatusInit(void * arg, const char * usb_status);
    static int __stdcall CERT_Pre_Process_Init(void *usr_arg);
    static int __stdcall CERT_Post_Process_Progress(void* usr_arg);
    static int __stdcall FormatProgressInit(HW_StorageType_E storage_type,
                               U64 begin_addr,
                               U64 length,
                               void *usr_arg);
    static int __stdcall FormatProgressCallback(
            unsigned char finished_percentage, void *usr_arg);

    static int __stdcall DownloadBLoaderInit(void* usr_arg);
    static int __stdcall DownloadBLoaderProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    static int __stdcall DownloadFlashInit(void* usr_arg);
    static int __stdcall DownloadFlashProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    static int __stdcall ReadbackInit(HW_StorageType_E storage,
                           U64 rb_addr,
                           U64 rb_len,
                           const char *file,
                           void* usr_arg);
    static int __stdcall  ReadbackProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    static int __stdcall BromDownload_Progress_Init(void *usr_arg);
    static int __stdcall BromDownload_Progress( unsigned char   finished_percentage,
                                                unsigned int    sent_bytes,
                                                unsigned int    total_bytes,
                                                void          *usr_arg);


    static void MemoryTestUICallback(const QString &msg, QColor color);
    static void DeviceTestUICallback(const QString &msg, QColor color);

    static void __stdcall WriteMemoryInit();
    static int __stdcall WriteMemoryProcess(unsigned char percent,
                                             U64  sent_bytes,
                                             U64  total_bytes,
                                             void*         usr_arg);

    static int __stdcall updateParaInit(void *usr_arg);
    static int __stdcall updatePara(unsigned char percent, void *usr_arg);
    static int __stdcall postProcessInit(void *usr_arg);
    static int __stdcall postProcess(unsigned char percent, void *usr_arg);

    static int __stdcall memChecksumInit(void *usr_arg, const char*image_name);
    static int __stdcall memChecksumPro(unsigned char finished_percentage, U64 finished_bytes,
                                                      U64 total_bytes, void *usr_arg);

signals:
    void signal_OperationFinished();
    void signal_OperationOK();
    void signal_GetDAReport(const DA_REPORT_T *p_da_report,
                            const std::string& friend_name,
                            const std::string& usb_status);
    void signal_DownloadDAInit(void *usr_arg);
    void signal_DownloadDAProgress(unsigned char percent, unsigned int send_bytes,
                                   unsigned int total_bytes, void *usr_arg);
    void signal_CheckUSBStatusInit(void *arg, const char* usb_status);
    void signal_FormatProgressInit(HW_StorageType_E storage_type,
                                   U64 begin_addr,
                                   U64 length,
                                   void *usr_arg);
    void signal_FormatProgressCallback(unsigned char finished_percentage,
                                       void *usr_arg);

    void signal_DownloadBLoaderInit(void* usr_arg);
    void signal_DownloadBLoaderProgress(
        unsigned char finished_percentage,
        unsigned int finished_bytes,
        unsigned int total_bytes,
        void *usr_arg);
    void signal_DADLChksumInit(void* usr_arg, const char* image_name);
    void signal_DADLChksumProgress(unsigned char percent, U64 send_bytes,
                                            U64 total_bytes, void *usr_arg);
    void signal_CERTProcessInit(void* usr_arg);
    void signal_CERTProcessProgress(void* usr_arg);

    void signal_DownloadFlashInit(void* usr_arg);
    void signal_DownloadFlashProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    void signal_ReadbackInit(HW_StorageType_E storage,
                           U64 rb_addr,
                           U64 rb_len,
                           const char *file,
                           void* usr_arg);
    void signal_ReadbackProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    void signal_BromDownloadInit(void *usr_arg);
    void signal_BromDownloadProgress(
        unsigned char finished_percentage,
        U64 sent_bytes,
        U64 total_bytes,
        void *usr_arg);

    void signal_MemoryTestCallback(const QString &msg, QColor color);
    void signal_DeviceTestCallback(const QString &msg, QColor color);

    void signal_WriteMemoryInit();
    void signal_WriteMemoryProgress(unsigned char percent,
                                    U64  sent_bytes,
                                    U64  total_bytes,
                                    void*         usr_arg);

    void signal_updateParaInit(void *usr_arg);
    void signal_updatePara(unsigned char percent, void *usr_arg);
    void signal_postProcessInit(void *usr_arg);
    void signal_postProcess(unsigned char percent, void *usr_arg);
    void signal_memChecksumInit(void *usr_arg, const QString image_name);
    void signal_memChecksumPro(unsigned char finished_percentage, U64 finished_bytes,
                                 U64 total_bytes, void *usr_arg);

public slots:
    void slot_UpdateTimeLabel();

    void slot_OperationFinished();
    void slot_OperationOK();
    void slot_GetDAReport(const DA_REPORT_T *p_da_report, const std::string& friend_name, const::std::string& usb_status);
    void slot_DownloadDAInit(void *usr_arg);
    void slot_DownloadDAProgress(unsigned char percent, unsigned int send_bytes,
                                 unsigned int total_bytes, void *usr_arg);
    void slot_CheckUSBStatusInit(void *arg, const char* /*usb_status*/);
    void slot_FormatProgressInit(HW_StorageType_E storage_type,
                                   U64 begin_addr,
                                   U64 length,
                                   void *usr_arg);
    void slot_FormatProgressCallback(unsigned char finished_percentage,
                                void *usr_arg);

    void slot_DownloadBLoaderInit(void* usr_arg);
    void slot_DownloadBLoaderProgress(
        unsigned char finished_percentage,
        unsigned int finished_bytes,
        unsigned int total_bytes,
        void *usr_arg);

    void slot_DownloadFlashInit(void* usr_arg);
    void slot_DownloadFlashProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    void slot_DADLChksum_Init(void* usr_arg, const char* );
    void slot_DADLChksum_Progress(unsigned char percent, U64 send_bytes,
                                            U64 total_bytes, void *usr_arg);

    void slot_CERTProcess_Init(void* usr_arg);
    void slot_CERTProcess_Progress(void* );

    void slot_ReadbackInit(HW_StorageType_E storage,
                           U64 rb_addr,
                           U64 rb_len,
                           const char *file,
                           void *usr_arg);
    void slot_ReadbackProgress(
        unsigned char finished_percentage,
        U64 finished_bytes,
        U64 total_bytes,
        void *usr_arg);

    void slot_BromDownloadInit(void *usr_arg);
    void slot_BromDownloadProgress(
        unsigned char finished_percentage,
        U64 sent_bytes,
        U64 total_bytes,
        void *usr_arg);

    void slot_WriteMemoryInit(void);
    void slot_WriteMemoryProgress(unsigned char percent,
                                  U64  sent_bytes,
                                  U64  total_bytes,
                                  void *usr_arg);

    void slot_updateParaInit(void *usr_arg);
    void slot_updatePara(unsigned char percent, void *usr_arg);
    void slot_postProcessInit(void *usr_arg);
    void slot_postProcess(unsigned char percent, void *usr_arg);
    void slot_MemChksumIni(void *usr_arg, const QString image_name);
    void slot_MemChksum(unsigned char finished_percentage, U64 finished_bytes,
                             U64 total_bytes, void *usr_arg);
};

#endif // MAINWINDOWCALLBACK_H
