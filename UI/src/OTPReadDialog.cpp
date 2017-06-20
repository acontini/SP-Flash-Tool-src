#include "OTPReadDialog.h"
#include "ui_OTPReadDialog.h"

#include <QDir>
#include <QFileDialog>
#include "../../Logger/Log.h"
#include "../../Utility/FileUtils.h"
#include "../../Host/Inc/RuntimeMemory.h"

OTPReadDialog::OTPReadDialog(QWidget *parent,Language_Tag _tag) :
    QDialog(parent),
    lang_tag(_tag),
    ui(new Ui::OTPReadDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("OTP Read Setting"));

    QRegExp regExpHex("0x[\\da-fA-F]{8}");
    QRegExp regExpHex16("0x[\\da-fA-F]{16}");
    ui->lineEdit_OTP_addr->setValidator(new QRegExpValidator(regExpHex16, ui->lineEdit_OTP_addr));
    ui->lineEdit_OTP_len->setValidator(new QRegExpValidator(regExpHex, ui->lineEdit_OTP_len));

    ui->lineEdit_OTP_addr->setText("0x");
    ui->lineEdit_OTP_len->setText("0x");

    ui->label_Region->setVisible(false);
    ui->comboBox_region->setVisible(false);

    QStringList regionList;

    regionList << "EMMC_BOOT_1" << "EMMC_BOOT_2" << "EMMC_USER";

    ui->comboBox_region->addItems(regionList);
}

OTPReadDialog::~OTPReadDialog()
{
    delete ui;
}

void OTPReadDialog::onScatterChanged(bool showRegion)
{
    //temp modify as hide it
    showRegion = false;
    ui->label_Region->setVisible(showRegion);
    ui->comboBox_region->setVisible(showRegion);
}

unsigned int OTPReadDialog::get_otp_addr()const
{
    QString addr_str = ui->lineEdit_OTP_addr->text();
    bool convert_ok = true;
    unsigned int addr = addr_str.toUInt(&convert_ok,16);
    return convert_ok?addr:0;
}

unsigned int OTPReadDialog::get_otp_len()const
{
    QString len_str = ui->lineEdit_OTP_len->text();
    bool convert_ok = true;
    unsigned int len = len_str.toUInt(&convert_ok,16);
    return convert_ok?len:0;
}
std::string OTPReadDialog::get_otp_file()const
{
    QString file_str = ui->lineEdit_OTP_save->text();
    return std::string((const char *)file_str.toLocal8Bit());
}

void OTPReadDialog::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(
                this,
                LoadQString(lang_tag, IDS_STRING_SAVE_AS),
                FileUtils::GetAppDirectory().c_str(),
                "");

    file_name = QDir::toNativeSeparators(file_name);
    if(file_name.isEmpty())
    {
        LOG("Fail to open file \"%s\".", file_name.data_ptr());
        return;
    }

    ui->lineEdit_OTP_save->setText(file_name);
}

void OTPReadDialog::on_pushButton_OK_clicked()
{
    accept();
}

void OTPReadDialog::on_pushButton_Cancel_clicked()
{
    reject();
}

void OTPReadDialog::UpdateUI()
{
    ui->retranslateUi(this);
}
