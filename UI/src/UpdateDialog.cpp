#include "UpdateDialog.h"
#include "ui_UpdateDialog.h"

#include "../../UI/src/AsyncUpdater.h"
#include "../../UI/src/MainWindow.h"
#include "../../Host/Inc/RuntimeMemory.h"

UpdateDialog::UpdateDialog(MainWindow *parent, AsyncUpdater *updater) :
    QDialog(parent),
    async_updater(updater),
    ui(new Ui::UpdateDialog),
    mainWindow(parent)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //TODO: read release notes into text edit
    ui->textBrowser_note->setText(async_updater->GetReleaseNotes());

    UpdateUI();
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::on_pushButton_remind_clicked()
{
    //TODO: When to re update

    close();
}

void UpdateDialog::on_pushButton_install_clicked()
{
    async_updater->AsyncUpdateTool();
}

void UpdateDialog::UpdateUI()
{
    this->setWindowTitle(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_UPDATE));
    ui->label->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_NEW_DETECT));
    ui->pushButton_remind->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_REMIND));
    ui->pushButton_install->setText(LoadQString(mainWindow->GetLanguageTag(), IDS_STRING_INSTALL_UPDATE));
}
