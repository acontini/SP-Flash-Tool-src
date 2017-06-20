#include "CloneDownloadWidget.h"
#include "ui_CloneDownloadWidget.h"
#include "MainWindow.h"
#include "MainController.h"
#include "ICallback.h"
#include "./Utility/IniItem.h"

CloneDownloadWidget::CloneDownloadWidget(QTabWidget *parent, MainWindow *window) :
    TabWidgetBase(9, tr("&Clone Download"), parent),
    ui(new Ui::CloneDownloadWidget),
    main_window_(window)
{
    ui->setupUi(this);
}

CloneDownloadWidget::~CloneDownloadWidget()
{
    delete ui;
}

void CloneDownloadWidget::LockOnUI()
{
    enableControl(false);
}

void CloneDownloadWidget::DoFinished()
{
    enableControl(true);
}

void CloneDownloadWidget::UpdateUI()
{
    ui->retranslateUi(this);
}

void CloneDownloadWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_CLONEDOWNLOAD);

    tab_widget->setTabText(index, label);
}

void CloneDownloadWidget::SetShortCut(int , const QString &)
{

}

void CloneDownloadWidget::enableControl(bool enable)
{
    ui->pushButton_CloneReadback->setEnabled(enable);
    ui->pushButton_Restore->setEnabled(enable);
    ui->pushButton_Stop->setEnabled(!enable);
    ui->pushButton_readback->setEnabled(enable);
    ui->pushButton_restore->setEnabled(enable);
}

void CloneDownloadWidget::on_pushButton_readback_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                      tr("CloneDownload"),
                                      "");

    if(!dir.isEmpty())
    {
        ui->lineEdit_readback->setText(TO_NATIVE_(dir));
        readback_folder_path_ = TO_NATIVE_(dir).toStdString();
    }
}

void CloneDownloadWidget::on_pushButton_restore_clicked()
{
    IniItem item("history.ini", "CloneLastDir", "lastDir");
    QString dir = item.GetStringValue();
    QString dir_name = QFileDialog::getExistingDirectory(this,
                                                     tr("CloneDownload"),
                                                     dir
                                                     );

    item.SaveStringValue(TO_NATIVE_(dir_name));

    if(!dir_name.isEmpty())
    {
        ui->lineEdit_restore->setText(TO_NATIVE_(dir_name));
        restore_folder_path_ = TO_NATIVE_(dir_name).toStdString();
    }
}

void CloneDownloadWidget::on_pushButton_CloneReadback_clicked()
{
    if(ui->lineEdit_readback->text().isEmpty())
    {
        QMessageBox::warning(this, LoadQString(LANGUAGE_TAG, IDS_STRING_TOOL_NAME),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_WARNING_SELECTRBFOLDER),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_OK),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_CANCEL),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_HELP));

        return;
    }

    if(false == main_window_->ValidateBeforeFormat())
        return;

    main_window_->main_controller()->SetPlatformSetting();

    main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
    main_window_->main_controller()->QueueAJob(main_window_->CreateSCIRBSetting());
    main_window_->main_controller()->StartExecuting(
                new SimpleCallback<MainWindow>(main_window_,&MainWindow::DoFinished));
    main_window_->LockOnUI();
    main_window_->GetOkDialog()->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_READ_BACK_OK));
}

void CloneDownloadWidget::on_pushButton_Restore_clicked()
{
    if(ui->lineEdit_restore->text().isEmpty())
    {
        QMessageBox::warning(this,  LoadQString(LANGUAGE_TAG, IDS_STRING_TOOL_NAME),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_WARNING_RESTOREFOLDER_EMPTY),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_OK),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_CANCEL),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_HELP));

        return;
    }

    if(false == main_window_->ValidateBeforeFormat())
        return;

    main_window_->main_controller()->SetPlatformSetting();
    main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
    main_window_->main_controller()->QueueAJob(main_window_->CreateSCIRestoreSetting());
    main_window_->main_controller()->StartExecuting(
                new SimpleCallback<MainWindow>(main_window_, &MainWindow::DoFinished));

    main_window_->LockOnUI();
    main_window_->GetOkDialog()->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_WRITE_MEMORY_OK));
}

void CloneDownloadWidget::on_pushButton_Stop_clicked()
{
    main_window_->main_controller()->StopByUser();
}
