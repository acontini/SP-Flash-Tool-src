#include "OTPWriteDialog.h"
#include "ui_OTPWritedialog.h"

#include <QDir>
#include <QFileDialog>
#include <QTextCodec>
#include "../../Utility/FileUtils.h"
#include "../../Logger/Log.h"
#include "../../Host/Inc/RuntimeMemory.h"

OTPWriteDialog::OTPWriteDialog(QWidget *parent, Language_Tag tag) :
    QDialog(parent),
    ui(new Ui::OTPWriteDialog),
    otp_data_(),
    lang_tag(tag)
{
    ui->setupUi(this);

    setWindowTitle(tr("OTP Write Setting"));

    QRegExp regExpHex("0x[\\da-fA-F]{8}");
    QRegExp regExpHex16("0x[\\da-fA-F]{16}");
    ui->le_otp_waddr->setValidator(new QRegExpValidator(regExpHex16, ui->le_otp_waddr));
    ui->le_otp_wlen->setValidator(new QRegExpValidator(regExpHex, ui->le_otp_wlen));

    ui->le_otp_waddr->setText("0x");
    ui->le_otp_wlen->setText("0x");

    ui->label_region->setVisible(false);
    ui->comboBox_region->setVisible(false);

    QStringList regionList;

    regionList << "EMMC_BOOT_1" << "EMMC_BOOT_2" << "EMMC_USER";

    ui->comboBox_region->addItems(regionList);

    initDefault();
}

OTPWriteDialog::~OTPWriteDialog()
{
    delete ui;
}

void OTPWriteDialog::onScatterChanged(bool showRegion)
{
    ui->label_region->setVisible(showRegion);
    ui->comboBox_region->setVisible(showRegion);
}

void OTPWriteDialog::initDefault()
{
    ui->rb_write_only->setChecked(true);
}

unsigned int OTPWriteDialog::get_otp_addr()const
{
    QString addr_str = ui->le_otp_waddr->text();
    bool convert_ok = true;
    unsigned int addr = addr_str.toUInt(&convert_ok,16);
    return convert_ok?addr:0;
}

unsigned int OTPWriteDialog::get_otp_len()const
{
    QString len_str = ui->le_otp_wlen->text();
    bool convert_ok = true;

    unsigned int len = len_str.toUInt(&convert_ok,16);
    return convert_ok?len:0;
}

std::string OTPWriteDialog::get_otp_file()const
{
    QString file_str = ui->le_OTPfile_path->text();
    return std::string((const char *)file_str.toLocal8Bit());
}

const QSharedPointer<U8> &OTPWriteDialog::get_otp_data()const
{
    return this->otp_data_;
}

OTP_OPERATION OTPWriteDialog::get_otp_oper()const
{
    OTP_OPERATION oper;

    if(ui->rb_write_only->isChecked())
    {
        oper = OTP_WRITE;
    }
    if(ui->rb_otp_wl->isChecked())
    {
        oper = OTP_WRITE_AND_LOCK;
    }
    if(ui->rb_lock_only->isChecked())
    {
        oper = OTP_LOCK;
    }

    return oper;
}

void OTPWriteDialog::on_pb_OTPfile_path_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(
        this,
        LoadQString(lang_tag, IDS_STRING_OPEN_OTP_FILE),
        FileUtils::GetAppDirectory().c_str(),
        "");

    //use to parse Chinese Characters in file path string
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(file_name).data();
    //////////////////////////////////////////////////////////////

    unsigned char *data_buf;
    unsigned int file_len;

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);
        if(FileUtils::LoadFile(name.c_str(), &data_buf, &file_len, NULL))
        {
            LOG("Load file fail.");
            return;
        }

        otp_data_ = QSharedPointer<U8>(data_buf);
        ui->le_OTPfile_path->setText(file_name);

        QString s = NULL;
        s.sprintf("0x%08x", file_len);
        ui->le_otp_wlen->setText(s);
    }
}

void OTPWriteDialog::on_pushButton_OK_clicked()
{
    accept();
}

void OTPWriteDialog::on_pushButton_Cancel_clicked()
{
    reject();
}

void OTPWriteDialog::UpdateUI()
{
    ui->retranslateUi(this);
}
