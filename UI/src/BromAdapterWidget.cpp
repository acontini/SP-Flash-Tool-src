#include "BromAdapterWidget.h"
#include "ui_BromAdapterWidget.h"
#include "MainController.h"
#include "MainWindow.h"
#include "ICallback.h"
#include "../../Utility/constString.h"
#include "../../Utility/FileUtils.h"
#include "../../Utility/Utils.h"
#include "../../Setting/BromAdapterSetting.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "../../BootRom/flashtool_api.h"

#include <stdio.h>
#include <QSettings>


BromAdapterWidget::BromAdapterWidget(QTabWidget *parent, MainWindow *window) :
    TabWidgetBase(7, tr("&Brom Adapter"), parent),
    main_window_(window),
    ui_(new Ui::BromAdapterWidget),
    download_buf_(NULL),
    download_len_(0),
    brom_state_(BROM_DISCONNECT),
    chip_type_(UNKNOWN_BBCHIP_TYPE),
    ext_clock_(UNKNOWN_EXT_CLOCK),
    sec_header_len_(GetCipherHeaderLen())
{
    ui_->setupUi(this);

    QRegExp regExpHex("0x[\\da-fA-F]{8}");
    ui_->lineEdit_startAddress->setValidator(new QRegExpValidator(regExpHex, ui_->lineEdit_startAddress));
    ui_->lineEdit_jumpAddress->setValidator(new QRegExpValidator(regExpHex, ui_->lineEdit_jumpAddress));


    ui_->frame_jump->setVisible(false);
    ui_->toolButton_jump->setVisible(false);
    ui_->toolButton_connect->setVisible(false);
}

BromAdapterWidget::~BromAdapterWidget()
{
    delete ui_;
    if(NULL != download_buf_) {
    	free(download_buf_);
    }
}

void BromAdapterWidget::LockOnUI()
{
    lockUI(true);
}

void BromAdapterWidget::DoFinished()
{
    lockUI(false);

    brom_state_ = BROM_DISCONNECT;
    ui_->toolButton_connect->setIcon(QIcon(":/images/connect.png"));
    ui_->toolButton_connect->setText(tr("Connect"));
}

void BromAdapterWidget::UpdateUI()
{
    ui_->retranslateUi(this);
}

void BromAdapterWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_BROM_ADAPTER);

    tab_widget->setTabText(index, label);
}

void BromAdapterWidget::SetShortCut(int cmd, const QString &shortcut)
{
    switch(cmd)
    {
    case BROM_DOWNLOAD:
        ui_->toolButton_download->setShortcut(shortcut);
        ui_->toolButton_download->setToolTip(shortcut);
        break;

    case BORM_JUMP:
        ui_->toolButton_jump->setShortcut(shortcut);
        ui_->toolButton_jump->setToolTip(shortcut);
        break;

    case BROM_STOP:
        ui_->toolButton_stop->setShortcut(shortcut);
        ui_->toolButton_stop->setToolTip(shortcut);
        break;
    }
}

void BromAdapterWidget::lockUI(bool lock)
{
    ui_->toolButton_download->setEnabled(!lock);
    ui_->toolButton_jump->setEnabled(!lock);
    ui_->toolButton_stop->setEnabled(lock);

    ui_->lineEdit_dataPath->setEnabled(!lock);
    ui_->lineEdit_jumpAddress->setEnabled(!lock);
    ui_->lineEdit_startAddress->setEnabled(!lock);

    ui_->checkBox_jump->setEnabled(!lock);
    ui_->pushButton_data->setEnabled(!lock);
}

bool BromAdapterWidget::get_is_manual_addr() const
{
    return ui_->lineEdit_startAddress->text().length() > 0 ? true : false;
}

bool BromAdapterWidget::get_is_jump() const
{
    return ui_->checkBox_jump->isChecked();
}

U64 BromAdapterWidget::get_start_addr() const
{
    QString addr_str = ui_->lineEdit_startAddress->text();

    return Utils::ConvertStrToAddr(addr_str);
}

U64 BromAdapterWidget::get_jump_addr() const
{
    QString addr_str = ui_->lineEdit_startAddress->text();

    return Utils::ConvertStrToAddr(addr_str);
}

U64 BromAdapterWidget::get_buf_len() const
{
    return download_buf_ ? download_len_-sec_header_len_ : 0;
}

const unsigned char *BromAdapterWidget::get_buf()
{
    return download_buf_ ? download_buf_+sec_header_len_ : NULL;
}

void BromAdapterWidget::set_chip_info(const BOOT_RESULT *p_boot_result)
{
    chip_type_ = p_boot_result->m_bbchip_type;
    ext_clock_ = p_boot_result->m_ext_clock;

//    SetupFromIni();
}

void BromAdapterWidget::on_toolButton_download_clicked()
{

    if(NULL == download_buf_)
    {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_LOADDATA, "E_APP_LOAD_DA.htm");

        return;
    }

    main_window_->main_controller()->SetPlatformSetting();
    main_window_->SetPlatfromForBat();
    main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
    main_window_->main_controller()->QueueAJob(main_window_->CreateBromAdapterSetting());
    main_window_->main_controller()->StartExecuting(
                new SimpleCallback<MainWindow>(main_window_, &MainWindow::DoFinished));

    main_window_->LockOnUI();
    main_window_->GetOkDialog()->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_BROMDL_OK));
}

void BromAdapterWidget::on_toolButton_stop_clicked()
{
    main_window_->main_controller()->StopByUser();
}

void BromAdapterWidget::on_pushButton_data_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(
                this,
                LoadQString(LANGUAGE_TAG, IDS_STRING_OPEN_DOWNLOAD_DATA),
                FileUtils::GetAppDirectory().c_str());

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        FILE *fp;
        unsigned char *buf;
        unsigned int file_length = -1;

        fp = fopen(file_name.toLocal8Bit().constData(), "rb");

        if(NULL != fp)
        {
            fseek(fp, 0, SEEK_END);
            file_length = ftell(fp);

            if(file_length <= 0)
            {
                main_window_->ShowHelpContents(this, IDS_STRING_ERROR_DATALENGTH,
                                               "E_APP_BAT_DATA.htm");

                return;
            }

            if(NULL == (buf = (unsigned char*)malloc(file_length)))
            {
                main_window_->ShowHelpContents(this, IDS_STRING_ERROR_NOMEMORY,
                                               "E_SYS_MEMORY_LOW.htm");

                return;
            } else {
            	memset(buf, 0, file_length);
            }

            fseek(fp, 0, SEEK_SET);
            size_t readlen = fread(buf, 1, file_length, fp);
            assert(readlen == file_length);

#ifdef _WIN32
            const char *key = "B145612390489AABEF1CB89713235BDC";


            int ret = FlashTool_DecryptINI(buf, (unsigned char*)key, 16);

            if(S_DONE != ret)
            {
                main_window_->ShowHelpContents(this, IDS_STRING_ERROR_DECRYPT,
                                               "APP_BAT_DECRYPT.htm");
                return;
            }
#endif

            if(NULL != download_buf_)
                free(download_buf_);

            download_buf_ = buf;
            download_len_ = file_length;

            fclose(fp);

            ui_->lineEdit_dataPath->setText(file_name);

            SetupFromIni();

            ui_->toolButton_download->setEnabled(true);
            ui_->toolButton_jump->setEnabled(true);
        }
        else
        {
            ui_->lineEdit_dataPath->setText("");
        }
    }
}

void BromAdapterWidget::SetupFromIni()
{
    QString file_path = ui_->lineEdit_dataPath->text();

    if(file_path.length() <= 0)
        return;

    QString addr = Utils::GetAddrFromIniFile(chip_type_, file_path);

    ui_->lineEdit_startAddress->setText(addr);

    ui_->lineEdit_jumpAddress->setText(addr);
}

