#include "CheckUpdateDialog.h"
#include "ui_CheckUpdateDialog.h"
#include "AsyncUpdater.h"
#include "../../UI/src/MainWindow.h"
#include "../../Host/Inc/RuntimeMemory.h"

CheckUpdateDialog::CheckUpdateDialog(MainWindow *parent, AsyncUpdater *updater) :
    QDialog(parent),
    async_checker(updater),
    ui(new Ui::CheckUpdateDialog),
    mainWindow(parent)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    SetOKButtonVisible(false);

    connect(async_checker, SIGNAL(signal_check_finish(const bool &)),
            this,SIGNAL(signal_check_finish(const bool&)));

    async_checker->CheckAsyncNewVersion();
    UpdateUI();
}

CheckUpdateDialog::~CheckUpdateDialog()
{
    delete ui;
}

void CheckUpdateDialog::SetCheckUpdateInfo(const QString &str)
{
    ui->label_info->setText(str);
}

void CheckUpdateDialog::SetOKButtonVisible(const bool &show)
{
    ui->pushButton_ok->setVisible(show);

    if(show)
    {
       ui->progressBar->setMaximum(1);
       ui->progressBar->setValue(1);
    }
}

void CheckUpdateDialog::on_pushButton_ok_clicked()
{
    close();
}

void CheckUpdateDialog::UpdateUI()
{
    this->setWindowTitle(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_UPDATE));
    if(ui->pushButton_ok->isHidden())
    {
        ui->label_info->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_CHECK_UPDATE));
        ui->pushButton_ok->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_OK));
    }
    else
    {
        ui->label_info->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_HAS_UPDATE));
        ui->pushButton_ok->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_OK));
    }
}
