#include "UfsWidget.h"
#include "ui_UfsWidget.h"

UfsWidget::UfsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UfsWidget)
{
    ui->setupUi(this);
}

UfsWidget::~UfsWidget()
{
    delete ui;
}

void UfsWidget::setUfsInfo(const QString &lu0_size, const QString &lu1_size,
                           const QString &lu2_size/*, const QString &lu3_size*/)
{
    ui->lineEdit_lu0->setText(lu0_size);
    ui->lineEdit_lu1->setText(lu1_size);
    ui->lineEdit_lu2->setText(lu2_size);
    //ui->lineEdit_lu3->setText(lu3_size);
}
