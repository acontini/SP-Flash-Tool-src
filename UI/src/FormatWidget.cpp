#include "FormatWidget.h"
#include "ui_FormatWidget.h"
#include "MainWindow.h"
#include "MainController.h"
#include "ICallback.h"
#include "../../Utility/constString.h"
#include "../../Utility/Utils.h"
#include "../../Host/Inc/RuntimeMemory.h"

static const int kMaxLength(18);

FormatWidget::FormatWidget(QTabWidget *parent, MainWindow *window) :
    TabWidgetBase(1, tr("Form&at"), parent),
    main_window_(window),
    ui_(new Ui::FormatWidget),
    storage_(HW_STORAGE_EMMC),
    platform_changed_(false),
    is_combo_dl_(false)
{
    ui_->setupUi(this);

    main_window_->main_controller()->GetPlatformSetting()->addObserver(this);
    main_window_->scatter_observer()->addObserver(this);

    ui_->comboBox_region->setVisible(false);
    ui_->label_region->setVisible(false);

    mUfsRegionList << "UFS_LU0" << "UFS_LU1" << "UFS_LU2";
    mEmmcRegionList << "EMMC_BOOT_1" << "EMMC_BOOT_2" << "EMMC_USER";

    InitDefault();
}

FormatWidget::~FormatWidget()
{
    delete ui_;
}

void FormatWidget::InitDefault()
{
    bool isEraseFlagVisible(false);
    isEraseFlagVisible = (main_window_->main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().IsNandOperation())
                        &&( main_window_->main_controller()->advance_on());

    //TODO(h):when nand flash && advanced mode, it should be visible.
    ui_->gb_ForamtEraseFlag->setVisible(isEraseFlagVisible);

    QRegExp regExpHex("0x[\\da-fA-F]{16}");

    ui_->lineEdit_BeginAddress->setValidator(new QRegExpValidator(regExpHex, ui_->lineEdit_BeginAddress));
    ui_->lineEdit_FormatLength->setValidator(new QRegExpValidator(regExpHex, ui_->lineEdit_FormatLength));

    ui_->frame_ManualFormatSettings->setHidden(true);
    ui_->groupBox_auto_fmt->setHidden(false);
    ui_->gb_ForamtEraseFlag->setHidden(true);

    ui_->radioButton_AutoFormat->setChecked(true);
}

void FormatWidget::LockOnUI()
{
    ui_->checkBox_Validation->setEnabled(false);
    ui_->toolButton_Start->setEnabled(false);
    ui_->toolButton_stop->setEnabled(true);
    ui_->frame_2->setEnabled(false);
    ui_->groupBox_auto_fmt->setEnabled(false);
    ui_->frame_ManualFormatSettings->setEnabled(false);
    ui_->gb_ForamtEraseFlag->setEnabled(false);
}

void FormatWidget::DoFinished()
{
    ui_->checkBox_Validation->setEnabled(true);
    ui_->toolButton_Start->setEnabled(true);
    ui_->toolButton_stop->setEnabled(false);
    ui_->frame_2->setEnabled(true);
    ui_->groupBox_auto_fmt->setEnabled(true);
    ui_->frame_ManualFormatSettings->setEnabled(true);
    ui_->gb_ForamtEraseFlag->setEnabled(true);
}

void FormatWidget::UpdateUI()
{
    U64 start_addr = ui_->lineEdit_BeginAddress->text().toULongLong(NULL,16);
    U64 len = ui_->lineEdit_FormatLength->text().toULongLong(NULL,16);

    QString addr_str;
    QString len_str;

    if(platform_changed_)
    {
        platform_changed_ = false;

        ui_->lineEdit_BeginAddress->setMaxLength(kMaxLength);
            addr_str.sprintf("0x%016llx", start_addr);
            ui_->lineEdit_BeginAddress->setText(addr_str);

        ui_->lineEdit_FormatLength->setMaxLength(kMaxLength);
            len_str.sprintf("0x%016llx", len);
            ui_->lineEdit_FormatLength->setText(len_str);

        if(main_window_->main_controller()->GetPlatformSetting()->getPlatformConfig().supportAutoFormatAll())
        {
            ui_->radioButton_fmt_all->setVisible(true);
        }
        else
        {
            ui_->radioButton_fmt_all->setVisible(false);
            ui_->radioButton_fmt_others->setChecked(true);
        }

        if(storage_!=HW_STORAGE_NAND)
        {
            ui_->gb_ForamtEraseFlag->setVisible(false);
        }
        else if(main_window_->main_controller()->advance_on())
        {
            ui_->gb_ForamtEraseFlag->setVisible(true);;
        }
    }
    else
    {
        ui_->retranslateUi(this);
    }
}

void FormatWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_FORMAT);

    tab_widget->setTabText(index, label);
}

void FormatWidget::SetShortCut(int cmd, const QString &shortCut)
{
    switch(cmd)
    {
    case FMT_START:
        ui_->toolButton_Start->setShortcut(shortCut);
        ui_->toolButton_Start->setToolTip(shortCut);
        break;

    case FMT_STOP:
        ui_->toolButton_stop->setShortcut(shortCut);
        ui_->toolButton_stop->setToolTip(shortCut);
        break;
    }
}

bool FormatWidget::get_auto_format() const
{
    return ui_->radioButton_AutoFormat->isChecked();
}

bool FormatWidget::get_validation() const
{
    return ui_->checkBox_Validation->isChecked();
}

bool FormatWidget::get_is_com_dl() const
{
    return is_combo_dl_;
}

U64 FormatWidget::get_begin_addr() const
{
    QString addr_str = ui_->lineEdit_BeginAddress->text();
    bool convert_ok = true;
    U64 addr = addr_str.toULongLong(&convert_ok,16);
    return convert_ok?addr:0;
}

U64 FormatWidget::get_fmt_length() const
{
    QString len_str = ui_->lineEdit_FormatLength->text();
    bool convert_ok = true;
    U64 addr = len_str.toULongLong(&convert_ok,16);
    return convert_ok?addr:0;
}

U32 FormatWidget::get_part_id() const
{
    QString part_str = ui_->comboBox_region->currentText();
    return Utils::getRegionPartId(part_str);
}

void FormatWidget::on_radioButton_AutoFormat_clicked()
{
    ui_->frame_ManualFormatSettings->setHidden(true);
    ui_->groupBox_auto_fmt->setHidden(false);
}

NUTL_EraseFlag_E FormatWidget::get_erase_flag() const
{
    if(ui_->radioButton_NormalErase->isChecked())
    {
        return NUTL_ERASE;
    }
    else if(ui_->radioButton_MarkAsBadBlock->isChecked())
    {
        return NUTL_MARK_BAD_BLOCK;
    }
    else if(ui_->radioButton_ForcedlyErase->isChecked())
    {
        return NUTL_FORCE_ERASE;
    }
    else
    {
        Q_ASSERT(0);
        return NUTL_ERASE;
    }
}

APCore::FormatSetting::AutoFormatFlag FormatWidget::get_auto_format_flag() const
{
    if(ui_->radioButton_fmt_all->isChecked())
    {
        return APCore::FormatSetting::Format_ALL;
    }
    else if(ui_->radioButton_fmt_others->isChecked())
    {
        return APCore::FormatSetting::Format_Except_BL;
    }
    else
    {
        Q_ASSERT(0);
        return APCore::FormatSetting::Format_FlagEnd;
    }

}

void FormatWidget::on_radioButton_ManualFormat_clicked()
{
    ui_->groupBox_auto_fmt->setHidden(true);

    ui_->frame_ManualFormatSettings->setHidden(false);
}

void FormatWidget::on_toolButton_Start_clicked()
{
    if(ValidateBeforeFormat())
    {
        main_window_->main_controller()->SetPlatformSetting();
        main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
        main_window_->main_controller()->QueueAJob(main_window_->CreateComboFormatSetting());
        main_window_->main_controller()->StartExecuting(
                    new SimpleCallback<MainWindow>(main_window_,&MainWindow::DoFinished));
        main_window_->LockOnUI();
        main_window_->GetOkDialog()->setWindowTitle(LoadQString(LANGUAGE_TAG, IDS_STRING_FORMAT_OK));
    }
}

void FormatWidget::on_toolButton_stop_clicked()
{
     main_window_->main_controller()->StopByUser();
}

bool FormatWidget::ValidateBeforeFormat()
{
    return main_window_->ValidateBeforeFormat();
}

void FormatWidget::ShowEraseFlag(bool show)
{
    ui_->gb_ForamtEraseFlag->setVisible(show);

    ui_->radioButton_MarkAsBadBlock->setVisible(false);
}

void FormatWidget::ShowMarkAsBadBlockFlag(bool show)
{
    ui_->radioButton_MarkAsBadBlock->setVisible(show);
}

void FormatWidget::onPlatformChanged()
{
     platform_changed_ = true;

     storage_ = main_window_->main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType();

     this->UpdateUI();
}

void FormatWidget::slot_platformChanged()
{
    onPlatformChanged();
}

void FormatWidget::OnScatterChanged(bool showRegion)
{
    is_combo_dl_ = showRegion;

    if(showRegion)
    {
        ui_->label_manualLabel->setText(tr("Specify the region, begin address and length of the format area."));

        ui_->comboBox_region->clear();

        if(HW_STORAGE_UFS == storage_)
        {
            ui_->comboBox_region->addItems(mUfsRegionList);
            ui_->comboBox_region->setCurrentIndex(2);
        }
        else
        {
            ui_->comboBox_region->addItems(mEmmcRegionList);
            ui_->comboBox_region->setCurrentIndex(2);
        }

    }
    else
        ui_->label_manualLabel->setText(tr("Specify the begin address and length of the format area."));

    ui_->comboBox_region->setVisible(showRegion);
    ui_->label_region->setVisible(showRegion);

}
