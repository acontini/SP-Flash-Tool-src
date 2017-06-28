#include "ChipInfoWidget.h"
#include "ui_ChipInfoWidget.h"

ChipInfoWidget::ChipInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChipInfoWidget)
{
    ui->setupUi(this);
}

ChipInfoWidget::~ChipInfoWidget()
{
    delete ui;
}

void ChipInfoWidget::setGeneralInfo(const QString &chip_name, const QString &chip_version,
                                    const QString &ext_clock, const QString &ram_type,
                                    const QString &ram_size, const QString &sram_size)
{
    ui->lineEdit_chipName->setText(chip_name);
    ui->lineEdit_chipVersion->setText(chip_version);
    ui->lineEdit_extClock->setText(ext_clock);
    ui->lineEdit_ramType->setText(ram_type);
    ui->lineEdit_ramSize->setText(ram_size);
    ui->lineEdit_sramSize->setText(sram_size);
}
