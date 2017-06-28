#include "ReadBackAddressDialog.h"
#include "ui_ReadBackAddressDialog.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "../../Utility/Utils.h"

#include <QtGlobal>

ReadBackAddressDialog::ReadBackAddressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadBackAddressDialog)
{
    ui->setupUi(this);
    setModal(true);

    connect(ui->radioButton_hex,SIGNAL(toggled(bool)),this,SLOT(onToggleHex(bool)));
    connect(ui->radioButton_decimal,SIGNAL(toggled(bool)),this,SLOT(onToggleDec(bool)));

    ui->label_Region->setVisible(false);
    ui->comboBox_region->setVisible(false);

    mUfsRegionList << "UFS_LU0" << "UFS_LU1" << "UFS_LU2";
    mEmmcRegionList << "EMMC_BOOT_1" << "EMMC_BOOT_2" << "EMMC_USER";
}

ReadBackAddressDialog::~ReadBackAddressDialog()
{
    delete ui;
}

void ReadBackAddressDialog::OnScatterChanged(bool showRegion)
{
    if(showRegion)
    {
        ui->comboBox_region->clear();
        if(HW_STORAGE_EMMC == storage_type_)
            ui->comboBox_region->addItems(mEmmcRegionList);
        else if(HW_STORAGE_UFS == storage_type_)
            ui->comboBox_region->addItems(mUfsRegionList);
    }
    ui->label_Region->setVisible(showRegion);
    ui->comboBox_region->setVisible(showRegion);

}

void ReadBackAddressDialog::SetCurrentState(NUTL_ReadFlag_E read_flag,
                     const QString &addr,
                                            const QString &len,
                                            const U32 part_id)
{
    ReadbackRule readback_rule;
    readback_rule.QueryReadBackRule(storage_type_);
    ui->groupBox_readMethod->setVisible(readback_rule.does_set_rb_flag());

    //set text max length
    ui->radioButton_hex->setChecked(true);
    ui->lineEdit_startAddress->setText(addr);
    ui->lineEdit_length->setText(len);

    if(readback_rule.does_set_rb_flag())
   {
       switch(read_flag)
       {
       case NUTL_READ_PAGE_SPARE:
           ui->radioButton_readPageSpare->setChecked(true);
           break;
       case NUTL_READ_PAGE_ONLY:
           ui->radioButton_readPageOnly->setChecked(true);
           break;
       case NUTL_READ_SPARE_ONLY:
           ui->radioButton_readSpareOnly->setChecked(true);
           break;
       case NUTL_READ_PAGE_WITH_ECC:
           ui->radioButton_readPageWithECC->setChecked(true);
           break;
       default:
        Q_ASSERT(0);
       }
   }

    if(HW_STORAGE_EMMC == storage_type_)
    {
        switch(part_id)
        {
        case EMMC_PART_BOOT1:
            ui->comboBox_region->setCurrentIndex(0);
            break;

        case EMMC_PART_BOOT2:
            ui->comboBox_region->setCurrentIndex(1);
            break;

        case EMMC_PART_USER:
            ui->comboBox_region->setCurrentIndex(2);
            break;
        }
    }
    else if(HW_STORAGE_UFS == storage_type_)
    {
        switch(part_id)
        {
        case UFS_PART_LU0:
            ui->comboBox_region->setCurrentIndex(0);
            break;

        case UFS_PART_LU1:
            ui->comboBox_region->setCurrentIndex(1);
            break;

        case UFS_PART_LU2:
            ui->comboBox_region->setCurrentIndex(2);
            break;
        }
    }
}

void ReadBackAddressDialog::SetStorageType(HW_StorageType_E storage_type)
{
    this->storage_type_ = storage_type;
}

void ReadBackAddressDialog::UpdateUI()
{
    ui->retranslateUi(this);
}

NUTL_ReadFlag_E ReadBackAddressDialog::read_flag()
{
    NUTL_ReadFlag_E flag = NUTL_READ_FLAG_END;
    if(ui->radioButton_readPageSpare->isChecked())
    {
        flag = NUTL_READ_PAGE_SPARE;
    }
    else if(ui->radioButton_readPageOnly->isChecked())
    {
        flag = NUTL_READ_PAGE_ONLY;
    }
    else if(ui->radioButton_readSpareOnly->isChecked())
    {
        flag = NUTL_READ_SPARE_ONLY;
    }
    else if(ui->radioButton_readPageWithECC->isChecked())
    {
        flag = NUTL_READ_PAGE_WITH_ECC;
    }
    else
    {
        Q_ASSERT(0);
    }
    return flag;
}

U64 ReadBackAddressDialog::addr()
{
    QString addr_str = ui->lineEdit_startAddress->text();

    U64 addr = 0;
    if(ui->radioButton_hex->isChecked())
    {
        addr = addr_str.toULongLong(NULL,16);
    }
    else
    {
        addr = addr_str.toULongLong();
    }
    return addr;
}

U64 ReadBackAddressDialog::length()
{
    QString len_str = ui->lineEdit_length->text();
    U64 len = 0;
    if(ui->radioButton_hex->isChecked())
    {
        len = len_str.toULongLong(NULL,16);
    }
    else
    {
        len = len_str.toULongLong();
    }
    return len;
}

U32 ReadBackAddressDialog::region_id()
{
    QString part_str = ui->comboBox_region->currentText();
    return Utils::getRegionPartId(part_str);
}

const QString ReadBackAddressDialog::region()
{
   return ui->comboBox_region->currentText();
}

void ReadBackAddressDialog::onHexToDec()
{
    //hex to dec
    U64 addr = ui->lineEdit_startAddress->text().toULongLong(NULL,16);
    U64 len  = ui->lineEdit_length->text().toULongLong(NULL,16);

    ui->lineEdit_startAddress->setText(QString::number(addr));
    ui->lineEdit_length->setText(QString::number(len));
}

void ReadBackAddressDialog::onDecToHex()
{
    //dec to hex
    U64 addr = ui->lineEdit_startAddress->text().toULongLong(NULL,16);
    U64 len  = ui->lineEdit_length->text().toULongLong(NULL,16);

    QString addr_str;

         addr_str.sprintf("0x%016llx", addr);
    
    ui->lineEdit_startAddress->setText(addr_str);

    QString len_str;
    len_str.sprintf("0x%016llx", len);
    ui->lineEdit_length->setText(len_str);
}

void ReadBackAddressDialog::onToggleHex(bool toggled)
{
    if(toggled)
    {
        onDecToHex();
    }
}

void ReadBackAddressDialog::onToggleDec(bool toggled)
{
    if(toggled)
    {
        onHexToDec();
    }
}

void ReadBackAddressDialog::on_radioButton_readPageSpare_clicked()
{

}

void ReadBackAddressDialog::slot_platform_changed(HW_StorageType_E &storage)
{
    storage_type_ = storage;

    ReadbackRule readback_rule;
    readback_rule.QueryReadBackRule(storage_type_);
    //set read flag visible
    ui->groupBox_readMethod->setVisible(readback_rule.does_set_rb_flag());
}


