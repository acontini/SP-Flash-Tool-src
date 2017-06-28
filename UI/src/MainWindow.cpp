#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "DownloadWidget.h"
#include "ReadBackWidget.h"
#include "MemoryTestWidget.h"
#include "DeviceTestWidget.h"
#include "WelcomeWidget.h"
#include "ParameterWidget.h"
#include "FormatWidget.h"
#include "BromAdapterWidget.h"
#include "WriteMemoryWidget.h"
#include "SCIDownloadWidget.h"
#include "CloneDownloadWidget.h"
#include "AboutDialog.h"
#include "CheckUpdateDialog.h"
#include "ProcessingDialog.h"
#include "MainController.h"
#include "MainWindowCallback.h"
#include "Assistant.h"
#include "ScatterObserver.h"
#include "OkDialog.h"
#include "UpdateDialog.h"
#include "AsyncUpdater.h"
#include "../../BootRom/brom.h"
#include "ChipInfoWidget.h"
#include "NandWidget.h"
#include "EMMCWidget.h"
#include "SDMMCWidget.h"
#include "NorFlashWidget.h"
#include "UfsWidget.h"
#include "../../Setting/ConnBromUSBSetting.h"
#include "../../Setting/ConnBromUARTSetting.h"
#include "../../ConsoleMode/Config.h"
#include "../../Utility/FileUtils.h"
#include "../../Utility/constString.h"
#include "../../Utility/Utils.h"
#include "../../Utility/IniItem.h"
#include "../../Utility/version.h"
#include "../../Err/AppException.h"
#include "../../Err/FlashToolErrorCodeDef.h"
#include "../../Err/Exception.h"
#include "../../Err/ErrorLookup.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "EncryDialog.h"
#include "../../BootRom/internal_api.h"

#include <QtDebug>
#include <QtGui>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    info_widget_(NULL),
    nor_widget_(NULL),
    nand_widget_(NULL),
    emmc_widget_(NULL),
    sdmmc_widget_(NULL),
    ufs_widget_(NULL),
    language_tag(LANGUAGE_ENGLISH),
    about_dialog(NULL),
    processing_dialog_(new ProcessingDialog(this)),
    option_dialog (new OptionDialog(this, this)),
    ok_dialog(new OkDialog(this)),
    checkupdate_dialog(NULL),
    update_dialog(NULL),
    encry_dialog(NULL),
    async_updater(new AsyncUpdater(this)),
    current_platform(new PlatformObj()),
    scatter_observer_(new ScatterObserver()),
    mbromAdapter_on(false),
    msecurity_on(false),
    mSCIDownload_on(false),
    mLog_on(false),
    mDeviceTest_on(false),
    lockui_on(false),
    show_welcome(true),
    assistant_(new Assistant(this)),
    is_auto_polling_enable_(false),
    auto_polling_count_(0),
    auto_polling_upper_limit_(0),
    finish_(false),
    is_ok_(true),
    is_scidl_visible_(false)
{
    ui->setupUi(this);

    SetUI();

    CreateWidget();

    CreateConnect();

    SetInitState();

    InitShortcuts();


#ifdef _WIN32
    if(!ToolInfo::IsCustomerVer())
    {
        IniItem item("option.ini", "CheckUpdate", "Update");
        if(item.GetBooleanValue() == true )
            async_updater->CheckAsyncNewVersion();
    }
#endif
}

void MainWindow::SetUI()
{
    ui->progressBar->setTextVisible(true);
    ui->actionExportPartial_Format_Download->setVisible(false);
    ui->actionCerfitication_download->setVisible(false);

    if(ToolInfo::IsCustomerVer())
    {
        ui->actionExportFormat->setVisible(false);
        ui->actionReadback->setVisible(false);
    }

    QImage image = Utils::AddStringToImage(":/images/Phone.png", "MediaTek");

    ui->ImageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::CreateWidget()
{
    main_controller_ = new MainController(this,this);
    main_callbacks_ = new MainWindowCallback(this,this);
    send_report_ = new SendReport(this, this);
    thread_timer_ = new QTimer(this);
    thread_timer_->setSingleShot(true);

    download_widget = new DownloadWidget(ui->tabWidget, this);
    memtest_widget = new MemoryTestWidget(ui->tabWidget, this);
    devtest_widget = new DeviceTestWidget(ui->tabWidget, this);
    parameter_widget = new ParameterWidget(ui->tabWidget, this);
    format_widget = new FormatWidget(ui->tabWidget, this);
    writeMemory_widget = new WriteMemoryWidget(ui->tabWidget, this);
    readback_widget = new ReadBackWidget(ui->tabWidget, this);
    welcome_widget = new WelcomeWidget(ui->tabWidget, this);
    bromAdapter_widget = new BromAdapterWidget(ui->tabWidget, this);
    sciDownload_widget_ = new SCIDownloadWidget(ui->tabWidget, this);
    cloneDownload_wdiget_ = new CloneDownloadWidget(ui->tabWidget, this);

    tab_widgets.push_back(welcome_widget);
    tab_widgets.push_back(format_widget);
    tab_widgets.push_back(download_widget);
    tab_widgets.push_back(readback_widget);
    tab_widgets.push_back(memtest_widget);

    for(std::list<TabWidgetBase*>::const_iterator it=tab_widgets.begin();
        it != tab_widgets.end(); ++it)
    {
        (*it)->Attach();
    }

    ui->tabWidget->setCurrentWidget(download_widget);
}

void MainWindow::CreateConnect()
{
    main_controller()->GetPlatformSetting()->addObserver(current_platform);
    main_controller()->GetPlatformSetting()->addObserver(option_dialog);

    connect(main_controller_, SIGNAL(signal_err_msg(int, const std::string&)),
            this, SLOT(slot_show_err(int, const std::string&)));
    connect(this, SIGNAL(signal_err_msg(int, const std::string&)),
            this, SLOT(slot_show_err(int, const std::string&)));
    connect(this,SIGNAL(signal_UnlockUI()),SLOT(slot_UnlockUI()));
    connect(main_controller_, SIGNAL(signal_conn_init()),
            this, SLOT(slot_conn_init()));
    connect(async_updater, SIGNAL(signal_check_finish(const bool&)),
            this, SLOT(slot_check_finish(const bool&)));
    connect(option_dialog, SIGNAL(signal_enable_DAChksum(int)),
            this, SLOT(slot_enable_DAChksum(int)));
    connect(this, SIGNAL(signal_language_change(int)),
            this, SLOT(slot_language_changed(int)));

    connect(current_platform, SIGNAL(signal_platform_change()), this,
            SLOT(slot_platformChanged()));

    connect(current_platform, SIGNAL(signal_platform_change()),
            format_widget, SLOT(slot_platformChanged()));

    connect(ui->menuExport_Console_Mode_XML, SIGNAL(aboutToShow()), this,
            SLOT(slot_setMenuItemStatus()));

    connect(this, SIGNAL(signal_start_download()), download_widget,
            SLOT(slot_start_download()));

    connect(this, SIGNAL(signal_set_UARTBaudrateIndex(uint)), option_dialog,
            SLOT(slot_SetUARTBaudrateIndex(uint)));

    connect(thread_timer_, SIGNAL(timeout()), send_report_, SLOT(waitSendThreadTimeout()));
}

void MainWindow::SetInitState()
{
    ui->label_status_port->setText(option_dialog->GetDescription());
    ui->actionPhysical_Format->setVisible(false);
    ui->actionWrite_Memory->setVisible(false);
    ui->actionParameter_Page->setChecked(false);
    ui->actionWrite_Memory->setChecked(false);
    ui->actionExportWriteMemory->setVisible(false);
    ui->actionSecurity_Mode->setVisible(true);
    ui->actionSecurity_Mode->setChecked(false);
    parameter_widget->setVisible(false);
    writeMemory_widget->setVisible(false);
    devtest_widget->setVisible(false);
    bromAdapter_widget->setVisible(false);
    sciDownload_widget_->setVisible(false);
    cloneDownload_wdiget_->setVisible(false);
    download_widget->Enable_groupBox_boot_mode_flag(false);

    bool state;

    IniItem item("option.ini", "WelcomePage", "ShowWelcome");

    state = item.GetBooleanValue();

    if(state == false)
    {
        ui->tabWidget->removeTab(0);
        show_welcome = false;
        ui->actionShow_Welcome->setText(tr("Show Welcome"));
    }
    else
    {
        show_welcome = true;
        ui->actionShow_Welcome->setText(tr("Hide Welcome"));
    }

    item.SetSectionName("RestoreState");
    item.SetItemName("Restore");

    if(item.GetBooleanValue())
        ReadSettings();

    item.SetSectionName("Trace");
    item.SetItemName("EnableTrace");
    mLog_on = item.GetBooleanValue();
    ui->toolBox_status->setVisible(false);
    UpdateWindowTitle(mLog_on);
    main_controller()->ToggleLogging(mLog_on);
}

void MainWindow::LoadScatterFile()
{
    download_widget->LoadLastScatterFile();
}

void MainWindow::LoadDAFile()
{
    download_widget->LoadDefaultDA();
}

void MainWindow::LoadLastAuthFile()
{
    download_widget->LoadLastAuthFile();
}

MainWindow::~MainWindow()
{
    async_updater->waitForFinished();

    if(about_dialog != NULL)
    {
        delete about_dialog;
    }
    while(!tab_widgets.empty())
    {
        TabWidgetBase* tab = tab_widgets.front();
        tab_widgets.pop_front();
        delete tab;
    }

    if(nor_widget_ != NULL)
        delete nor_widget_;

    if(nand_widget_ != NULL)
        delete nand_widget_;

    if(emmc_widget_ != NULL)
        delete emmc_widget_;

    if(sdmmc_widget_ != NULL)
        delete sdmmc_widget_;

    if(ufs_widget_ != NULL)
        delete ufs_widget_;

    if(info_widget_ != NULL)
        delete info_widget_;

    if(thread_timer_ != NULL)
    {
        if(thread_timer_->isActive())
            thread_timer_->stop();
        delete thread_timer_;
    }

    delete processing_dialog_;
    delete option_dialog;
    delete checkupdate_dialog;
    delete update_dialog;
    delete encry_dialog;
    delete async_updater;
    delete current_platform;
    delete scatter_observer_;
    delete assistant_;
    delete ui;
}

QSharedPointer<APCore::ConnSetting> MainWindow::CreateConnSetting()
{
    return option_dialog->CreateConnSetting();
}

QSharedPointer<APCore::DADownloadAllSetting> MainWindow::CreateDownloadSetting()
{
    QSharedPointer<APCore::DADownloadAllSetting> setting(new APCore::DADownloadAllSetting());
    setting->set_cb_da_report(main_callbacks_->GetDAReport);
    setting->set_cb_bl_dl_init(main_callbacks_->DownloadBLoaderInit);
    setting->set_cb_bl_dl_prog(main_callbacks_->DownloadBLoaderProgress);
    setting->set_cb_dl_init(main_callbacks_->DownloadFlashInit);
    setting->set_cb_dl_prog(main_callbacks_->DownloadFlashProgress);
    setting->set_cb_da_chksum_init(main_callbacks_->DADLChksumInit);
    setting->set_cb_da_chksum_progress(main_callbacks_->DADLChksumProgress);
    setting->set_cb_cert_init(main_callbacks_->CERT_Pre_Process_Init);
    setting->set_cb_cert_progress(main_callbacks_->CERT_Post_Process_Progress);

    return setting;
}

QSharedPointer<APCore::FormatSetting> MainWindow::CreateFormatSetting()
{
    QSharedPointer<APCore::FormatSetting> setting(new APCore::FormatSetting());
    setting->set_cb_fmt_init(main_callbacks_->FormatProgressInit);
    setting->set_cb_fmt_progress(main_callbacks_->FormatProgressCallback);

    bool is_phy_fmt = IsPhysicalFmtandReadback();

    bool auto_fmt = format_widget->get_auto_format();
    setting->set_auto_format(auto_fmt);
    setting->set_validation(format_widget->get_validation());
    setting->set_physical_fmt(is_phy_fmt);
    setting->set_erase_flag(format_widget->get_erase_flag());
    setting->set_storage_type(main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType());

    if(!auto_fmt)
    {
        setting->set_begin_addr(format_widget->get_begin_addr());
        setting->set_length(format_widget->get_fmt_length());
        setting->set_part_id(format_widget->get_part_id());
    }
    else
    {
        setting->set_auto_format_flag(format_widget->get_auto_format_flag());
    }

    return setting;
}

QSharedPointer<APCore::ComboFormatSetting> MainWindow::CreateComboFormatSetting(DL_SCATTER_TYPE type)
{
    QSharedPointer<APCore::ComboFormatSetting> setting(new APCore::ComboFormatSetting());

    setting->set_format_setting(this->CreateFormatSetting());
    setting->set_storage_type(main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType());

    if(NORMAL_SCATTER == type)
        setting->set_is_combo_dl(format_widget->get_is_com_dl());
    else if(SCI_SCATTER == type)
        setting->set_is_combo_dl(sciDownload_widget_->IsComboSCI());

    return setting;
}

QSharedPointer<ISetting> MainWindow::CreateDLSetting()
{
    Download_Scene scene = download_widget->scene();

    LOGD("Download scene:(%s).",DownloadScenetEnumToString(scene).c_str());

    if(DOWNLOAD_ONLY == scene)
    {
        return CreateDLOnlySetting();
    }
    else if(WIPE_DATA == scene)
    {
        return CreateSecDLSetting();
    }
    else if(FORMAT_ALL_DOWNLOAD == scene)
    {
        return CreateDLPlugFmtAllSetting();
    }
    else
    {
        return CreateFirmwareUpgradeSetting();
    }
}

QSharedPointer<APCore::DLOnlySetting> MainWindow::CreateDLOnlySetting()
{
    QSharedPointer<APCore::DLOnlySetting> setting(new APCore::DLOnlySetting());

    setting->set_download_setting(this->CreateDownloadSetting());

    return setting;
}

QSharedPointer<APCore::DLPlugFmtAllSetting> MainWindow::CreateDLPlugFmtAllSetting()
{
    QSharedPointer<APCore::DLPlugFmtAllSetting> setting(new APCore::DLPlugFmtAllSetting());

    setting->set_format_setting(this->CreateDLPlugFmtAllFormatSetting());
    setting->set_download_setting(this->CreateDownloadSetting());

    return setting;
}

QSharedPointer<APCore::ComboFormatSetting> MainWindow::CreateDLPlugFmtAllFormatSetting()
{
    QSharedPointer<APCore::ComboFormatSetting> setting(new APCore::ComboFormatSetting());

    setting->set_format_setting(this->CreateFormatSetting());

    setting->set_is_combo_dl(format_widget->get_is_com_dl());

    setting->set_storage_type(main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType());

    if(FORMAT_ALL_DOWNLOAD == download_widget->scene())
    {
        setting->format_setting()->set_physical_fmt(true);
        setting->format_setting()->set_auto_format(true);
        setting->format_setting()->set_auto_format_flag(APCore::FormatSetting::Format_ALL);
    }

    return setting;
}

QSharedPointer<APCore::SecDLSetting> MainWindow::CreateSecDLSetting()
{
    QSharedPointer<APCore::SecDLSetting> setting(new APCore::SecDLSetting());

    setting->set_format_setting(this->CreateFormatSetting());
    setting->set_download_setting(this->CreateDownloadSetting());

    return setting;
}

QSharedPointer<APCore::SCIDownloadSetting> MainWindow::CreateSCIDownloadSetting()
{
    QSharedPointer<APCore::SCIDownloadSetting> setting(new APCore::SCIDownloadSetting());

    setting->set_format_setting(this->CreateComboFormatSetting(SCI_SCATTER));
    setting->set_download_setting(this->CreateDownloadSetting());

    return setting;
}

QSharedPointer<APCore::FirmwareUpgradeSetting> MainWindow::CreateFirmwareUpgradeSetting()
{
    QSharedPointer<APCore::FirmwareUpgradeSetting> setting(new APCore::FirmwareUpgradeSetting());
    setting->set_format_setting(this->CreateFormatSetting());
    setting->set_download_setting(this->CreateDownloadSetting());
    setting->set_readback_setting(this->CreateEmptyReadbackSetting());
    setting->set_writememory_setting(this->CreateWriteMemorySetting());

    Download_Scene scene = download_widget->scene();
    setting->set_download_scene(scene);

    return setting;
}

QSharedPointer<APCore::SCIRBSetting> MainWindow::CreateSCIRBSetting()
{
    QSharedPointer<APCore::SCIRBSetting> setting(new APCore::SCIRBSetting());
    setting->set_rb_setting(this->CreateEmptyReadbackSetting());
    setting->set_rb_folderPath(cloneDownload_wdiget_->GetSCIReadbackFolderPath());

    return setting;
}

QSharedPointer<APCore::SCIRestoreSetting> MainWindow::CreateSCIRestoreSetting()
{
    QSharedPointer<APCore::SCIRestoreSetting> setting(new APCore::SCIRestoreSetting());

    setting->set_wm_setting(this->CreateWriteMemorySetting());
    setting->set_restore_folder_path(cloneDownload_wdiget_->GetSCIRestoreFolderPath());

    return setting;
}

QSharedPointer<APCore::WatchDogSetting> MainWindow::CreateWatchDogSetting()
{
    QSharedPointer<APCore::WatchDogSetting> setting(new APCore::WatchDogSetting());

    return setting;
}

QSharedPointer<APCore::SetBootModeSetting> MainWindow::CreateSetBootModeSetting()
{
    QSharedPointer<APCore::SetBootModeSetting> setting(new APCore::SetBootModeSetting());
    setting->setBootMode(download_widget->GetBootMode());
    setting->setComType(download_widget->GetBootMode_ComType());
    setting->setComId(download_widget->GetBootMode_ComId());

    return setting;
}

QSharedPointer<APCore::CheckRPMBSetting> MainWindow::CreateCheckRPMBSetting()
{
    QSharedPointer<APCore::CheckRPMBSetting> setting(new APCore::CheckRPMBSetting());
    return setting;
}

QSharedPointer<APCore::BromAdapterSetting> MainWindow::CreateBromAdapterSetting(bool only_jump)
{
    QSharedPointer<APCore::BromAdapterSetting> setting(new APCore::BromAdapterSetting());

    setting->set_cb_wbuf_init(main_callbacks_->BromDownload_Progress_Init);
    setting->set_cb_wbuf_progress(main_callbacks_->BromDownload_Progress);

    setting->set_start_addr(bromAdapter_widget->get_start_addr());
    setting->set_jump_addr(bromAdapter_widget->get_jump_addr());
    setting->set_isjump(bromAdapter_widget->get_is_jump());
    setting->set_buf_len(bromAdapter_widget->get_buf_len());
    setting->set_buf((unsigned char*)bromAdapter_widget->get_buf());
    setting->set_is_only_jump(only_jump);
    setting->set_is_manual_addr(bromAdapter_widget->get_is_manual_addr());

    return setting;
}

QSharedPointer<APCore::ReadbackSetting> MainWindow::CreateEmptyReadbackSetting()
{
    QSharedPointer<APCore::ReadbackSetting> setting(new APCore::ReadbackSetting());
    setting->set_cb_readback_init(main_callbacks_->ReadbackInit);
    setting->set_cb_readback_progress(main_callbacks_->ReadbackProgress);

    setting->set_storage_type(main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType());

    return setting;
}

QSharedPointer<APCore::ReadbackSetting> MainWindow::CreateReadbackSetting()
{
    QSharedPointer<APCore::ReadbackSetting> setting(new APCore::ReadbackSetting());
    setting->set_cb_readback_init(main_callbacks_->ReadbackInit);
    setting->set_cb_readback_progress(main_callbacks_->ReadbackProgress);

    setting->set_storage_type(main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType());

    readback_widget->SetReadbackListItem(setting);

    return setting;
}

QSharedPointer<APCore::MemoryTestSetting> MainWindow::CreateMemtestSetting()
{
    QSharedPointer<APCore::MemoryTestSetting> setting =
            memtest_widget->CreateMemtestSetting();
    setting->set_cb_gui(main_callbacks_->MemoryTestUICallback);

    return setting;
}

QSharedPointer<APCore::DeviceTestSetting> MainWindow::CreateDeviceTestSetting()
{
    QSharedPointer<APCore::DeviceTestSetting> setting =
            devtest_widget->CreateDeviceTestSetting();
    setting->set_cb_gui(main_callbacks_->DeviceTestUICallback);

    return setting;
}

QSharedPointer<APCore::CertDLSetting> MainWindow::CreateCertDLSetting()
{
    QSharedPointer<APCore::CertDLSetting> setting(new APCore::CertDLSetting());

    return setting;
}

QSharedPointer<APCore::OTPSetting> MainWindow::CreateOTPSetting()
{
   QSharedPointer<APCore::OTPSetting> setting(new APCore::OTPSetting());
   setting->set_para_process_init(main_callbacks_->updateParaInit);
   setting->set_para_process(main_callbacks_->updatePara);
   setting->set_post_process_init(main_callbacks_->postProcessInit);
   setting->set_post_process(main_callbacks_->postProcess);
   setting->set_otp_operation(parameter_widget->get_operation());

   if(parameter_widget->get_operation() == OTP_READ)
   {
       OTPReadDialog* dlg = parameter_widget->otp_read_dialog();
       setting->set_otp_address(dlg->get_otp_addr());
       setting->set_otp_file((dlg->get_otp_file()).c_str());
       setting->set_otp_length(dlg->get_otp_len());
       setting->set_otp_data(QSharedPointer<U8>((U8*)malloc(dlg->get_otp_len())));
   }
   else
   {
       OTPWriteDialog* dlg = parameter_widget->otp_write_dialog();
       setting->set_otp_address(dlg->get_otp_addr());
       setting->set_otp_file((dlg->get_otp_file()).c_str());
       setting->set_otp_data(dlg->get_otp_data());
       setting->set_otp_length(dlg->get_otp_len());
   }

   return setting;
}

QSharedPointer<APCore::WriteMemorySetting> MainWindow::CreateWriteMemorySetting()
{
   QSharedPointer<APCore::WriteMemorySetting> setting = writeMemory_widget->CreateWriteMemSetting();//(new APCore::WriteMemorySetting());

    setting->set_flash_type(this->main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType());
    setting->set_cb_write_memory_progress(main_callbacks_->WriteMemoryProcess);
    setting->set_cb_write_memory_init(main_callbacks_->WriteMemoryInit);
    return setting;
}

QSharedPointer<APCore::ChksumSetting> MainWindow::CreateChksumSetting()
{
    QSharedPointer<APCore::ChksumSetting> setting(new APCore::ChksumSetting());
    setting->set_cb_checksum_init(main_callbacks_->memChecksumInit);
    setting->set_cb_checksum_progress(main_callbacks_->memChecksumPro);
    setting->set_chksum_level(option_dialog->GetChecksumLevel());

    return setting;
}

QSharedPointer<ConsoleMode::GeneralSetting> MainWindow::CreateGeneralSetting()
{
    QSharedPointer<ConsoleMode::GeneralSetting> gen_setting(
                new ConsoleMode::GeneralSetting());

    download_widget->BuildGeneralSetting(gen_setting);

    gen_setting->vSetConnSetting(CreateConnSetting());

    gen_setting->SetChksumSetting(CreateChksumSetting());

    return gen_setting;
}

void MainWindow::LockOnUI()
{
    EnableMenus(false);
    lockui_on = true;
    for(std::list<TabWidgetBase*>::const_iterator it = tab_widgets.begin();
        it != tab_widgets.end(); ++it)
    {
        if((*it)->IsSelectedTab())
        {
            (*it)->LockOnUI();
        }
        else
        {
            (*it)->SetTabEnabled(false);
        }
    }
}

void MainWindow::DoFinished()
{
    if(is_auto_polling_enable_ &&
            auto_polling_count_ < auto_polling_upper_limit_ - 1 &&
            !main_controller()->isStop() &&
            is_ok_)
    {
        auto_polling_count_++;
        LOGD("Auto polling upper limit(%d), polling count(%d).",
             this->auto_polling_upper_limit_,
             this->auto_polling_count_);
    }
    else
    {
        finish_ = true;

        auto_polling_count_ = 0;

        emit signal_UnlockUI();
    }
}

void MainWindow::startDownload()
{
    emit signal_start_download();
}

void MainWindow::SetUARTBaudrateIndex(unsigned int index)
{
    emit signal_set_UARTBaudrateIndex(index);
}

//slots
void MainWindow::slot_check_finish(const bool &hasUpdate)
{
    if(hasUpdate)
    {
        LaunchUpdateDialog();
    }
    else
    {
        if(NULL != checkupdate_dialog)
        {
            checkupdate_dialog->SetCheckUpdateInfo(LoadQString(language_tag, IDS_STRING_HAS_UPDATE));
            checkupdate_dialog->SetOKButtonVisible(true);
        }
    }
}

void MainWindow::setChipInfo(ChipInfoWidget *widget, const DA_REPORT_T *p_da_report)
{
    if(NULL == widget || NULL == p_da_report)
        return;

   //change da_report.chipname to scatter.platform, for MT6737 special process(otherwise 6737 will show 6735)
   //and for other chip, da_report.chipname == scatter.platform, for at this stage, da_init done, chiptype matched.
   DL_PlatformInfo info;
   memset(&info, 0, sizeof(info));
   main_controller()->GetPlatformInfo(info);
   QString chip_name = info.m_szBBChipType;

   QString chip_hw_ver;
   QString chip_ext_clock = Utils::ExtClockToName(p_da_report->m_ext_clock);
   QString ram_type = Utils::ramType(p_da_report->m_ext_ram_type);
   QString ram_size;
   QString sram_size;


    widget->setGeneralInfo(chip_name,
                           chip_hw_ver.sprintf("0x%08x",p_da_report->m_bbchip_hw_ver),
                           chip_ext_clock,
                           ram_type,
                           ram_size.sprintf("0x%llx",p_da_report->m_ext_ram_size),
                           sram_size.sprintf("0x%08x",p_da_report->m_int_sram_size)
                           );
}

void MainWindow::slot_GetDAReport(const DA_REPORT_T *p_da_report,
                                  const std::string& friend_name,
                                  const int usb_status)
{
    ui->toolBox_status->setVisible(true);

    if(info_widget_ == NULL)
    {
        ui->toolBox_status->removeItem(0);
        info_widget_ = new ChipInfoWidget(this);
        ui->toolBox_status->addItem(info_widget_, QIcon(":/images/format.png"), tr("Chip Info"));
    }

    setChipInfo(info_widget_, p_da_report);

    QString str[8];

    if(p_da_report->m_nor_ret == S_DONE)
    {
        if(nor_widget_ == NULL)
        {
            nor_widget_ = new NorFlashWidget(this);
            ui->toolBox_status->addItem(nor_widget_, QIcon(":/images/writeMemory.png"), tr("Nor Flash"));
        }

        nor_widget_->setNorFlashInfo(str[0].sprintf("0x%08x",p_da_report->m_nor_flash_id),
                                    str[1].sprintf("0x%08x",p_da_report->m_nor_flash_size),
                                    str[2].sprintf("0x%08x",p_da_report->m_nor_flash_otp_size));
    }
    else
    {
        if(NULL != nor_widget_)
        {
            int index = ui->toolBox_status->indexOf(nor_widget_);
            ui->toolBox_status->removeItem(index);

            delete nor_widget_;
            nor_widget_ = NULL;
        }
    }

    if(S_DONE == p_da_report->m_nand_ret)
    {
        if(nand_widget_ == NULL)
        {
            nand_widget_ = new NandWidget(this);
            ui->toolBox_status->addItem(nand_widget_, QIcon(":/images/add.png"), tr("Nand Flash"));
        }

        nand_widget_->setNandInfo(str[0].sprintf("0x%08x",p_da_report->m_nand_flash_id),
                                 str[1].sprintf("0x%llX",p_da_report->m_nand_flash_size),
                                 str[2].sprintf("0x%08x",p_da_report->m_nand_pagesize),
                                 str[3].sprintf("0x%08x",p_da_report->m_nand_sparesize),
                                 str[4].sprintf("0x%08x",p_da_report->m_nand_pages_per_block),
                                 p_da_report->m_nand_bmt_exist ? "true" : "false"
                                 );
    }
    else
    {
        if(NULL != nand_widget_)
        {
            ui->toolBox_status->removeItem(ui->toolBox_status->indexOf(nand_widget_));

            delete nand_widget_;
            nand_widget_ = NULL;
        }
    }

    if(S_DONE == p_da_report->m_emmc_ret)
    {
        if(emmc_widget_ == NULL)
        {
            emmc_widget_ = new EMMCWidget(this);
             ui->toolBox_status->addItem(emmc_widget_, QIcon(":/images/setting.png"), tr("EMMC Flash"));
        }

        emmc_widget_->setEmmcInfo(str[0].sprintf("0x%llx",p_da_report->m_emmc_boot1_size),
                                 str[1].sprintf("0x%llx",p_da_report->m_emmc_boot2_size),
                                 str[2].sprintf("0x%llx",p_da_report->m_emmc_rpmb_size),
                                 str[3].sprintf("0x%llx",p_da_report->m_emmc_gp1_size),
                                 str[4].sprintf("0x%llx",p_da_report->m_emmc_gp2_size),
                                 str[5].sprintf("0x%llx",p_da_report->m_emmc_gp3_size),
                                 str[6].sprintf("0x%llx",p_da_report->m_emmc_gp4_size),
                                 str[7].sprintf("0x%llx",p_da_report->m_emmc_ua_size));
    }
    else
    {
        if(NULL != emmc_widget_)
        {
            ui->toolBox_status->removeItem(ui->toolBox_status->indexOf(emmc_widget_));

            delete emmc_widget_;
            emmc_widget_ = NULL;
        }
    }

    if(S_DONE == p_da_report->m_sdmmc_ret)
    {
        if(sdmmc_widget_ == NULL)
        {
            sdmmc_widget_ = new SDMMCWidget(this);
            ui->toolBox_status->addItem(sdmmc_widget_, QIcon(":/images/update.png"), tr("SD Flash"));
        }

        sdmmc_widget_->setSdmmcInfo(str[0].sprintf("0x%08x",p_da_report->m_sdmmc_cid[0]),
                                str[1].sprintf("0x%llx",p_da_report->m_sdmmc_ua_size));
    }
    else
    {
        if(NULL != sdmmc_widget_)
        {
            ui->toolBox_status->removeItem(ui->toolBox_status->indexOf(sdmmc_widget_));

            delete sdmmc_widget_;
            sdmmc_widget_ = NULL;
        }
    }

    if(S_DONE == p_da_report->m_ufs_ret)
    {
        if(ufs_widget_ == NULL)
        {
            ufs_widget_ = new UfsWidget(this);
            ui->toolBox_status->addItem(ufs_widget_, QIcon(":/images/update.png"), tr("UFS Flash"));
        }

        ufs_widget_->setUfsInfo(str[0].sprintf("0x%016llX",p_da_report->m_ufs_lu0_size),
                                 str[1].sprintf("0x%016llX",p_da_report->m_ufs_lu1_size),
                                 str[2].sprintf("0x%016llX",p_da_report->m_ufs_lu2_size)/*,
                                 str[3].sprintf("0x%016llX",p_da_report->m_ufs_lu3_size)*/);
    }
    else
    {
        if(NULL != ufs_widget_)
        {
            ui->toolBox_status->removeItem(ui->toolBox_status->indexOf(ufs_widget_));

            delete ufs_widget_;
            ufs_widget_ = NULL;
        }
    }

    ui->label_status_port->setText(friend_name.c_str());

    setUSBStatus(usb_status);
}

void MainWindow::setUSBStatus(const int status)
{
    QString usb_status;

    switch(status)
    {
    case USB_FULL_SPEED:
        usb_status = tr("Full Speed");
        break;

    case USB_HIGH_SPEED:
        usb_status = tr("High Speed");
        break;

    case USB_ULTRA_HIGH_SPEED:
        usb_status = tr("Ultra High Speed");
        break;

    case USB_STATUS_UNKNOWN:
    default:
        usb_status = tr("Unknown");
    }

    ui->label_usbStatus->setText(usb_status);
}

void MainWindow::slot_GetBootResult(const BOOT_RESULT *p_boot_result, const std::string& friend_name)
{
    bromAdapter_widget->set_chip_info(p_boot_result);
    ui->label_status_port->setText(friend_name.c_str());
}

void MainWindow::slot_UpdateConnStatus(const std::string &friendly_name)
{
    ui->label_status_port->setText(friendly_name.c_str());
}

void MainWindow::stopTimer()
{
    thread_timer_->stop();
}

void MainWindow::slot_show_err(int err_code, const std::string& err)
{
    if(err_code < 0)
    {
        QMessageBox::critical(this, LoadQString(language_tag, IDS_STRING_TOOL_NAME), err.c_str(),
                                  LoadQString(language_tag, IDS_STRING_OK)
                                  );
    }
    else
    {
        QString content;

        if(err_code >= 8000 && err_code < 9000)
        {
            content = ErrorLookup::FTStatusToString(static_cast<FLASH_TOOL_RESULT>(err_code)).c_str();
        }
        else
        {
            content = StatusToString(err_code);
        }

        content.append(".htm");

        ShowHelpContents(this, err.c_str(), content);
    }

    ResetStatus();
}

void MainWindow::slot_UnlockUI()
{
    EnableMenus(true);
    lockui_on = false;
    for(std::list<TabWidgetBase*>::const_iterator it = tab_widgets.begin();
        it != tab_widgets.end(); ++it)
    {

        (*it)->DoFinished();

        (*it)->SetTabEnabled(true);
    }
}

void MainWindow::slot_show_ok()
{
    if(ok_dialog == NULL)
    {
        ok_dialog = new OkDialog(this);
    }

    ok_dialog->show();
}

void MainWindow::slot_conn_init()
{
    option_dialog->SetPlatformSetting(main_controller_->GetPlatformSetting());
    ui->label_status_port->setText(option_dialog->GetDescription());
}

void MainWindow::slot_enable_DAChksum(int chksum_type)
{
   this->main_controller_->EnableDAChksum(chksum_type);
}

void MainWindow::slot_language_changed(int index)
{
    ui->retranslateUi(this);

    IniItem item("option.ini", "Trace", "EnableTrace");

    UpdateWindowTitle(item.GetBooleanValue());

    item.SetSectionName("Conn");
    item.SetItemName("USBHighSpeed");

    if(item.GetBooleanValue())
        ui->label_usbStatus->setText("High Speed");
    else
        ui->label_usbStatus->setText("Full Speed");

    if(show_welcome == true)
    {
        ui->actionShow_Welcome->setText(tr("Hide Welcome"));
    }
    else
    {
        ui->actionShow_Welcome->setText(tr("Show Welcome"));
    }

    for(std::list<TabWidgetBase*>::const_iterator it = tab_widgets.begin();
        it != tab_widgets.end(); ++it)
    {
        (*it)->UpdateUI();
        (*it)->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(*it));
    }

    writeMemory_widget->UpdateUI();
    writeMemory_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(writeMemory_widget));
    parameter_widget->UpdateUI();
    parameter_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(parameter_widget));
    bromAdapter_widget->UpdateUI();
    bromAdapter_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(bromAdapter_widget));
    cloneDownload_wdiget_->UpdateUI();
    cloneDownload_wdiget_->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(cloneDownload_wdiget_));
    sciDownload_widget_->UpdateUI();
    sciDownload_widget_->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(sciDownload_widget_));

    devtest_widget->UpdateUI();
    devtest_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(devtest_widget));

    option_dialog->UpdateUI();
    option_dialog->SetLabelText(index);

    if(about_dialog != NULL)
        about_dialog->UpdateUI();

    if(NULL != encry_dialog)
        encry_dialog->UpdateUI();

    if(NULL != checkupdate_dialog)
        checkupdate_dialog->UpdateUI();

    if(NULL != update_dialog)
        update_dialog->UpdateUI();

    QSettings settings(ABS_PATH_C("key.ini"), QSettings::IniFormat);
    int size = settings.beginReadArray("Shortcuts");
    for(int i = 0; i < size; i++)
    {
        option_dialog->SetShortcut(i);
    }
    settings.endArray();
}

void MainWindow::slot_platformChanged()
{
   std::string storage_name = main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().
           GetStorageName();

   ui->label_storage->setText(QString(storage_name.c_str()));

   std::string chip_name = main_controller_->GetPlatformSetting()->getLoadPlatformName();

   ui->label_status_port->setText(option_dialog->GetDescription());

   bool show_set_boot_mode = main_controller_->GetPlatformSetting()->is_set_boot_mode_support()
        && main_controller_->advance_on();
   download_widget->Enable_groupBox_boot_mode_flag(show_set_boot_mode);
}

void MainWindow::slot_setMenuItemStatus()
{
    bool isLoad = download_widget->IsScatterLoad();

    ui->actionExportFormat->setEnabled(isLoad);
    ui->actionExportDownload->setEnabled(isLoad);
    ui->actionReadback->setEnabled(isLoad);
    ui->actionExportWriteMemory->setEnabled(isLoad);
    ui->actionExportPartial_Format_Download->setEnabled(isLoad);
}

void MainWindow::EnableMenus(bool enable)
{
    ui->actionUSB_UART_options->setEnabled(enable);
    ui->actionParameter_Page->setEnabled(enable);
    ui->actionWrite_Memory->setEnabled(enable);
    ui->actionPhysical_Format->setEnabled(enable);
    ui->menuExport_Console_Mode_XML->setEnabled(enable);
    ui->actionExit->setEnabled(enable);
    ui->actionSecurity_Mode->setEnabled(enable);
}

void MainWindow::UpdateMenus()
{
    ui->actionPhysical_Format->setVisible(main_controller_->advance_on());

    ui->actionWrite_Memory->setVisible(main_controller_->advance_on());

    ui->actionExportWriteMemory->setVisible(main_controller_->advance_on());
}

void MainWindow::UpdatePlatformImageString(const QString str, const QString owner)
{
    QImage image = Utils::AddStringToImage(":/images/Phone.png", str, owner);

    ui->ImageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::ResetStatus()
{
    ui->progressBar->setValue(0);

    ui->progressBar->setFormat(tr(""));

    ui->label_SpeedInfo->setText(tr("0 B/s"));

    ui->label_total->setText(tr("0 Bytes"));

    ui->label_time->setText("0:00");

    ui->label_status_port->setText(option_dialog->GetDescription());

    this->reset_polling_finish();
}

void MainWindow::InitShortcuts()
{
    mbadblock_shortcut = new QShortcut(this);
    mbadblock_shortcut->setKey(Qt::CTRL+Qt::ALT+Qt::Key_B);
    connect(mbadblock_shortcut, SIGNAL(activated()),this, SLOT(toggleEnableMarkBadBlockMode()));

    madvance_shortcut = new QShortcut(this);
    madvance_shortcut->setKey(Qt::CTRL+Qt::ALT+Qt::Key_V);
    connect(madvance_shortcut, SIGNAL(activated()), this, SLOT(toggleEnableAdvanceMode()));

    mbromAdapter_shortcut = new QShortcut(this);
    mbromAdapter_shortcut->setKey(Qt::CTRL + Qt::ALT + Qt::Key_A);
    connect(mbromAdapter_shortcut, SIGNAL(activated()), this, SLOT(toggleBromAdapterWidget()));

    mSCIDownload_shortcut = new QShortcut(this);
    mSCIDownload_shortcut->setKey(Qt::CTRL + Qt::ALT + Qt::Key_L);
    connect(mSCIDownload_shortcut, SIGNAL(activated()), this,SLOT(toggleSCIDownloadWidget()));

    mlogger_shortcut = new QShortcut(this);
    mlogger_shortcut->setKey(Qt::CTRL + Qt::ALT + Qt::Key_M);
    connect(mlogger_shortcut,SIGNAL(activated()), this, SLOT(toggleLogging()));

    mDevTest_shortcut = new QShortcut(this);
    mDevTest_shortcut->setKey(Qt::CTRL + Qt::ALT + Qt::Key_D);
    connect(mDevTest_shortcut, SIGNAL(activated()), this,SLOT(toggleDeviceTestWidget()));

}

void MainWindow::UpdateWindowTitle(bool enableTrace)
{
    QString title = LoadQString(language_tag, IDS_STRING_TOOL_NAME);

    if(enableTrace)
   {
        title += LoadQString(language_tag, IDS_STRING_TRACE_MODE);
    }

    if(main_controller()->advance_on())
    {
        title += LoadQString(language_tag, IDS_STRING_ADVANCE_MODE);
    }

    if(main_controller()->markblocks_on())
    {
        title += LoadQString(language_tag, IDS_STRING_MARKBBM);
    }

    if(msecurity_on)
    {
        title += LoadQString(language_tag, IDS_STRING_SECURITYMODE);
    }

    this->setWindowTitle(title);
}

bool MainWindow::isPlatformValid()
{
    return download_widget->InitialPlatform();
}

void MainWindow::toggleLogging()
{
    if(lockui_on)
        return;

    mLog_on = !mLog_on;

    main_controller()->ToggleLogging(mLog_on);

    UpdateWindowTitle(mLog_on);
}

//TODO(h):all the Mode Toggle??clear up
void MainWindow::toggleEnableMarkBadBlockMode()
{
    main_controller()->EnableMarkBadBlocksMode();

    HW_StorageType_E storage_type = main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType();

    format_widget->ShowEraseFlag(main_controller()->markblocks_on() &&
                                 storage_type == HW_STORAGE_NAND);

    format_widget->ShowMarkAsBadBlockFlag(main_controller()->markblocks_on() &&
                                          storage_type == HW_STORAGE_NAND);

    IniItem item("option.ini", "Trace", "EnableTrace");

    UpdateWindowTitle(item.GetBooleanValue());
}

void MainWindow::toggleEnableAdvanceMode()
{
    if(lockui_on)
        return;

    main_controller()->EnableAdvanceMode();

    UpdateMenus();

    HW_StorageType_E storage_type = main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType();

    format_widget->ShowEraseFlag(main_controller()->advance_on() &&
                                 storage_type == HW_STORAGE_NAND);

    option_dialog->ShowPhysicalFormatItem(main_controller()->advance_on() &&
                                          storage_type == HW_STORAGE_NAND);

    if(main_controller()->advance_on() == false)
    {
        ui->actionWrite_Memory->setChecked(false);

        RemoveWriteMemoryPage();
    }

    std::string platform_name = main_controller_->GetPlatformSetting()->getLoadPlatformName();
    if(FlashTool_Chip_isOldArch(platform_name.c_str()))
    {
        memtest_widget->ShowEMMCTest(main_controller()->advance_on() &&
                                     storage_type == HW_STORAGE_EMMC);

        memtest_widget->ShowNANDTest(main_controller()->advance_on() &&
                                     storage_type == HW_STORAGE_NAND);
    }

    /*
    memtest_widget->ShowUFSTest(main_controller()->advance_on() &&
                                 storage_type == HW_STORAGE_UFS);
    */

    //set boot mode flag
    //parse platform xml, <support set boot mode flag>
    bool show_set_boot_mode = main_controller_->GetPlatformSetting()->is_set_boot_mode_support()
         && main_controller_->advance_on();
    download_widget->Enable_groupBox_boot_mode_flag(show_set_boot_mode);
    //end of set boot mode flag

    //check rpmb option
    bool show_check_rpmb = main_controller()->GetPlatformSetting()->is_check_rpmb_support()
         && main_controller_->advance_on();
    option_dialog->ShowCheckRPMBItem(show_check_rpmb);
    //end of check rpmb option

    IniItem item("option.ini", "Trace", "EnableTrace");

    UpdateWindowTitle(item.GetBooleanValue());
}

void MainWindow::toggleBromAdapterWidget()
{
    if(lockui_on)
        return;

    mbromAdapter_on = !mbromAdapter_on;

    if(mbromAdapter_on)
    {
        tab_widgets.push_back(bromAdapter_widget);
        bromAdapter_widget->Attach();
        ui->tabWidget->setCurrentWidget(bromAdapter_widget);
        bromAdapter_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(bromAdapter_widget));
    }
    else
    {
        tab_widgets.remove(bromAdapter_widget);
        bromAdapter_widget->Detach();
    }
}

void MainWindow::toggleSCIDownloadWidget()
{
    if(lockui_on)
        return;

    mSCIDownload_on = !mSCIDownload_on;

    if(mSCIDownload_on)
    {
        if(NULL == encry_dialog)
        {
            encry_dialog = new EncryDialog(this, this);
        }

        encry_dialog->show();

        if(QDialog::Accepted == encry_dialog->exec())
        {
            if(is_scidl_visible_)
            {
                tab_widgets.push_back(cloneDownload_wdiget_);
                cloneDownload_wdiget_->Attach();
                cloneDownload_wdiget_->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(cloneDownload_wdiget_));


                tab_widgets.push_back(sciDownload_widget_);
                sciDownload_widget_->Attach();
                sciDownload_widget_->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(sciDownload_widget_));

                ui->tabWidget->setCurrentWidget(sciDownload_widget_);
            }
        }
        else
            mSCIDownload_on = false;
    }
    else
    {
        tab_widgets.remove(cloneDownload_wdiget_);
        cloneDownload_wdiget_->Detach();

        tab_widgets.remove(sciDownload_widget_);
        sciDownload_widget_->Detach();
    }
}

void MainWindow::toggleDeviceTestWidget()
{
    if(lockui_on)
        return;

    mDeviceTest_on = !mDeviceTest_on;

    if(mDeviceTest_on)
    {
        tab_widgets.push_back(devtest_widget);
        devtest_widget->Attach();
        devtest_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(devtest_widget));

        ui->tabWidget->setCurrentWidget(devtest_widget);
    }
    else
    {
        tab_widgets.remove(devtest_widget);
        devtest_widget->Detach();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_actionExportFormat_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(
                this,"Save File", "format.xml", "*.xml");
    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        ConsoleMode::Config config;
        config.vSetGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting>(this->CreateGeneralSetting()->pclClone()));
        config.vAddCommandSetting(this->CreateComboFormatSetting());
        config.SaveFile(file_name.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionExportDownload_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(
                this,"Save File", "download.xml", "*.xml");
    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        ConsoleMode::Config config;
        config.vSetGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting>(this->CreateGeneralSetting()->pclClone()));
        config.vAddCommandSetting(this->CreateDLSetting());
        config.SaveFile(file_name.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionCerfitication_download_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(
                this, "Save File", "certdl.xml", "*.xml");

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        ConsoleMode::Config config;
        config.vSetGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting>(this->CreateGeneralSetting()->pclClone()));
        config.vAddCommandSetting(this->CreateCertDLSetting());
        config.SaveFile(file_name.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionExportPartial_Format_Download_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(
                this,"Save File", "format_download.xml", "*.xml");
    bool is_phy_fmt = IsPhysicalFmtandReadback();

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        ConsoleMode::Config config;
        config.vSetGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting>(this->CreateGeneralSetting()->pclClone()));

        //FIXME! hardcode for auto partitial format
        QSharedPointer<APCore::FormatSetting> fmt_setting(new APCore::FormatSetting());
        fmt_setting->set_auto_format(true);
        fmt_setting->set_auto_format_flag(APCore::FormatSetting::Format_Except_BL);
        fmt_setting->set_erase_flag(NUTL_ERASE);
        fmt_setting->set_physical_fmt(is_phy_fmt);
        fmt_setting->set_storage_type(HW_STORAGE_NAND);
        fmt_setting->set_validation(false);
        config.vAddCommandSetting(fmt_setting);

        config.vAddCommandSetting(this->CreateDownloadSetting());
        config.SaveFile(file_name.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionReadback_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(
                this,"Save File", "readback.xml", "*.xml");
    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        ConsoleMode::Config config;
        config.vSetGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting>(this->CreateGeneralSetting()->pclClone()));
        config.vAddCommandSetting(this->CreateReadbackSetting());
        config.SaveFile(file_name.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionExportWriteMemory_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(
                this,"Save File", "write_memory.xml", "*.xml");

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        ConsoleMode::Config config;
        config.vSetGeneralSetting(QSharedPointer<ConsoleMode::GeneralSetting>(this->CreateGeneralSetting()->pclClone()));
        config.vAddCommandSetting(this->CreateWriteMemorySetting());
        config.SaveFile(file_name.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionUSB_UART_options_triggered()
{
    option_dialog->exec();

    ui->label_status_port->setText(option_dialog->GetDescription());

    ui->label_usbStatus->setText(option_dialog->GetUSBSpeed());

    return;
}

void MainWindow::on_actionPhysical_Format_triggered()
{
    ui->actionPhysical_Format->setChecked(ui->actionPhysical_Format->isChecked());
}

void MainWindow::on_actionParameter_Page_triggered()
{
    if(ui->actionParameter_Page->isChecked())
    {
        tab_widgets.push_back(parameter_widget);
        parameter_widget->Attach();
        ui->tabWidget->setCurrentWidget(parameter_widget);
        parameter_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(parameter_widget));
    }
    else
    {
        tab_widgets.pop_front();
        parameter_widget->Detach();
    }
}

void MainWindow::on_actionWrite_Memory_triggered()
{
    if(ui->actionWrite_Memory->isChecked())
    {
        tab_widgets.push_back(writeMemory_widget);
        writeMemory_widget->Attach();
        ui->tabWidget->setCurrentWidget(writeMemory_widget);
        writeMemory_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(writeMemory_widget));
    }
    else
    {
        tab_widgets.pop_front();
        writeMemory_widget->Detach();
    }
}

void MainWindow::on_actionSecurity_Mode_triggered()
{
    if(lockui_on)
        return;

    msecurity_on = ui->actionSecurity_Mode->isChecked();
    ui->tabWidget->setCurrentWidget(download_widget);
    download_widget->ShowUnavailableItems(msecurity_on);
    ui->actionCerfitication_download->setVisible(msecurity_on);

    UpdateWindowTitle(mLog_on);
}

void MainWindow::RemoveWriteMemoryPage()
{
    std::list<TabWidgetBase*>::iterator it = std::find(tab_widgets.begin(),
                                                       tab_widgets.end(), writeMemory_widget);

    if(it != tab_widgets.end())
    {
        tab_widgets.remove(writeMemory_widget);
       // tab_widgets.pop_front();
        writeMemory_widget->Detach();
    }
}

void MainWindow::UpdateSatusBar()
{
    ui->label_status_port->setText(option_dialog->GetDescription());
}

void MainWindow::UpdateReadbackList(bool show)
{
    readback_widget->ShowReadFlagColumn(show);
}

void MainWindow::on_actionShow_Welcome_triggered()
{
    if(show_welcome)
    {
        show_welcome = false;
        ui->actionShow_Welcome->setText(LoadQString(language_tag, IDS_STRING_SHOW_WELCOME));

        welcome_widget->Detach();
    }
    else
    {
        show_welcome = true;
        ui->actionShow_Welcome->setText(LoadQString(language_tag, IDS_STRING_HIDE_WELCOME));

        welcome_widget->Attach();
        ui->tabWidget->setCurrentWidget(welcome_widget);
        welcome_widget->SetTabLabel(ui->tabWidget, ui->tabWidget->indexOf(welcome_widget));
    }
}

void MainWindow::ShowHelpContents(QWidget *parent, const QString &errMsg, const QString &page)
{
    int ret = -1;

#ifdef _WIN32
    if(!ToolInfo::IsCustomerVer())
        ret =  QMessageBox::warning(parent, LoadQString(language_tag, IDS_STRING_TOOL_NAME), errMsg,
                                    LoadQString(language_tag, IDS_STRING_OK),
                                    LoadQString(language_tag, IDS_STRING_HELP),
                                    LoadQString(language_tag, IDS_STRING_SEND_REPORT));
    else
        ret =  QMessageBox::warning(parent, LoadQString(language_tag, IDS_STRING_TOOL_NAME), errMsg,
                                    LoadQString(language_tag, IDS_STRING_OK),
                                    LoadQString(language_tag, IDS_STRING_HELP));
#else
    ret =  QMessageBox::warning(parent, LoadQString(language_tag, IDS_STRING_TOOL_NAME), errMsg,
                                LoadQString(language_tag, IDS_STRING_OK),
                                LoadQString(language_tag, IDS_STRING_HELP));
#endif

    if(ret == IDC_BUTTON_HELP)
    {
        assistant_->ShowDocumentation(page);
    }
    else if(ret == IDC_BUTTON_SENDREPORT)
    {
        QString extPath = "C:\\Python33\\python.exe";

        QProcess process;

        QStringList parameter;

        std::string temp = "dirlist.py";

        parameter << temp.c_str();

        process.startDetached(extPath, parameter);
        //send_report_->setChipID(6582);
        //thread_timer_->start(5000);
        //send_report_->sendErrorReport();
    }
}

void MainWindow::ShowHelpContents(QWidget* parent, int errID, const QString &page)
{
    QString errMsg = LoadQString(language_tag, errID);

    return ShowHelpContents(parent, errMsg, page);
}

void MainWindow::on_actionIndex_triggered()
{
   assistant_->ShowDocumentation("welcome.htm", false, true);
}

void MainWindow::on_actionContents_triggered()
{
    assistant_->ShowDocumentation("welcome.htm", true, false);
}

void MainWindow::on_actionAbout_triggered()
{
    if(about_dialog == NULL)
    {
        about_dialog = new AboutDialog(this);
    }
    about_dialog->UpdateUI();
    about_dialog->show();
}

void MainWindow::on_action_SoftwareUpdate_triggered()
{
    if(NULL == checkupdate_dialog)
    {
        checkupdate_dialog = new CheckUpdateDialog(this, async_updater);

        connect(checkupdate_dialog, SIGNAL(signal_check_finish(const bool&)),
                this, SLOT(slot_check_finish(const bool&)));
    }

    checkupdate_dialog->show();
}

void MainWindow::on_actionOpen_Logs_Folder_triggered()
{
    Logger::SetSPFlashToolLogFolder();

    QString path = QString::fromLocal8Bit(Logger::GetSPFlashToolLogFolder().c_str());

#ifdef _WIN32
    QProcess::startDetached("explorer "+path);
#else
    QProcess::startDetached("nautilus "+path);
#endif
}

void MainWindow::LaunchUpdateDialog()
{
    if(NULL != checkupdate_dialog)
        checkupdate_dialog->close();

    if(NULL == update_dialog)
    {
        update_dialog = new UpdateDialog(this, async_updater);
    }

    update_dialog->show();
}

bool MainWindow::ValidateBeforeFormat()
{
    return download_widget->ValidateBeforeFormat();
}

bool MainWindow::IsScatterFileLoad()
{
    return download_widget->IsScatterFileLoad();
}

bool MainWindow::IsDALoad()
{
    if(!main_controller()->IsDALoaded())
    {
        ShowHelpContents(this, IDS_STRING_WARNING_LOADDA, "E_APP_LOAD_DA.htm");

        return false;
    }
    return true;
}

bool MainWindow::IsPreloaderLoad()
{
    bool is_preloader_loaded = false;
    std::list<ImageInfo> images;
    main_controller()->GetImageInfoList(images);
    for(std::list<ImageInfo>::const_iterator it=images.begin(); it!=images.end(); ++it)
    {
        if(stricmp(it->name.c_str(), "PRELOADER") == 0)
        {
            is_preloader_loaded = it->enabled;
            break;
        }
    }
    if(!is_preloader_loaded)
    {
        ShowHelpContents(this, IDS_STRING_WARNING_LOADPRELOADER,
                         "E_APP_LOAD_PRELOADER.htm");
        return false;
    }

    return true;
}

bool MainWindow::IsPhysicalFmtandReadback()
{
    IniItem item("option.ini", "Download", "PhyFmt");

    bool is_phy_fmt = item.GetBooleanValue();

    is_phy_fmt = is_phy_fmt &&
            main_controller_->GetPlatformSetting()->getFlashToolStorageConfig().IsNandOperation() &&
            main_controller_->advance_on();

    return is_phy_fmt;
}

bool MainWindow::CheckRPMBEnable()
{
    if(option_dialog->isRPMBChecked())
    {
        IniItem item("option.ini", "Download", "CheckRPMB");
        bool check_rpmb_enable = item.GetBooleanValue();

        check_rpmb_enable = check_rpmb_enable
                && main_controller_->GetPlatformSetting()->is_check_rpmb_support()
                && main_controller_->advance_on();

        return check_rpmb_enable;
    }
    else
        return false;
}

bool MainWindow::ValidateBeforeMemoryTest()
{
    if(!IsDALoad() || !IsPreloaderLoad())
        return false;

    if(memtest_widget->isSelectNothing())
    {
        ShowHelpContents(this, IDS_STRING_MEMORY_TEST_SELECT_NOTHING,
                         "");
        return false;
    }

    return true;
}

bool MainWindow::ValidateBeforeWriteMemory()
{
    return IsDALoad() && IsPreloaderLoad();
}

void MainWindow::ReadSettings()
{
    QSettings setting("MTK", "Clipper");

    setting.beginGroup("MainWindow");
    resize(setting.value("size", QSize(400, 400)).toSize());
    move(setting.value("pos", QPoint(200, 200)).toPoint());
    setting.endGroup();
}

void MainWindow::WriteSettings()
{
    QSettings setting("MTK", "Clipper");

    setting.beginGroup("MainWindow");
    setting.setValue("size", size());
    setting.setValue("pos", pos());

    setting.endGroup();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    const QString info = LoadQString(language_tag, IDS_STRING_WARNING_EXIT);
    const QString title = LoadQString(language_tag, IDS_STRING_EXIT);
    if(lockui_on)
    {
        int promptbutton;
        promptbutton = QMessageBox::question(this, title,
                                             info,
                                             LoadQString(language_tag, IDS_STRING_YES),
                                             LoadQString(language_tag, IDS_STRING_NO));
        if(promptbutton == 0)
        {
            //wait for real stop;
            main_controller()->StopByTool();
        }
        else if(promptbutton == 1)
        {
            e->ignore();
            return;
        }
    }


    if(!lockui_on){
        LOGD("UI has unlocked!");
        WriteSettings();
        e->accept();
    }
}

void MainWindow::ChangeLanguage(int index)
{
    QTranslator qt_translator;
    QTranslator app_translator;
    bool result;
    switch(index)
    {
    case 0:
        SetLanguageTag(LANGUAGE_ENGLISH);
        qApp->removeTranslator(&app_translator);

        break;

    case 1:
        SetLanguageTag(LANGUAGE_ZH_CHINESE);
        result = qt_translator.load("qt_zh_CN.qm", ":/translations");
        if(!result)
        {
            signal_err_msg(FT_LOAD_TRANSLATION_FILE_FAIL, "load qt_zh_CN.qm fail");
            return;
        }
        qApp->installTranslator(&qt_translator);
        result = app_translator.load("Flashtool_zh_CN.qm", ":/translations");
        if(!result)
        {
            signal_err_msg(FT_LOAD_TRANSLATION_FILE_FAIL, "load Flashtool_zh_CN.qm fail");
            return;
        }
        qApp->installTranslator(&app_translator);

        break;

    case 2:
        SetLanguageTag(LANGUAGE_TW_CHINESE);
        result = qt_translator.load("qt_zh_TW.qm", ":/translations");
        if(!result)
        {
            signal_err_msg(FT_LOAD_TRANSLATION_FILE_FAIL, "load qt_zh_TW.qm fail");
            return;
        }
        qApp->installTranslator(&qt_translator);
        result = app_translator.load("Flashtool_zh_TW.qm", ":/translations");
        if(!result)
        {
            signal_err_msg(FT_LOAD_TRANSLATION_FILE_FAIL, "load Flashtool_zh_TW.qm fail");
            return;
        }
        qApp->installTranslator(&app_translator);

        break;
    }

    emit signal_language_change(index);
}

void MainWindow::SetShortCut(Command_Category cmd_category, int cmd, int row)
{
    QString shortCut = option_dialog->GetKeyboardModify(row);

    switch(cmd_category)
    {
    case OPTIONS:
        ui->actionUSB_UART_options->setShortcut(shortCut);
        break;

    case WINDOW:
        ui->actionParameter_Page->setShortcut(shortCut);
        break;

    case HELP:
        SetHelpShortcut(cmd, shortCut);
        break;

    case FORMAT:
        format_widget->SetShortCut(cmd, shortCut);
        break;

    case DOWNLOAD:
        download_widget->SetShortCut(cmd, shortCut);
        break;

    case READBACK:
        readback_widget->SetShortCut(cmd, shortCut);
        break;

    case MEMORYTEST:
        memtest_widget->SetShortCut(cmd, shortCut);
        break;

    case WRITEMEMORY:
        writeMemory_widget->SetShortCut(cmd, shortCut);
        break;

    case PARAMETER:
        parameter_widget->SetShortCut(cmd, shortCut);
        break;
    case BAT:
        bromAdapter_widget->SetShortCut(cmd, shortCut);
        break;
    }
}

void MainWindow::SetHelpShortcut(int cmd, const QString &shortCut)
{
    switch(cmd)
    {
    case CMD_CONTENTS:
        ui->actionContents->setShortcut(shortCut);
        break;

    case CMD_INDEX:
        ui->actionIndex->setShortcut(shortCut);
        break;
    }
}

void MainWindow::SetPlatfromForBat()
{
    option_dialog->SetPlatformSetting(main_controller_->GetPlatformSetting());
    ui->label_status_port->setText(option_dialog->GetDescription());
}
