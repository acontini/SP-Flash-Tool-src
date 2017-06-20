#include "DeviceTestWidget.h"

#include "ui_DeviceTestWidget.h"
#include "MainWindow.h"
#include "MainController.h"
#include "ICallback.h"
#include "../../Utility/constString.h"
#include "../../Host/Inc/RuntimeMemory.h"

DeviceTestWidget::DeviceTestWidget(QTabWidget *parent, MainWindow *window) :
    TabWidgetBase(4, tr("Device&Test"), parent),
    main_window_(window),
    ui_(new Ui::DeviceTestWidget)
{
    ui_->setupUi(this);

    connect(main_window_, SIGNAL(signal_DeviceTestCallback(const QString &,QColor)),
            this, SLOT(slot_DeviceTestCallback(const QString &,QColor)));
}

DeviceTestWidget::~DeviceTestWidget()
{
    delete ui_;
}

void DeviceTestWidget::LockOnUI()
{
    ui_->groupBox->setEnabled(false);
    ui_->toolButton_start->setEnabled(false);
    ui_->toolButton_stop->setEnabled(true);
}

void DeviceTestWidget::DoFinished()
{
    ui_->groupBox->setEnabled(true);
    ui_->toolButton_start->setEnabled(true);
    ui_->toolButton_stop->setEnabled(false);
}

void DeviceTestWidget::UpdateUI()
{
    ui_->retranslateUi(this);
    ui_->SELECT_ALL->setShortcut(Qt::CTRL + Qt::Key_A);

}

void DeviceTestWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_DEVICE_TEST);

    tab_widget->setTabText(index, label);
}

void DeviceTestWidget::SetShortCut(int cmd, const QString &shortcut)
{
    switch(cmd)
    {
    case MT_START:
        ui_->toolButton_start->setShortcut(shortcut);
        ui_->toolButton_start->setToolTip(shortcut);
        break;

    case MT_STOP:
        ui_->toolButton_stop->setShortcut(shortcut);
        ui_->toolButton_stop->setToolTip(shortcut);
        break;
    }
}

void DeviceTestWidget::on_toolButton_start_clicked()
{
    //if(main_window_->ValidateBeforeDeviceTest())
    //{
        main_window_->main_controller()->SetPlatformSetting();
        main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
        main_window_->main_controller()->QueueAJob(main_window_->CreateDeviceTestSetting());
        main_window_->main_controller()->StartExecuting(
                    new SimpleCallback<MainWindow>(main_window_,&MainWindow::DoFinished));
        main_window_->LockOnUI();
        main_window_->GetOkDialog()->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_DEVICE_TEST_OK));
    //}
}

void DeviceTestWidget::on_SELECT_ALL_clicked()
{
#define BATTERY_TEST_CHECKED \
    ( ui_->BATTERY_TEST->isHidden()    \
    || (ui_->BATTERY_TEST->isChecked()    \
        && ui_->Battery_on->isChecked()    \
        && ui_->Battery_auto->isChecked()    \
        && ui_->Battery_off->isChecked()))

    if (BATTERY_TEST_CHECKED )
    {
        selectAllTestItems(false);
    }
    else
    {
        selectAllTestItems(true);
    }
}

void DeviceTestWidget::selectAllTestItems(bool select)
{
    ui_->BATTERY_TEST->setChecked(select);
    ui_->Battery_on->setChecked(select);
    ui_->Battery_auto->setChecked(select);
    ui_->Battery_off->setChecked(select);
}

QSharedPointer<APCore::DeviceTestSetting> DeviceTestWidget::CreateDeviceTestSetting()
{
    QSharedPointer<APCore::DeviceTestSetting> setting(new APCore::DeviceTestSetting());

    if(!ui_->BATTERY_TEST->isHidden())
    {
        setting->set_battery_test(ui_->BATTERY_TEST->isChecked());
        setting->set_with_battery(ui_->Battery_on->isChecked());
        setting->set_auto_detect(ui_->Battery_auto->isChecked());
        setting->set_without_battery(ui_->Battery_off->isChecked());
    }
    return setting;
}

void DeviceTestWidget::slot_DeviceTestCallback(const QString &msg, QColor color)
{
    ui_->DeviceTestReport->setUpdatesEnabled(FALSE);
    ui_->DeviceTestReport->setTextColor(color);
    ui_->DeviceTestReport->append(msg);
    /* mtk10304[nailiang.song] data_bus_test + addr_bus_test, hit append(msg) 93 times, then tool crash,
    comment qApp->processEvents(), tool NOT crash */
    //qApp->processEvents();
    ui_->DeviceTestReport->setUpdatesEnabled(TRUE);
}

void DeviceTestWidget::on_toolButton_stop_clicked()
{
    main_window_->main_controller()->StopByUser();
}
