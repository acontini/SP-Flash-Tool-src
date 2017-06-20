#ifndef DEVICETESTWIDGET_H
#define DEVICETESTWIDGET_H

#include "TabWidgetBase.h"
#include "../../Setting/DeviceTestSetting.h"

#include <QColor>
#include <QSharedPointer>

namespace Ui
{
class DeviceTestWidget;
}
class MainWindow;

class DeviceTestWidget : public TabWidgetBase
{
    Q_OBJECT
public:
    explicit DeviceTestWidget(QTabWidget *parent, MainWindow *window);
    ~DeviceTestWidget();

    DECLARE_TABWIDGET_VFUNCS()

    QSharedPointer<APCore::DeviceTestSetting> CreateDeviceTestSetting();

private:
    void selectAllTestItems(bool select);

    MainWindow *main_window_;
    Ui::DeviceTestWidget *ui_;


signals:

public slots:

private slots:
    void on_toolButton_start_clicked();
    void on_SELECT_ALL_clicked();

    void slot_DeviceTestCallback(const QString &msg, QColor color);
    void on_toolButton_stop_clicked();
};

#endif // DEVICETESTWIDGET_H
