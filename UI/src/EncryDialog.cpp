#include "EncryDialog.h"
#include "ui_EncryDialog.h"

#include "MainWindow.h"
#include <time.h>

EncryDialog::EncryDialog(QWidget *parent, MainWindow *window) :
    QDialog(parent),
    ui(new Ui::EncryDialog),
    main_windows_(window)
{
    ui->setupUi(this);

    ui->line->setVisible(false);

    Qt::WindowFlags flags = windowFlags() & ~Qt::WindowContextHelpButtonHint;

    setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);

    UpdateUI();
}

EncryDialog::~EncryDialog()
{
    delete ui;
}

bool EncryDialog::checkPassword()
{
    bool ret = false;

    QString strPassword1_day, strPassword2_day, strPassword3_day;
    QString strPassword1_week, strPassword2_week, strPassword3_week;
    QString strPassword1_month, strPassword2_month, strPassword3_month;

    unsigned long password_day, password_c1_day, password_c2_day, password_c3_day, password_c4_day;
    unsigned long password_week, password_c1_week, password_c2_week, password_c3_week, password_c4_week;
    unsigned long password_month, password_c1_month, password_c2_month, password_c3_month, password_c4_month;

    time_t now;
    struct tm *timenow;
    time(&now);
    timenow = localtime(&now);

    if(strPassword_ == "OnlyMTK!")
        return true;

    password_day = 5827;
    password_week = 3958;
    password_month = 45236;

    int wDay = timenow->tm_wday;
    int wMonth = 1 + timenow->tm_mon;

    password_day /= wDay;
    if ((wDay > 0) && (wDay <= 10))
        password_week /= 15;
    else if ((wDay > 10) && (wDay <= 20))
        password_week /= 21;
    else
        password_week /= 9;

    password_month /= wMonth;

    password_day = password_day << wDay;
    password_day = password_day + wMonth;
    password_week = password_week << wMonth;
    password_month = password_month << wMonth;

    password_day += wDay;
    password_month += wMonth;

    strPassword1_day.sprintf("%d", password_day);
    strPassword1_week.sprintf("%d", password_week);
    strPassword1_month.sprintf("%d", password_month);

    password_day = wDay * 58942;
    if ((wDay > 0) && (wDay <= 10))
        password_week = 542341;
    else if ((wDay > 10) && (wDay <= 20))
        password_week = 748634;
    else
        password_week = 123453;
    password_month = wMonth * 12385;

    password_c1_day = password_day >> 4;
    password_c1_week = password_week * 12 >> 4;
    password_c1_month = password_month * 21 >> 4;

    if (password_c1_day % 2)
    {
        if ((password_c1_day < 65) || (password_c1_day > 90))
            password_c1_day = password_c1_day % 90;

        while (password_c1_day < 65)
            password_c1_day +=25;
    }
    else
    {
        if ((password_c1_day < 97) || (password_c1_day > 122))
            password_c1_day = password_c1_day % 122;

        while (password_c1_day < 97)
            password_c1_day +=25;
    }

    if (password_c1_week % 2)
    {
        if ((password_c1_week < 65) || (password_c1_week > 90))
            password_c1_week = password_c1_week % 90;

        while (password_c1_week < 65)
            password_c1_week +=25;
    }
    else
    {
        if ((password_c1_week < 97) || (password_c1_week > 122))
            password_c1_week = password_c1_week % 122;

        while (password_c1_week < 97)
            password_c1_week +=25;
    }

    if (password_c1_month % 2)
    {
        if ((password_c1_month < 65) || (password_c1_month > 90))
            password_c1_month = password_c1_month % 90;

        while (password_c1_month < 65)
            password_c1_month +=25;
    }
    else
    {
        if ((password_c1_month < 97) || (password_c1_month > 122))
            password_c1_month = password_c1_month % 122;

        while (password_c1_month < 97)
            password_c1_month +=25;
    }

    password_c2_day = password_day >> 3;
    password_c2_week = password_week * 3 >> 3;
    password_c2_month = password_month * 16 >> 3;

    if (password_c2_day % 2)
    {
        if ((password_c2_day < 65) || (password_c2_day > 90))
            password_c2_day = password_c2_day % 90;

        while (password_c2_day < 65)
            password_c2_day +=25;
    }
    else
    {
        if ((password_c2_day < 97) || (password_c2_day > 122))
            password_c2_day = password_c2_day % 122;

        while (password_c2_day < 97)
            password_c2_day +=25;
    }

    if (password_c2_week % 2)
    {
        if ((password_c2_week < 65) || (password_c2_week > 90))
            password_c2_week = password_c2_week % 90;

        while (password_c2_week < 65)
            password_c2_week +=25;
    }
    else
    {
        if ((password_c2_week < 97) || (password_c2_week > 122))
            password_c2_week = password_c2_week % 122;

        while (password_c2_week < 97)
            password_c2_week +=25;
    }

    if (password_c2_month % 2)
    {
        if ((password_c2_month < 65) || (password_c2_month > 90))
            password_c2_month = password_c2_month % 90;

        while (password_c2_month < 65)
            password_c2_month +=25;
    }
    else
    {
        if ((password_c2_month < 97) || (password_c2_month > 122))
            password_c2_month = password_c2_month % 122;

        while (password_c2_month < 97)
            password_c2_month +=25;
    }

    password_c3_day = password_day >> 2;
    password_c3_week = password_week >> 2;
    password_c3_month = password_month >> 2;

    if (password_c3_day % 2)
    {
        if ((password_c3_day < 65) || (password_c3_day > 90))
            password_c3_day = password_c3_day % 90;

        while (password_c3_day < 65)
            password_c3_day +=25;
    }
    else
    {
        if ((password_c3_day < 97) || (password_c3_day > 122))
            password_c3_day = password_c3_day % 122;

        while (password_c3_day < 97)
            password_c3_day +=25;
    }

    if (password_c3_week % 2)
    {
        if ((password_c3_week < 65) || (password_c3_week > 90))
            password_c3_week = password_c3_week % 90;

        while (password_c3_week < 65)
            password_c3_week +=25;
    }
    else
    {
        if ((password_c3_week < 97) || (password_c3_week > 122))
            password_c3_week = password_c3_week % 122;

        while (password_c3_week < 97)
            password_c3_week +=25;
    }

    if (password_c3_month % 2)
    {
        if ((password_c3_month < 65) || (password_c3_month > 90))
            password_c3_month = password_c3_month % 90;

        while (password_c3_month < 65)
            password_c3_month +=25;
    }
    else
    {
        if ((password_c3_month < 97) || (password_c3_month > 122))
            password_c3_month = password_c3_month % 122;

        while (password_c3_month < 97)
            password_c3_month +=25;
    }

    password_c4_day = password_day >> 1;
    password_c4_week = password_week >> 1;
    password_c4_month = password_month >> 1;

    if (password_c4_day % 2)
    {
        if ((password_c4_day < 65) || (password_c4_day > 90))
            password_c4_day = password_c4_day % 90;

        while (password_c4_day < 65)
            password_c4_day +=25;
    }
    else
    {
        if ((password_c4_day < 97) || (password_c4_day > 122))
            password_c4_day = password_c4_day % 122;

        while (password_c4_day < 97)
            password_c4_day +=25;
    }

    if (password_c4_week % 2)
    {
        if ((password_c4_week < 65) || (password_c4_week > 90))
            password_c4_week = password_c4_week % 90;

        while (password_c4_week < 65)
            password_c4_week +=25;
    }
    else
    {
        if ((password_c4_week < 97) || (password_c4_week > 122))
            password_c4_week = password_c4_week % 122;

        while (password_c4_week < 97)
            password_c4_week +=25;
    }

    if (password_c4_month % 2)
    {
        if ((password_c4_month < 65) || (password_c4_month > 90))
            password_c4_month = password_c4_month % 90;

        while (password_c4_month < 65)
            password_c4_month +=25;
    }
    else
    {
        if ((password_c4_month < 97) || (password_c4_month > 122))
            password_c4_month = password_c4_month % 122;

        while (password_c4_month < 97)
            password_c4_month +=25;
    }

    strPassword2_day.sprintf("%c%c%c%c", password_c1_day, password_c2_day, password_c3_day, password_c4_day);
    strPassword2_week.sprintf("%c%c%c%c", password_c1_week, password_c2_week, password_c3_week, password_c4_week);
    strPassword2_month.sprintf("%c%c%c%c", password_c1_month, password_c2_month, password_c3_month, password_c4_month);

    password_day = 2948201;
    password_week = 8138947;
    password_month = 6023848;
    password_day /= wDay;
    password_day *= wMonth;
    if ((wDay > 0) && (wDay <= 10))
        password_week = password_week >> 2;
    else if ((wDay > 10) && (wDay <= 20))
        password_week = password_week << 5;
    else
        password_week /= 3;
    password_month /= wMonth;
    password_day = password_day >> 7;
    password_week = password_week >> 3;
    password_month = password_month >> 6;

    strPassword3_day.sprintf("%d", password_day);
    strPassword3_week.sprintf("%d", password_week);
    strPassword3_month.sprintf("%d", password_month);

    strPassword_day_ = strPassword1_day;
    strPassword_day_ += strPassword2_day;
    strPassword_day_ += strPassword3_day;
    strPassword_week_ = strPassword1_week;
    strPassword_week_ += strPassword2_week;
    strPassword_week_ += strPassword3_week;
    strPassword_month_ = strPassword1_month;
    strPassword_month_ += strPassword2_month;
    strPassword_month_ += strPassword3_month;

    if ((strPassword_ == strPassword_day_) || (strPassword_ == strPassword_week_)
            || (strPassword_ == strPassword_month_) || (strPassword_ == "rlaghdtjrqkqh0313"))
    {
        ret = true;
    }
    else
    {
        strPassword_ = "";

        ret = false;
    }

    return ret;
}

void EncryDialog::on_pushButton_OK_clicked()
{
    strPassword_ = ui->lineEdit_password->text();

    if(strPassword_.isEmpty())
    {
        //TODO
       // main_windows_->ShowHelpContents(this, , "");
        return;
    }

    if(checkPassword())
    {
        main_windows_->SetSCIDownloadVisible(true);

        accept();
    }
    else
    {
        main_windows_->SetSCIDownloadVisible(false);
        ui->lineEdit_password->setText(tr(""));

        QMessageBox::warning(this,
                             LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_TOOL_NAME),
                             LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_PWD_ERROR),
                             LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_OK));
    }
}

void EncryDialog::on_pushButton_Cancel_clicked()
{
    main_windows_->SetSCIDownloadVisible(false);

    reject();
}

void EncryDialog::on_lineEdit_password_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->pushButton_OK->setEnabled(false);
    else
        ui->pushButton_OK->setEnabled(true);
}

void EncryDialog::on_lineEdit_password_returnPressed()
{
    on_pushButton_OK_clicked();
}

void EncryDialog::closeEvent(QCloseEvent *)
{
    main_windows_->SetSCIDownloadVisible(false);
}

void EncryDialog::showEvent(QShowEvent *)
{
    ui->lineEdit_password->setText(tr(""));
}

void EncryDialog::UpdateUI()
{
    this->setWindowTitle(LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_ENCRY));
    ui->label_hint->setText(LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_PASSWORD));
    ui->pushButton_OK->setText(LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_OK));
    ui->pushButton_Cancel->setText(LoadQString(main_windows_->GetLanguageTag(), IDS_STRING_CANCEL));
}
