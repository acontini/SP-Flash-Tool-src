#ifndef WRITEMEMORYWIDGET_H
#define WRITEMEMORYWIDGET_H

#include <QWidget>
#include "TabWidgetBase.h"
#include <QtGui>

#include "../../BootRom/DOWNLOAD.H"
#include "../../Setting/WriteMemorySetting.h"
#include "ScatterObserver.h"

namespace Ui {
class WriteMemoryWidget;
}

class MainWindow;

class WriteMemoryWidget : public TabWidgetBase, public IScatterObj
{
    Q_OBJECT

public:
    explicit WriteMemoryWidget(QTabWidget *parent, MainWindow *window);
    ~WriteMemoryWidget();

    DECLARE_TABWIDGET_VFUNCS()

    virtual void OnScatterChanged(bool showRegion);

    QSharedPointer<APCore::WriteMemorySetting> CreateWriteMemSetting();

private slots:

    void on_toolButton_writeMemory_clicked();

    void on_toolButton_stop_clicked();

    void on_toolButton_openFile_clicked();

    U64 getBeginAddress() const;

private:
    MainWindow *main_window_;
    Ui::WriteMemoryWidget *ui_;

    U64 file_length_;

    QStringList mEmmcRegionList;
    QStringList mUfsRegionList;
};

#endif // WRITEMEMORYWIDGET_H
