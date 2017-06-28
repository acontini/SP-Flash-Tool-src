#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include "TabWidgetBase.h"
#include "../../Public/AppTypes.h"
#include "../../ConsoleMode/GeneralSetting.h"
#include "../../Setting/DADownloadAllSetting.h"
#include "../../Setting/PlatformSetting.h"
#include <QSharedPointer>
#include <list>
#include <QtGui>
#include <QStringList>
#include "MainController.h"
#include "ScatterObserver.h"

namespace Ui
{
class DownloadWidget;
}

class MainWindow;
class CheckHeader;
class PlatformSetting;

class DownloadWidget : public TabWidgetBase, public APCore::IPlatformOb, public IScatterObj
{
    Q_OBJECT
public:
    enum ImageColumn{
        ColumnEnable = 0,
        ColumnName,
        ColumnBeginAddr,
        ColumnEndAddr,
        columnRegion,
        ColumnLocation
    };

    explicit DownloadWidget(QTabWidget *parent, MainWindow *window);
    ~DownloadWidget();

    Download_Scene scene () const
   {
          return this->scene_;
   }

    DECLARE_TABWIDGET_VFUNCS()

    virtual void onPlatformChanged();
    virtual void OnScatterChanged(bool showRegion);

    void OnLoadByScatterEnd();
    void OnLoadByScatterFailed();
    void OnLoadByScatterCanceled();
    void OnLoadRomDone();
    void OnLoadRomFailed();
    bool ValidateBeforeFormat();
    bool IsScatterFileLoad();
    void LoadLastScatterFile();
    void ShowUnavailableItems(bool show = false);
    bool IsScatterLoad();
    bool InitialPlatform(void);

    void BuildGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting> &gen_setting);
    void BuildDownloadSetting(QSharedPointer<APCore::DADownloadAllSetting> &dl_setting);
    void LoadDefaultDA();
    void LoadAuthFile(const QString& file_name);
    void LoadLastAuthFile();

    unsigned char GetBootMode_ComType(void);
    unsigned char  GetBootMode_ComId(void);
    unsigned char GetBootMode(void);
    void EnableBootMode_ComType(bool enable);
    void EnableBootMode_ComId(bool enable);
    void Enable_groupBox_boot_mode_flag(bool enable);

private:
    void UpdateImageList(std::list<ImageInfo> &image_list);
    void UpdateImageList(bool checked, Download_Scene scene_);
    void UpdateImageList();
    void SetRomAddress(int row, int column, U64 address);

    void LoadScatterFile(const QString &file_name);

    int  FindIndex(const QString &filePath);

    bool ValidateBeforeDownload();
    bool ValidateBeforeCertDL();

    bool hasUncheckedRomInfo();
    void UpdateScatterFile(const QString& fileName);
    void UpdateScene();
    void UpdateCustomSceneSelectItems();
    void UpdateHeadviewCheckState();
    void SetDACheckSum();

    void choose_rom_file(int row, int column);
    void UpdateRomInfoList(Download_Scene scene_);

    int GetSecRoIndex();
    void StartDownload();

    void UpdateUIStatus();

    MainWindow *main_window_;
    Ui::DownloadWidget *ui_;

    QStringList scatterFile_historyList_;
    QStringList authFile_historyList_;
    Download_Scene scene_;
    CheckHeader *header_;

    HW_StorageType_E storage_;
    bool bUpdateScene;
    QString default_da;
    std::list<IScatterObj *> observers;
    int mStopFlag;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void signal_load_finished();
    void signal_load_failed();
    void signal_load_canceled();
    void signal_load_rom_done();
    void signal_load_rom_failed();

public slots:
    void slot_OnLoadByScatterEnd();
    void slot_OnUserCancelLoadScatter();
    void slot_OnLoadByScatterCanceled();
    void slot_OnLoadByScatterFailed();

    void slot_OnLoadRomDone();
    void slot_OnLoadRomFailed();

private slots:
    void on_pushButton_downloadAgent_clicked();
    void on_pushButton_scatterLoading_clicked();

    //enable/disable a ROM
    void on_tableWidget_cellClicked(int row, int column);
    void on_pushButton_download_clicked();
    void on_pushButton_stop_clicked();

    void slot_OnHeaderView_click(int index);
    void slot_start_download();

    void on_comboBox_scatterFilePath_activated(const QString &arg1);
    void on_pushButton_CertFile_clicked();
    void on_pushButton_authFile_clicked();
    void on_comboBox_Scene_activated(int index);
    void on_toolButton_Certification_clicked();
    void on_tableWidget_itemChanged(QTableWidgetItem *item);
    void on_checkbox_set_boot_mode_to_meta_clicked();
    void on_comboBox_authFilePath_currentIndexChanged(int index);
};

#endif // DOWNLOADWIDGET_H
