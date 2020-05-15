#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QShortcut>
#include <QTimer>
#include "../../Setting/ISetting.h"
#include "../../Setting/ConnSetting.h"
#include "../../ConsoleMode/GeneralSetting.h"
#include "../../Setting/DADownloadAllSetting.h"
#include "../../Setting/FormatSetting.h"
#include "../../Setting/ComboFormatSetting.h"
#include "../../Setting/ReadbackSetting.h"
#include "../../Setting/MemoryTestSetting.h"
#include "../../Setting/DeviceTestSetting.h"
#include "../../Setting/FirmwareUpgradeSetting.h"
#include "../../Setting/DLOnlySetting.h"
#include "../../Setting/SecDLSetting.h"
#include "../../Setting/OTPSetting.h"
#include "../../Setting/ChksumSetting.h"
#include "../../Setting/CertDLSetting.h"
#include "../../Setting/WatchDogSetting.h"
#include "../../Setting/SetBootModeSetting.h"
#include "../../Setting/CheckRPMBSetting.h"
#include "../../Setting/BromAdapterSetting.h"
#include "../../Setting/SCIRBSetting.h"
#include "../../Setting/SCIRestoreSetting.h"
#include "../../Setting/SCIDownloadSetting.h"
#include "../../Setting/DLPlugFmtAllSetting.h"
#include "../../Utility/constString.h"
#include "../../Setting/PlatformSetting.h"
#include "../../Conn/Connection.h"
#include "../../Utility/sendreport.h"
#include "../../Utility/Utils.h"
#include "PlatformObj.h"
#include "OptionDialog.h"
#include "OkDialog.h"


namespace Ui
{
class MainWindow;
}
class AboutDialog;
class OkDialog;
class ProcessingDialog;
class TabWidgetBase;
class MainController;
class MainWindowCallback;
class DownloadWidget;
class MemoryTestWidget;
class DeviceTestWidget;
class ParameterWidget;
class FormatWidget;
class WriteMemoryWidget;
class CheckUpdateDialog;
class UpdateDialog;
class AsyncUpdater;
class ReadBackWidget;
class WelcomeWidget;
class BromAdapterWidget;
class Assistant;
class ScatterObserver;
class ChipInfoWidget;
class NorFlashWidget;
class NandWidget;
class EMMCWidget;
class SDMMCWidget;
class UfsWidget;
class SCIDownloadWidget;
class CloneDownloadWidget;
class EncryDialog;

typedef enum
{
    OPTIONS = 0,
    WINDOW,
    HELP,
    FORMAT,
    DOWNLOAD,
    READBACK,
    MEMORYTEST,
    WRITEMEMORY,
    PARAMETER,
    BAT
}Command_Category;

typedef enum
{
    CMD_CONTENTS,
    CMD_INDEX
}H_COMMAND;

typedef enum
{
    FMT_START,
    FMT_STOP
}FMT_COMMAND;

typedef enum
{
    DL_BEGIN,
    DL_STOP,
    DL_CERT
}DL_COMMAND;

typedef enum
{
    RB_ADD,
    RB_REMVOE,
    RB_START,
    RB_STOP
}RB_COMMAND;

typedef enum
{
    MT_START,
    MT_STOP
}MT_COMMAND;

typedef enum
{
    WM_START,
    WM_STOP
}WME_COMMAND;

typedef enum
{
    PARA_UPDATE,
    PARA_STOP
}PARA_COMMAND;

typedef enum
{
    BROM_DOWNLOAD,
    BORM_JUMP,
    BROM_STOP
}BAT_COMMAND;

#define TO_NATIVE_(path)    QDir::toNativeSeparators(path)

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MainController *main_controller() const{ return main_controller_; }
    ProcessingDialog *processing_dialog() const{ return processing_dialog_; }
    MainWindowCallback *main_callbacks() const{ return main_callbacks_; }

    QSharedPointer<ConsoleMode::GeneralSetting> CreateGeneralSetting();
    QSharedPointer<APCore::ConnSetting> CreateConnSetting();

    QSharedPointer<APCore::DADownloadAllSetting> CreateDownloadSetting();
    QSharedPointer<APCore::FormatSetting> CreateFormatSetting();
    QSharedPointer<APCore::ComboFormatSetting> CreateComboFormatSetting(DL_SCATTER_TYPE type = NORMAL_SCATTER);
    QSharedPointer<APCore::ReadbackSetting> CreateReadbackSetting();
    QSharedPointer<APCore::ReadbackSetting> CreateEmptyReadbackSetting();
    QSharedPointer<APCore::MemoryTestSetting> CreateMemtestSetting();
    QSharedPointer<APCore::DeviceTestSetting> CreateDeviceTestSetting();
    QSharedPointer<APCore::WriteMemorySetting> CreateWriteMemorySetting();
    QSharedPointer<APCore::OTPSetting> CreateOTPSetting();
    QSharedPointer<ISetting> CreateDLSetting();
    QSharedPointer<APCore::SecDLSetting> CreateSecDLSetting();
    QSharedPointer<APCore::DLOnlySetting> CreateDLOnlySetting();
    QSharedPointer<APCore::DLPlugFmtAllSetting> CreateDLPlugFmtAllSetting();
    QSharedPointer<APCore::ComboFormatSetting> CreateDLPlugFmtAllFormatSetting();
    QSharedPointer<APCore::FirmwareUpgradeSetting> CreateFirmwareUpgradeSetting();
    QSharedPointer<APCore::SCIRBSetting> CreateSCIRBSetting();
    QSharedPointer<APCore::SCIRestoreSetting> CreateSCIRestoreSetting();
    QSharedPointer<APCore::FormatSetting> CreateFirmwareUpgradeFormatSetting();
    QSharedPointer<APCore::CertDLSetting> CreateCertDLSetting();
    QSharedPointer<APCore::WatchDogSetting> CreateWatchDogSetting();
    QSharedPointer<APCore::SetBootModeSetting> CreateSetBootModeSetting();
    QSharedPointer<APCore::CheckRPMBSetting> CreateCheckRPMBSetting();
    QSharedPointer<APCore::BromAdapterSetting> CreateBromAdapterSetting(bool only_jump = false);
    QSharedPointer<APCore::SCIDownloadSetting> CreateSCIDownloadSetting();
    QSharedPointer<APCore::ChksumSetting> CreateChksumSetting();

    void LockOnUI(); //called in UI thread
    void DoFinished(); //called in any thread
    void ResetStatus();
    void UpdateWindowTitle(bool enableTrace = false);
    void LoadScatterFile();
    void LoadDAFile();
    void LoadLastAuthFile();
    void RemoveWriteMemoryPage();
    void SetUI();
    void CreateWidget();
    void CreateConnect();
    void SetInitState();

    OkDialog *GetOkDialog(){return ok_dialog;}
    bool ValidateBeforeFormat();
    bool IsScatterFileLoad();
    bool IsDALoad();
    bool IsPreloaderLoad();
    bool IsPhysicalFmtandReadback();
    bool CheckRPMBEnable();
    bool ValidateBeforeMemoryTest();
    bool ValidateBeforeWriteMemory();
    void ShowHelpContents(QWidget *parent, const QString &errMsg, const QString &page);
    void ShowHelpContents(QWidget *parent, int errID, const QString &page);

    void SetLanguageTag(Language_Tag tag){language_tag = tag;}
    Language_Tag GetLanguageTag(){return language_tag;}

    void ChangeLanguage(int index);
    void SetShortCut(Command_Category cmd_category, int cmd, int row);
    void SetHelpShortcut(int cmd, const QString &shortCut);

    void SetPlatfromForBat();

    void UpdateSatusBar();
    void UpdateReadbackList(bool show);
    void UpdatePlatformImageString(const QString str, const QString owner="");

    ScatterObserver* scatter_observer() { return scatter_observer_;}

    void setIsAutoPollingEnable(bool enable){
        this->is_auto_polling_enable_ = enable;
    }

    bool isPlatformValid();

    bool isAutoPollingEnable() const {
        return this->is_auto_polling_enable_;
    }

    void setAutoPollingUpperLimit(unsigned int limit){
        this->auto_polling_upper_limit_ = limit;
    }

    unsigned int autoPollingUpperLimit() const{
       return this->auto_polling_upper_limit_;
    }

    bool isPollingFinish() {
        return finish_;//auto_polling_count_ >= auto_polling_upper_limit_;
    }

    void reset_polling_finish(){
        finish_ = false;
    }

    void set_polling_is_ok(bool is_ok){
        is_ok_ = is_ok;
    }

    void SetSCIDownloadVisible(bool visible){
        is_scidl_visible_ = visible;
    }

    void startDownload();

    void SetUARTBaudrateIndex(unsigned int index);

private:
    Ui::MainWindow *ui;
    MainController *main_controller_;
    MainWindowCallback *main_callbacks_;

    DownloadWidget *download_widget;
    MemoryTestWidget *memtest_widget;
    DeviceTestWidget *devtest_widget;
    ParameterWidget *parameter_widget;
    FormatWidget *format_widget;
    WriteMemoryWidget *writeMemory_widget;
    ReadBackWidget*  readback_widget;
    WelcomeWidget* welcome_widget;
    BromAdapterWidget* bromAdapter_widget;
    SCIDownloadWidget* sciDownload_widget_;
    CloneDownloadWidget* cloneDownload_wdiget_;
    std::list<TabWidgetBase*> tab_widgets;

    ChipInfoWidget* info_widget_;
    NorFlashWidget* nor_widget_;
    NandWidget* nand_widget_;
    EMMCWidget* emmc_widget_;
    SDMMCWidget* sdmmc_widget_;
    UfsWidget* ufs_widget_;

    Language_Tag language_tag;

    AboutDialog *about_dialog;
    ProcessingDialog *processing_dialog_;
    OptionDialog    *option_dialog;
    OkDialog *ok_dialog;
    CheckUpdateDialog *checkupdate_dialog;
    UpdateDialog    *update_dialog;
    EncryDialog     *encry_dialog;
    AsyncUpdater    *async_updater;
    PlatformObj *current_platform;
    ScatterObserver* scatter_observer_;

    QShortcut *mbadblock_shortcut;
    bool mbadblock_on;
    QShortcut *madvance_shortcut;
    bool madvance_on;
    QShortcut *mbromAdapter_shortcut;
    bool mbromAdapter_on;
    bool msecurity_on;
    QShortcut *mSCIDownload_shortcut;
    bool mSCIDownload_on;
    QShortcut *mlogger_shortcut;
    bool mLog_on;
    QShortcut *mDevTest_shortcut;
    bool mDeviceTest_on;

    bool lockui_on;
    bool show_welcome;

    Assistant* assistant_;
    bool is_auto_polling_enable_;
    unsigned int auto_polling_count_;
    unsigned int auto_polling_upper_limit_;
    bool finish_;
    bool is_ok_;
    bool is_scidl_visible_;
    QTimer* thread_timer_;


    //For send error report
    SendReport *send_report_;

    friend class MainWindowCallback;

    void EnableMenus(bool enable);
    void UpdateMenus();
    void InitShortcuts();
    void closeEvent(QCloseEvent *e);
    void LaunchUpdateDialog();
    void ReadSettings();
    void WriteSettings();
    void setChipInfo(ChipInfoWidget* widget, const DA_REPORT_T *p_da_report);
    void setUSBStatus(const int status);
protected:
    void resizeEvent(QResizeEvent *);

signals:
    void signal_UnlockUI();
    void signal_MemoryTestCallback(const QString &msg, QColor color);
    void signal_DeviceTestCallback(const QString &msg, QColor color);
    void signal_platform_change();
    void signal_language_change(int index);
    void signal_start_download();
    void signal_set_UARTBaudrateIndex(unsigned int index);
    void signal_err_msg(int err_code, const std::string &err);

public slots:
    void slot_show_err(int err_code, const std::string &err);
    void slot_check_finish(const bool& hasUpdate);
    void slot_UnlockUI();
    void slot_show_ok();
    void slot_conn_init();
    void slot_enable_DAChksum(int chksum_type);
    void slot_language_changed(int index);
    void slot_platformChanged();
    void slot_setMenuItemStatus();
    void toggleLogging();
    void toggleEnableMarkBadBlockMode();
    void toggleEnableAdvanceMode();
    void toggleBromAdapterWidget();
    void toggleSCIDownloadWidget();
    void toggleDeviceTestWidget();
    void slot_GetDAReport(const DA_REPORT_T *p_da_report, const std::string& friend_name,  const int usb_status );
    void slot_GetBootResult(const BOOT_RESULT *p_boot_result, const std::string& friend_name);
    void slot_UpdateConnStatus(const std::string &friendly_name);
    void stopTimer();

private slots:
    void on_actionAbout_triggered();
    void on_action_SoftwareUpdate_triggered();
    void on_actionUSB_UART_options_triggered();
    void on_actionExit_triggered();
    void on_actionExportFormat_triggered();
    void on_actionExportDownload_triggered();
    void on_actionExportPartial_Format_Download_triggered();
    void on_actionExportWriteMemory_triggered();
    void on_actionPhysical_Format_triggered();
    void on_actionParameter_Page_triggered();
    void on_actionWrite_Memory_triggered();
    void on_actionSecurity_Mode_triggered();
    void on_actionOpen_Logs_Folder_triggered();
    void on_actionReadback_triggered();
    void on_actionIndex_triggered();
    void on_actionContents_triggered();
    void on_actionShow_Welcome_triggered();
    void on_actionCerfitication_download_triggered();
};

#endif // MAINWINDOW_H
