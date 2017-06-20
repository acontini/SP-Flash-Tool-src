#include "NandWidget.h"
#include "ui_NandWidget.h"

NandWidget::NandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NandWidget)
{
    ui->setupUi(this);
}

NandWidget::~NandWidget()
{
    delete ui;
}

void NandWidget::setNandInfo(const QString &id, const QString &size, const QString &page_size,
                             const QString &spare_size, const QString &page_per_block,
                             const QString &bmt_exist)
{
    ui->lineEdit_id->setText(id);
    ui->lineEdit_size->setText(size);
    ui->lineEdit_pageSize->setText(page_size);
    ui->lineEdit_spareSize->setText(spare_size);
    ui->lineEdit_ppb->setText(page_per_block);
    ui->lineEdit_bmtExist->setText(bmt_exist);
}
