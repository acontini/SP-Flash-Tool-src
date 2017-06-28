#include "EMMCWidget.h"
#include "ui_EMMCWidget.h"

EMMCWidget::EMMCWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EMMCWidget)
{
    ui->setupUi(this);
}

EMMCWidget::~EMMCWidget()
{
    delete ui;
}

void EMMCWidget::setEmmcInfo(const QString &boot1_size, const QString &boot2_size,
                             const QString &rmpb_size, const QString &gp1_size,
                             const QString &gp2_size, const QString &gp3_size,
                             const QString &gp4_size, const QString &ua_size)
{
    ui->lineEdit_boot1->setText(boot1_size);
    ui->lineEdit_boot2->setText(boot2_size);
    ui->lineEdit_rpmb->setText(rmpb_size);
    ui->lineEdit_gp1->setText(gp1_size);
    ui->lineEdit_gp2->setText(gp2_size);
    ui->lineEdit_gp3->setText(gp3_size);
    ui->lineEdit_gp4->setText(gp4_size);
    ui->lineEdit_ua->setText(ua_size);
}
