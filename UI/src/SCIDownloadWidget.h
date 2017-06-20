#ifndef SCIDOWNLOADWIDGET_H
#define SCIDOWNLOADWIDGET_H

#include <QWidget>
#include <QtGui>
#include "TabWidgetBase.h"
#include "DLDelegate.h"
#include "../../Public/AppTypes.h"
#include "../../Setting/PlatformSetting.h"

namespace Ui {
class SCIDownloadWidget;
}

class MainWindow;
class CheckHeader;

class SCIDownloadWidget : public TabWidgetBase, public APCore::IPlatformOb
{
    Q_OBJECT
    
public:
    explicit SCIDownloadWidget(QTabWidget* parent, MainWindow *window);
    ~SCIDownloadWidget();

    DECLARE_TABWIDGET_VFUNCS()

    virtual void onPlatformChanged();

    void OnLoadSCIScatterEnd();
    void OnLoadSCIScatterFailed();

    void setDelegate(DLDelegate *delegate){
        delegate_ = delegate;
    }

    bool IsComboSCI()
    {
        return iscombo_sci_;
    }

private:
    void CreateSignalConnection();
    void LoadSCIScatterFile(const QString &file_path);
    void UpdateImageList(bool checked);
    bool ValidBeforeDownload();

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);

signals:
    void signal_sci_load_finished();
    void signal_sci_load_failed();

    
private slots:
   void on_pushButton_SCIscatterLoading_clicked();
   void slot_OnSCILoadEnd();
   void slot_OnSCILoadFailed();

   void slot_OnHeaderView_click(int index);

   void on_pushButton_SCIDownload_clicked();

   void on_pushButton_Stop_clicked();

private:
    Ui::SCIDownloadWidget *ui;
    MainWindow* main_window_;
    CheckHeader *header_;
    HW_StorageType_E storage_;
    DLDelegate *delegate_;
    bool iscombo_sci_;
};

#endif // SCIDOWNLOADWIDGET_H
