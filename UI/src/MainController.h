#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QtCore/QThread>
#include <list>
#include "../../Public/Host.h"
#include "../../Public/AppTypes.h"
#include "../../Public/AppCore.h"
#include "../../Setting/ISetting.h"
#include "../../Setting/ConnSetting.h"
#include <QSharedPointer>
#include "../../Setting/PlatformSetting.h"
#include "../../Setting/ChksumSetting.h"

class AppCore;
class MainWindow;
class ICallback;
class AsyncResourceLoader;
class BackgroundWorker;

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent, MainWindow * window);
    ~MainController();

    bool LoadDA(const QString& file_name);

    bool GetDAInfo(DA_INFO *da_info);

    bool IsDALoaded();

    bool LoadCertFile(const QString& file_name);

    bool IsSecurityFileLoaded();

    bool LoadAuthFile(const QString& file_name);

    bool IsAuthFileLoaded();

    int  CheckSecUnlockSecro(const int index);

    bool GetScatterVersion(char* version);

    bool GetScatterHeadInfo(SCATTER_Head_Info *p_info);

    bool IsComboSCIDownload();

    void LoadScatterAsync(const QString& file_name, ICallback *callback, ICallback *failed_callback,ICallback *cancel_callback);

    void LoadSCIScatterAsync(const QString& file_name, ICallback *cb, ICallback *failed_cb);

    void LoadROMAsync(const QString& file_name, unsigned int index,
                      ICallback *callback, ICallback *failed_callback);

    unsigned int QueryROMIndex(int visible_index);

    void GetImageInfoList(std::list<ImageInfo> &image_info_list, Download_Scene scene = FORMAT_ALL_DOWNLOAD);

    void EnableROM(const int index, bool enable);

    void EnableROM(const std::string &rom_name, bool enable);

    void EnableDAChksum(int chksum);

    //for readback handles
    void ClearAllRBItem();

    void AppendRBItem(const ReadbackItem &item);

    void UpdateRBItem(const ReadbackItem &item);

    void RemoveRBItem(const int index);

    void EnableRBItem(const int index, bool enable);
    void GetPlatformInfo(DL_PlatformInfo &info);

    void SetConnSetting(const QSharedPointer<APCore::ConnSetting> &setting);

    void SetPlatformSetting();
    void SetChksumSetting(const QSharedPointer<APCore::ChksumSetting> &setting);

    void QueueAJob(const QSharedPointer<ISetting> &job);

    void StartExecuting(ICallback *callback);

    void StopByUser();

    void StopByTool();

    bool isStop();

    void SetStopLoadFlag(int* stop_flag);

    void ToggleLogging(bool enable_log);
    void EnableAdvanceMode();
    void EnableMarkBadBlocksMode();
    void EnablePhysicalFormat();

    bool advance_on() const { return advance_on_;}
    bool markblocks_on() const { return markblocks_on_;}

    QSharedPointer<APCore::PlatformSetting> GetPlatformSetting() const{return platform_setting;}
    void get_da_report(const DA_REPORT_T &da_report, const std::string&, const int);
    void get_brom_result(const BOOT_RESULT &brom_result, const std::string&);
    void set_loop_dl_ok(bool is_ok);

private:
    void CreateNewThread();

    MainWindow *main_window;
    AppCore *app_core;
    APKey session_id;

    bool advance_on_;
    bool markblocks_on_;

    QSharedPointer<APCore::PlatformSetting> platform_setting;
    AsyncResourceLoader *async_loader;
    BackgroundWorker *background_worker;

signals:
    void signal_err_msg(int err_code, const std::string& err_msg);
    void signal_stop_working();
    void signal_conn_init();
    void signal_show_ok();
    void signal_GetDAReport(const DA_REPORT_T *p_da_report, const std::string& friend_name, const int usb_status);
    void signal_GetBootResult(const BOOT_RESULT *p_boot_result, const std::string& friend_name);

public slots:

};

#endif // MAINCONTROLLER_H
