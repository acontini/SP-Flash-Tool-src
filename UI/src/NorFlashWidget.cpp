#include "NorFlashWidget.h"
#include "ui_NorFlashWidget.h"

NorFlashWidget::NorFlashWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NorFlashWidget)
{
    ui->setupUi(this);
}

NorFlashWidget::~NorFlashWidget()
{
    delete ui;
}

void NorFlashWidget::setNorFlashInfo(const QString &id, const QString &size, const QString &otp_size)
{
    ui->lineEdit_id->setText(id);
    ui->lineEdit_size->setText(size);
    ui->lineEdit_otpSize->setText(otp_size);
}
