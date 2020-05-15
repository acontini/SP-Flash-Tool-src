#ifndef CLONEDOWNLOADWIDGET_H
#define CLONEDOWNLOADWIDGET_H

#include <QWidget>
#include <QtGui>
#include "TabWidgetBase.h"

namespace Ui {
class CloneDownloadWidget;
}

class MainWindow;

class CloneDownloadWidget : public TabWidgetBase
{
    Q_OBJECT
    
public:
    explicit CloneDownloadWidget(QTabWidget *parent, MainWindow* window);
    ~CloneDownloadWidget();

    DECLARE_TABWIDGET_VFUNCS()

    const std::string &GetSCIReadbackFolderPath(){
        return readback_folder_path_;
    }

    const std::string &GetSCIRestoreFolderPath(){
        return restore_folder_path_;
    }

private:
    void enableControl(bool enable);
    
private slots:
    void on_pushButton_readback_clicked();

    void on_pushButton_restore_clicked();

    void on_pushButton_CloneReadback_clicked();

    void on_pushButton_Restore_clicked();

    void on_pushButton_Stop_clicked();

private:
    Ui::CloneDownloadWidget *ui;
    MainWindow* main_window_;
    std::string readback_folder_path_;
    std::string restore_folder_path_;
};

#endif // CLONEDOWNLOADWIDGET_H
