#ifndef MEMORYTESTWIDGET_H
#define MEMORYTESTWIDGET_H

#include "TabWidgetBase.h"
#include "../../Setting/MemoryTestSetting.h"
#include "../../Setting/PlatformSetting.h"
#include "ScatterObserver.h"

#include <QColor>
#include <QSharedPointer>

namespace Ui
{
class MemoryTestWidget;
}
class MainWindow;

class MemoryTestWidget : public TabWidgetBase, public APCore::IPlatformOb, public IScatterObj
{
    Q_OBJECT
public:
    explicit MemoryTestWidget(QTabWidget *parent, MainWindow *window);
    ~MemoryTestWidget();

    DECLARE_TABWIDGET_VFUNCS()

    virtual void onPlatformChanged();
    virtual void OnScatterChanged(bool showRegion);

    QSharedPointer<APCore::MemoryTestSetting> CreateMemtestSetting();

    bool isSelectNothing();
    void ShowEMMCTest(bool isShow);
    void ShowNANDTest(bool isShow);
    void ShowRAMTest(bool isShow);
    void ShowDRAMFlipTest(bool isShow);

private:
    void HideFlashTestItems();
    void selectAllTestItems(bool select);

    MainWindow *main_window_;
    Ui::MemoryTestWidget *ui_;


signals:

public slots:

private slots:
    void on_toolButton_start_clicked();
    void on_SELECT_ALL_clicked();

    void slot_MemoryTestCallback(const QString &msg, QColor color);
    void on_toolButton_stop_clicked();
};

#endif // MEMORYTESTWIDGET_H
