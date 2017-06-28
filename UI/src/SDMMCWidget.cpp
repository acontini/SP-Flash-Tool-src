#include "SDMMCWidget.h"
#include "ui_SDMMCWidget.h"

SDMMCWidget::SDMMCWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SDMMCWidget)
{
    ui->setupUi(this);
}

SDMMCWidget::~SDMMCWidget()
{
    delete ui;
}

void SDMMCWidget::setSdmmcInfo(const QString &id, const QString &size)
{
    ui->lineEdit_id->setText(id);
    ui->lineEdit_size->setText(size);
}
