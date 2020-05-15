#include "DownloadWidget.h"
#include "ui_DownloadWidget.h"
#include "MainWindow.h"
#include "CheckHeader.h"
#include "ICallback.h"
#include "ProcessingDialog.h"
#include "../../Utility/Utils.h"
#include "../../Utility/FileUtils.h"
#include "../../Utility/constString.h"
#include "../../Utility/IniItem.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "../../Utility/version.h"

#include <QtDebug>
#include <QtGui>
#include <QTextCodec>

DownloadWidget::DownloadWidget(QTabWidget *parent, MainWindow *window) :
    TabWidgetBase(2, tr("&Download"), parent),
    main_window_(window),
    ui_(new Ui::DownloadWidget),
    scatterFile_historyList_(),
    authFile_historyList_(),
    scene_(DOWNLOAD_ONLY),
    header_(new CheckHeader(Qt::Horizontal, this)),
    storage_(HW_STORAGE_EMMC),
    bUpdateScene(false)
{
    ui_->setupUi(this);

    setAcceptDrops(true);

    ui_->tableWidget->setHorizontalHeader(header_);

    QObject::connect(header_,SIGNAL(sectionClicked(int)),
                     this, SLOT(slot_OnHeaderView_click(int)));

    connect(this, SIGNAL(signal_load_finished()), SLOT(slot_OnLoadByScatterEnd()));
    connect(this, SIGNAL(signal_load_failed()), SLOT(slot_OnLoadByScatterFailed()));
    connect(this, SIGNAL(signal_load_canceled()), SLOT(slot_OnLoadByScatterCanceled()));

    connect(this, SIGNAL(signal_load_rom_done()),SLOT(slot_OnLoadRomDone()));
    connect(this, SIGNAL(signal_load_rom_failed()), SLOT(slot_OnLoadRomFailed()));
    connect (main_window_->processing_dialog(), SIGNAL(user_cancel_processing()),this, SLOT(slot_OnUserCancelLoadScatter()));

    ui_->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    main_window_->main_controller()->GetPlatformSetting()->addObserver(this);
    main_window_->scatter_observer()->addObserver(this);

    QStringList sceneList;

    sceneList << LoadQString(LANGUAGE_TAG, IDS_STRING_SCENE_FMTALLDL)
              << LoadQString(LANGUAGE_TAG, IDS_STRING_SCENE_FIRMWAREUPGRADE)
              << LoadQString(LANGUAGE_TAG, IDS_STRING_SCENE_DOWNLOADONLY)
              << LoadQString(LANGUAGE_TAG, IDS_STRING_SCENE_WIPE_DATA);

    ui_->comboBox_Scene->addItems(sceneList);

    ui_->comboBox_Scene->setCurrentIndex(DOWNLOAD_ONLY);

    //LoadDefaultDA();

    ShowUnavailableItems(false);

    UpdateImageList(true, scene_);

    header_->SetChecked(true);

    ui_->tableWidget->setColumnHidden(columnRegion, true);
}

DownloadWidget::~DownloadWidget()
{
    IniItem item("history.ini", "RecentOpenFile", "scatterHistory");

    item.SaveStringListValue(scatterFile_historyList_);

    item.SetItemName("authHistory");
    item.SaveStringListValue(authFile_historyList_);

    UpdateCustomSceneSelectItems();

    delete header_;

    delete ui_;
}

void DownloadWidget::on_pushButton_downloadAgent_clicked()
{
    IniItem item("history.ini", "LastDAFilePath", "lastDir");

    QString last_dir = item.GetStringValue();

    if(last_dir.isEmpty())
        last_dir = FileUtils::GetAppDirectory().c_str();

    QString file_name = QFileDialog::getOpenFileName(
                this,
                LoadQString(LANGUAGE_TAG, IDS_STRING_OPEN_DA),
                last_dir,
                LoadQString(LANGUAGE_TAG, IDS_STRING_BIN));

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        item.SaveStringValue(file_name);

        default_da = file_name;

        //load DA
        if(main_window_->main_controller()->LoadDA(file_name))
        {
            ui_->lineEdit_agentFilePath->setText(file_name);
        }
        else
        {
            ui_->lineEdit_agentFilePath->setText("");
        }
    }
}

void DownloadWidget::on_pushButton_scatterLoading_clicked()
{
    IniItem item("history.ini", "RecentOpenFile", "lastDir");

    QString last_dir = item.GetStringValue();

    QString file_name = QFileDialog::getOpenFileName(
                this,
                LoadQString(LANGUAGE_TAG, IDS_STRING_OPEN_SCATTER),
                last_dir,
                LoadQString(LANGUAGE_TAG, IDS_STRING_MAP_FILE));


    if(!file_name.isEmpty())
    {
        item.SaveStringValue(QDir::toNativeSeparators(file_name));
        UpdateScatterFile(file_name);
    }
}

void DownloadWidget::LoadAuthFile(const QString& filename)
{
    QString file_name = filename;
    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);
        LOG("file_name: %s", file_name.toLocal8Bit().constData());

        if(main_window_->main_controller()->LoadAuthFile(file_name))
        {
            int index = authFile_historyList_.indexOf(file_name);
            //always set select item to index 0
            //1. new, insert at 0
            //2. old, not at 0, move to 0
            //3. old, at 0, do nothing
            if(index < 0)
            {
                authFile_historyList_.push_front(file_name);
            }
            else if(index != 0)
            {
                authFile_historyList_.move(index, 0);
            }

            ui_->comboBox_authFilePath->clear();
            ui_->comboBox_authFilePath->insertItems(0, authFile_historyList_);
            ui_->comboBox_authFilePath->addItem("");
            ui_->comboBox_authFilePath->setCurrentIndex(0);
        }
        else
        {
            authFile_historyList_.removeFirst();
            ui_->comboBox_authFilePath->clear();
            ui_->comboBox_authFilePath->insertItems(0, authFile_historyList_);
            ui_->comboBox_authFilePath->addItem("");
            ui_->comboBox_authFilePath->setCurrentIndex(-1);
        }
    }

    LOG("The authFile history list size is %d.\n", authFile_historyList_.size());
}

void DownloadWidget::LoadLastAuthFile()
{
    IniItem item("history.ini", "RecentOpenFile", "authHistory");
    authFile_historyList_ = item.GetStringListValue();
    if(!authFile_historyList_.isEmpty())
    {
        QString lastAuth = authFile_historyList_[0];
        LoadAuthFile(lastAuth);
    }
}

void DownloadWidget::on_pushButton_authFile_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(
                this,
                LoadQString(LANGUAGE_TAG, IDS_STRING_OPEN_AUTHFILE),
                "",
                LoadQString(LANGUAGE_TAG, IDS_STRING_AUTH_BIN));

    LoadAuthFile(file_name);
}

void DownloadWidget::on_pushButton_CertFile_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(
                this,
                LoadQString(LANGUAGE_TAG, IDS_STRING_OPEN_SECFILE),
                "",
                LoadQString(LANGUAGE_TAG, IDS_STRING_SEC_BIN));

    if(!file_name.isEmpty())
    {
        file_name = QDir::toNativeSeparators(file_name);

        if(main_window_->main_controller()->LoadCertFile(file_name))
        {
            ui_->lineEdit_certFilePath->setText(file_name);
        }
        else
        {
            ui_->lineEdit_certFilePath->setText("");
        }
    }
}

void DownloadWidget::slot_start_download()
{
    StartDownload();
}

void DownloadWidget::StartDownload()
{
    main_window_->main_controller()->SetPlatformSetting();
    main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());

    //check rpmb written before download
    bool enable_check_rpmb = main_window_->CheckRPMBEnable();
    if(enable_check_rpmb)
        main_window_->main_controller()->QueueAJob(main_window_->CreateCheckRPMBSetting());
    //end of check rpmb

    main_window_->main_controller()->QueueAJob(main_window_->CreateDLSetting());

    bool enable_set_boot_mode = main_window_->main_controller()->GetPlatformSetting()->is_set_boot_mode_support()
         && main_window_->main_controller()->advance_on()
         && ui_->checkbox_set_boot_mode_to_meta->checkState() == Qt::Checked;
    if(enable_set_boot_mode)
        main_window_->main_controller()->QueueAJob(main_window_->CreateSetBootModeSetting());

    if(!ToolInfo::IsCustomerVer())
        main_window_->main_controller()->QueueAJob(main_window_->CreateWatchDogSetting());

    main_window_->main_controller()->StartExecuting(
                    new SimpleCallback<MainWindow>(main_window_,&MainWindow::DoFinished));
}

void DownloadWidget::on_pushButton_download_clicked()
{
    IniItem item("option.ini", "Download", "RiskReminder");
    bool isRiskReminderOn = item.GetBooleanValue();
    if(isRiskReminderOn)
    {
        int ristRet = QMessageBox::warning(this, LoadQString(LANGUAGE_TAG, IDS_STRING_WARNING),
                                       LoadQString(LANGUAGE_TAG, IDS_STRING_DLRISK_REMINDER),
                                       LoadQString(LANGUAGE_TAG, IDS_STRING_CONTINUE),
                                       LoadQString(LANGUAGE_TAG, IDS_STRING_CANCEL));
        if(ristRet == 1) //cancel
            return;
    }

    if(ValidateBeforeDownload())
    {
        StartDownload();
        main_window_->LockOnUI();
        main_window_->GetOkDialog()->setWindowTitle(LoadQString(LANGUAGE_TAG, IDS_STRING_DOWNLOAD_OK));
    }
}

void DownloadWidget::on_toolButton_Certification_clicked()
{
    if(ValidateBeforeCertDL())
    {
        main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
        main_window_->main_controller()->QueueAJob(main_window_->CreateCertDLSetting());
        main_window_->main_controller()->StartExecuting(
                    new SimpleCallback<MainWindow>(main_window_, &MainWindow::DoFinished));
        main_window_->LockOnUI();
        main_window_->GetOkDialog()->setWindowTitle(LoadQString(LANGUAGE_TAG, IDS_STRING_CERT_OK));
    }
}

void DownloadWidget::on_pushButton_stop_clicked()
{
    ui_->pushButton_stop->setEnabled(false);
    main_window_->main_controller()->StopByUser();
}

void DownloadWidget::slot_OnHeaderView_click(int index)
{
    if(index == 0)
    {
        bool checked = header_->GetChecked();

        if(!checked)
        {
            UpdateImageList(false, scene_);
        }
        else
        {
                UpdateImageList(true, scene_);
        }

        UpdateScene();
    }
}

void DownloadWidget::on_tableWidget_cellClicked(int row, int column)
{
    if (column == ColumnLocation)
    {
        choose_rom_file(row, ColumnLocation);
    }
}

void DownloadWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        QPoint point = event->pos();

        point = ui_->tableWidget->mapFromParent(event->pos());

        QRect rect = ui_->tableWidget->geometry();

        LOG("The  table widget rect x1 = %d, y1 = %d, x2 = %d, y2 = %d\n",
            rect.x(), rect.y(), rect.width(), rect.height());

        if(rect.contains(point))
        {
            event->acceptProposedAction();
        }
    }
}

void DownloadWidget::dropEvent(QDropEvent *event)
{
    if(!ui_->tableWidget->underMouse())
    {
        return;
    }

    QList<QUrl> urls = event->mimeData()->urls();

    if(urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
        return;

    UpdateScatterFile(fileName);
}

void DownloadWidget::OnLoadByScatterEnd()
{
    emit signal_load_finished();//to update UI accrossing threads.
}

void DownloadWidget::OnLoadByScatterFailed()
{
    emit signal_load_failed();
}

void DownloadWidget::OnLoadByScatterCanceled()
{
    emit signal_load_canceled();
}

void DownloadWidget::OnLoadRomDone()
{
    emit signal_load_rom_done();//to update UI accrossing threads.
}

void DownloadWidget::OnLoadRomFailed()
{
    emit signal_load_rom_failed();
}

void DownloadWidget::UpdateUIStatus()
{
    QString strPath = ui_->comboBox_scatterFilePath->currentText();
    scatterFile_historyList_.removeOne(strPath);
    ui_->comboBox_scatterFilePath->removeItem(ui_->comboBox_scatterFilePath->currentIndex());
    ui_->comboBox_scatterFilePath->setEditText("");
    ui_->comboBox_scatterFilePath->setCurrentIndex(-1);
    main_window_->ResetStatus();

    IniItem item("history.ini", "RecentOpenFile", "lastDir");
    item.SaveStringValue(tr(""));

    SetDACheckSum();
}

unsigned char DownloadWidget::GetBootMode_ComType()
{
    if(ui_->com_type_usb->isChecked())
        return 2;
    else if(ui_->com_type_uart->isChecked())
        return 1;
    else if(ui_->com_type_wifi->isChecked())
        return 3;
    else
        return 0;
}

unsigned char DownloadWidget::GetBootMode_ComId()
{
    int id = 0;
    if(!ui_->checkbox_mobile_log_on->isChecked())
        id |= 2;

    if(!ui_->checkbox_adb_on->isChecked())
        id |= 1;

    return id;
}

unsigned char DownloadWidget::GetBootMode()
{
    return (unsigned char)ui_->checkbox_set_boot_mode_to_meta->isChecked();
}

void DownloadWidget::EnableBootMode_ComType(bool enable)
{
    ui_->groupBox_com_type->setEnabled(enable);
}

void DownloadWidget::EnableBootMode_ComId(bool enable)
{
    ui_->groupBox_com_id->setEnabled(enable);
}

void DownloadWidget::Enable_groupBox_boot_mode_flag(bool enable)
{
    ui_->groupBox_boot_mode_flag->setEnabled(enable);
    ui_->groupBox_boot_mode_flag->setVisible(enable);
}

void DownloadWidget::slot_OnLoadByScatterEnd()
{
    main_window_->processing_dialog()->hide();

    if(InitialPlatform())
    {
        //show rom image infos...
        std::list<ImageInfo> image_list;
        main_window_->main_controller()->GetImageInfoList(image_list, scene_);
        main_window_->ResetStatus();
        Platform platform = main_window_->main_controller()->GetPlatformSetting()
                ->getPlatformConfig();

        main_window_->setAutoPollingUpperLimit(platform.autoPollingUpperLimit());
        main_window_->setIsAutoPollingEnable(platform.isAutoPollingEnable());

        UpdateImageList(image_list);

        SetDACheckSum();

        //start set baudrate=921600 for MTxxxx, fix me in future
        if(platform.getSimpleName().compare("MTxxxx")==0)
            main_window_->SetUARTBaudrateIndex(0);
        else
            main_window_->SetUARTBaudrateIndex(3);
    }
    else
    {
        UpdateUIStatus();
    }
}

void DownloadWidget::slot_OnLoadByScatterCanceled()
{
    main_window_->processing_dialog()->hide();
    ui_->comboBox_scatterFilePath->setEditText("");
    ui_->comboBox_scatterFilePath->setCurrentIndex(-1);
    main_window_->ResetStatus();
}

void DownloadWidget::slot_OnLoadByScatterFailed()
{
    main_window_->processing_dialog()->hide();

    UpdateUIStatus();
}

void DownloadWidget::slot_OnLoadRomDone()
{
    slot_OnLoadByScatterEnd();
}

void DownloadWidget::slot_OnLoadRomFailed()
{
    main_window_->processing_dialog()->hide();

    QMessageBox::critical(this,  LoadQString(LANGUAGE_TAG, IDS_STRING_TOOL_NAME),
                          LoadQString(LANGUAGE_TAG, IDS_STRING_ROMFILE_WARNING),
                          LoadQString(LANGUAGE_TAG, IDS_STRING_YES));

    int row = ui_->tableWidget->currentRow();

    QTableWidgetItem *tableItem = ui_->tableWidget->item(row, ColumnLocation);
    if (tableItem != NULL) {
        tableItem->setText(tr(""));
    }
}

void DownloadWidget::LoadLastScatterFile()
{
    IniItem item("history.ini", "RecentOpenFile", "scatterHistory");

    scatterFile_historyList_ = item.GetStringListValue();

    item.SetItemName("lastDir");

    QString last_dir = item.GetStringValue();

    LOG("Init read the history list size is %d.\n", scatterFile_historyList_.size());

    if(!scatterFile_historyList_.empty())
    {
        int index = 0;

        if(last_dir.length() > 0)
        {
            index = scatterFile_historyList_.indexOf(last_dir);

            if(index < 0)
            {
                scatterFile_historyList_.insert(0, QDir::toNativeSeparators(last_dir));
                index = 0;
            }
        }
        QString path = QDir::toNativeSeparators((scatterFile_historyList_.at(index)));

        QFileInfo info(path);

        if(info.exists())
            LoadScatterFile(path);
        else
        {
            main_window_->slot_show_err(STATUS_SCATTER_FILE_NOT_FOUND, "The scatter file cannot find, please make sure the file is exist before download.");

            scatterFile_historyList_.removeAt(index);
            ui_->comboBox_scatterFilePath->clear();
            ui_->comboBox_scatterFilePath->insertItems(0, scatterFile_historyList_);
            ui_->comboBox_scatterFilePath->setCurrentIndex(-1);
            ui_->comboBox_scatterFilePath->setEditText("");

            item.SetItemName("lastDir");
            item.SaveStringValue("");

            index = ui_->comboBox_scatterFilePath->findText(path);
            if(index >= 0)
                ui_->comboBox_scatterFilePath->removeItem(index);

            return;
        }

        if(main_window_->main_controller()->GetPlatformSetting()->is_scatter_file_valid())
        {
            ui_->comboBox_scatterFilePath->clear();

            ui_->comboBox_scatterFilePath->insertItems(0, scatterFile_historyList_);

            ui_->comboBox_scatterFilePath->setCurrentIndex(index);

            ui_->comboBox_scatterFilePath->setEditText(path);
        }
    }
}

void DownloadWidget::LoadScatterFile(const QString &file_name)
{
    QString name = QDir::toNativeSeparators(file_name);

    main_window_->processing_dialog()->showCancelButton(true);
    main_window_->processing_dialog()->show();

    mStopFlag = 0;
    main_window_->main_controller()->SetChksumSetting(
                main_window_->CreateChksumSetting());
    main_window_->main_controller()->SetStopLoadFlag(&mStopFlag);
    main_window_->main_controller()->LoadScatterAsync(
                name,
                new SimpleCallback<DownloadWidget>(this,&DownloadWidget::OnLoadByScatterEnd),
                new SimpleCallback<DownloadWidget>(this,&DownloadWidget::OnLoadByScatterFailed),
                new SimpleCallback<DownloadWidget>(this,&DownloadWidget::OnLoadByScatterCanceled));

}

void DownloadWidget::slot_OnUserCancelLoadScatter()
{
    mStopFlag = AP_STOP;
}

bool DownloadWidget::InitialPlatform()
{
    DL_PlatformInfo info;
    std::string error_hint;
    bool is_validate(false);

    memset(&info, 0, sizeof(info));

    //initial platform setting
    main_window_->main_controller()->GetPlatformInfo(info);

    is_validate = main_window_->main_controller()->GetPlatformSetting()
             ->initByNewScatterFile(info, error_hint);

    if(!is_validate)
    {
        main_window_->ShowHelpContents(this, QString(QString::fromLocal8Bit(error_hint.c_str())),
                                       "E_APP_SCATTER_FAILED.htm");

         return false;
    }

    return true;
}

void DownloadWidget::ShowUnavailableItems(bool show)
{
    ui_->label_certFile->setVisible(show);
    ui_->lineEdit_certFilePath->setVisible(show);
    ui_->pushButton_CertFile->setVisible(show);
    ui_->toolButton_Certification->setVisible(show);

    if(show)
        ui_->comboBox_Scene->addItem(LoadQString(LANGUAGE_TAG, IDS_STRING_SCENE_WIPE_DATA));
    else
        ui_->comboBox_Scene->removeItem(ui_->comboBox_Scene->count() - 1);
}

void DownloadWidget::LoadDefaultDA()
{
    IniItem item("history.ini", "LastDAFilePath", "lastDir");

    default_da = item.GetStringValue();

    if(default_da.isEmpty())
        default_da = Utils::GetTextCodec()->toUnicode(ABS_PATH_C("MTK_AllInOne_DA.bin"));

    if(FileUtils::IsFileExist(default_da.toLocal8Bit().data()))
    {
        //load DA
        if(main_window_->main_controller()->LoadDA(default_da))
        {
            default_da = QDir::toNativeSeparators(default_da);
            ui_->lineEdit_agentFilePath->setText(
                        default_da);
        }
        else
        {
            ui_->lineEdit_agentFilePath->setText("");
        }
    }
    else
    {
        LOGW("Load default DA(%s) failed.", default_da.toLocal8Bit().data());
    }

}

int DownloadWidget::FindIndex(const QString &filePath)
{
    for(int i = 0; i < ui_->comboBox_scatterFilePath->count(); i++)
    {
        QString text = ui_->comboBox_scatterFilePath->itemText(i);

        if(filePath.compare(text) == 0)
            return i;
    }

    return -1;
}

void DownloadWidget::UpdateImageList(std::list<ImageInfo> &image_list)
{
    int row_count = 0;
    for(std::list<ImageInfo>::const_iterator it = image_list.begin();
        it != image_list.end(); ++it)
    {
        if(it->visible)
        {
            row_count++;
        }
    }
    ui_->tableWidget->setRowCount(row_count);

    SCATTER_Head_Info info;
    main_window_->main_controller()->GetScatterHeadInfo(&info);

    bool showRegion = false;

    if(stricmp(info.version, "v1.1.1") > 0 && (stricmp(info.storage, "EMMC") == 0 || stricmp(info.storage, "UFS") == 0))
        showRegion = true;

    string strOwnerName = "";
    if(!ToolInfo::IsCustomerVer()) //not show owner name in customer version tool
    {
        Platform platform = main_window_->main_controller()->GetPlatformSetting()->getPlatformConfig();
        string sOwnerName = platform.getOwnerName();
        strOwnerName = sOwnerName=="" ? "" : ("Owner: "+sOwnerName);
    }
    QString str = QString((char*)(strOwnerName.c_str()));
    main_window_->UpdatePlatformImageString(info.platform, str);
    main_window_->scatter_observer()->notifyObservers(showRegion);
    main_window_->UpdateReadbackList((strcmp(info.storage, "EMMC") == 0 || strcmp(info.storage, "UFS") == 0) ? false : true);
/** onPlatformChanged End**/

    QTableWidgetItem * tableItem;
    int row = 0;
    bool has_uncheck_item = false;

    for(std::list<ImageInfo>::const_iterator it = image_list.begin();
        it != image_list.end(); ++it)
    {
        if(!it->visible)
        {
            //skip invisible items
            continue;
        }

        SetRomAddress(row, ColumnBeginAddr, it->begin_addr);
        SetRomAddress(row, ColumnEndAddr, it->end_addr);

        tableItem = ui_->tableWidget->item(row, columnRegion);
        if(tableItem == NULL){
            tableItem = new QTableWidgetItem();
            ui_->tableWidget->setItem(row, columnRegion, tableItem);
        }

        tableItem->setText(EnumToRomString(storage_, it->region));

        tableItem = ui_->tableWidget->item(row, ColumnLocation);
        if (tableItem == NULL) {
            tableItem = new QTableWidgetItem();
            ui_->tableWidget->setItem(row, ColumnLocation,tableItem);
        }
        tableItem->setText(QDir::toNativeSeparators(QString::fromLocal8Bit(it->location.c_str())));

        tableItem = ui_->tableWidget->item(row, ColumnName);
        if (tableItem == NULL) {
            tableItem = new QTableWidgetItem();
            ui_->tableWidget->setItem(row, ColumnName, tableItem);
        }
        tableItem->setText(it->name.c_str());

        tableItem = ui_->tableWidget->item(row, ColumnEnable);
        if (tableItem == NULL) {
            bUpdateScene = false;
            tableItem = new QTableWidgetItem();
            ui_->tableWidget->setItem(row, ColumnEnable, tableItem);
        }

        int item_index = main_window_->main_controller()->QueryROMIndex(tableItem->row());

        main_window_->main_controller()->EnableROM(item_index, it->enabled);

        bUpdateScene = false;
        if (it->enabled && !ui_->tableWidget->item(row, ColumnLocation)->text().isEmpty())
        {
            tableItem->setCheckState(Qt::Checked);
            tableItem->setFlags(tableItem->flags()| Qt::ItemIsUserCheckable);
            tableItem->setToolTip("");
        }
        else
        {
            tableItem->setCheckState(Qt::Unchecked);
            has_uncheck_item = true;
            if(ui_->tableWidget->item(row, ColumnLocation)->text().isEmpty())
            {
                tableItem->setFlags(tableItem->flags()&~Qt::ItemIsUserCheckable);
                tableItem->setToolTip("Please select valid rom file first");
            }
        }

        row++;
    }

    if(!has_uncheck_item)
        header_->SetChecked(true);
    else
        header_->SetChecked(false);

    bUpdateScene = true;
}

void DownloadWidget::UpdateImageList()
{
    IniItem item("option.ini", "CustomScene", "CheckedItems");

    QStringList items = item.GetStringListValue();

     if(items.size() == 0)
     {
         UpdateImageList(true, DOWNLOAD_ONLY);
         header_->SetChecked(true);
     }
     else
     {
         bool ok;
         for(int i = 0; i < items.size(); i++)
         {
             int index = items.at(i).toInt(&ok);

             if(ok && index >= 0)
             {
                 QTableWidgetItem *item = ui_->tableWidget->item(index, 0);
                 if(NULL != item)
                 {
                     item->setCheckState(Qt::Checked);
                     int item_index = main_window_->main_controller()->QueryROMIndex(item->row());

                     main_window_->main_controller()->EnableROM(item_index, true);
                 }
             }
         }
     }
}

void DownloadWidget::UpdateImageList(bool checked, Download_Scene scene)
{
    bool CheckAll = checked;

    for(int row = 0; row < ui_->tableWidget->rowCount(); row++)
    {
        QTableWidgetItem *item = ui_->tableWidget->item(row, 0);

        if( scene == WIPE_DATA && CheckAll != false)
        {
            QString rom_name = ui_->tableWidget->item(row, 1)->text();

            checked = AppCore::IsEnableRom(string((const char*)rom_name.toLocal8Bit()), scene);
        }

        bUpdateScene = false;

        checked = CheckAll && !ui_->tableWidget->item(row, ColumnLocation)->text().isEmpty();

        checked ? item->setCheckState(Qt::Checked) : item->setCheckState(Qt::Unchecked);

        int item_index = main_window_->main_controller()->QueryROMIndex(item->row());

        main_window_->main_controller()->EnableROM(item_index, checked);
    }
}

void DownloadWidget::UpdateScatterFile(const QString &file_name)
{
    LoadScatterFile(file_name);

    if(main_window_->main_controller()->GetPlatformSetting()->is_scatter_file_valid())
    {
        int  index = FindIndex(QDir::toNativeSeparators(file_name));

        if(index < 0)
        {
            scatterFile_historyList_.insert(0, QDir::toNativeSeparators(file_name));
            LOG("The history list size is %d.\n", scatterFile_historyList_.size());

            ui_->comboBox_scatterFilePath->insertItem(0, QDir::toNativeSeparators(file_name));

            ui_->comboBox_scatterFilePath->setCurrentIndex(0);
        }
        else
        {
            ui_->comboBox_scatterFilePath->setCurrentIndex(index);
        }

        ui_->comboBox_scatterFilePath->setEditText(QDir::toNativeSeparators(file_name));
    }
}

void DownloadWidget::SetRomAddress(int row, int column, U64 address)
{
    QTableWidgetItem *tableItem = ui_->tableWidget->item(row, column);
    if (tableItem == NULL) {
        tableItem = new QTableWidgetItem();
        ui_->tableWidget->setItem(row, column,tableItem);
    }
    tableItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    tableItem->setText(QString("0x%1").arg(address,16,16,QChar('0')));
}

bool DownloadWidget::hasUncheckedRomInfo()
{
    for(int row = 0; row < ui_->tableWidget->rowCount(); row++)
    {
        QTableWidgetItem *item = ui_->tableWidget->item(row, 0);

        if(item == NULL)
            return false;

        if(item->checkState() == Qt::Unchecked)
            return true;
    }

    return false;
}

void DownloadWidget::UpdateScene()
{
    if(scene_ != DOWNLOAD_ONLY)
    {
        ui_->comboBox_Scene->setCurrentIndex(DOWNLOAD_ONLY);
        scene_ = DOWNLOAD_ONLY;
    }
}

void DownloadWidget::UpdateCustomSceneSelectItems()
{
    QStringList items;

    for(int i = 0; i < ui_->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem *item = ui_->tableWidget->item(i, 0);

        if(item && item->checkState() == Qt::Checked)
        {
            items.append(QString::number(i));
        }
    }

    IniItem ini_item("option.ini", "CustomScene", "CheckedItems");

    ini_item.SaveStringListValue(items);
}

void DownloadWidget::UpdateHeadviewCheckState()
{
    if(scene_ == FORMAT_ALL_DOWNLOAD || scene_ == FIRMWARE_UPGRADE)
    {
        header_->SetChecked(true);
    }
    else if(scene_ == DOWNLOAD_ONLY)
    {
        if(hasUncheckedRomInfo())
            header_->SetChecked(false);
        else
            header_->SetChecked(true);
    }
    else
    {
        header_->SetChecked(false);
    }
}

void DownloadWidget::LockOnUI()
{
    //TODO
    ui_->toolButton_Certification->setEnabled(false);
    ui_->pushButton_download->setEnabled(false);
    ui_->pushButton_stop->setEnabled(true);
    ui_->FileLoadFrame->setEnabled(false);
    ui_->tableWidget->setEnabled(false);
}

void DownloadWidget::DoFinished()
{
    ui_->toolButton_Certification->setEnabled(true);
    ui_->pushButton_download->setEnabled(true);
    ui_->pushButton_stop->setEnabled(false);
    ui_->FileLoadFrame->setEnabled(true);
    ui_->tableWidget->setEnabled(true);
}

void DownloadWidget::UpdateUI()
{
    ui_->retranslateUi(this);

    if(FileUtils::IsFileExist(default_da.toLocal8Bit().data()))
            ui_->lineEdit_agentFilePath->setText(
                         QDir::toNativeSeparators(default_da));
}

void DownloadWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_DOWNLOAD);

    tab_widget->setTabText(index, label);

    for(int i = 0; i < ui_->comboBox_Scene->count(); i++)
    {
        ui_->comboBox_Scene->setItemText(i, LoadQString(LANGUAGE_TAG, IDS_STRING_SCENE_FMTALLDL + i));
    }
}

void DownloadWidget::SetShortCut(int cmd, const QString &shortcut)
{
    switch(cmd)
    {
    case DL_BEGIN:
        ui_->pushButton_download->setShortcut(shortcut);
        ui_->pushButton_download->setToolTip(shortcut);

        break;

    case DL_STOP:
        ui_->pushButton_stop->setShortcut(shortcut);
        ui_->pushButton_stop->setToolTip(shortcut);
        break;
    case DL_CERT:
        ui_->toolButton_Certification->setShortcut(shortcut);
        ui_->toolButton_Certification->setToolTip(shortcut);
    }
}

void DownloadWidget::SetDACheckSum()
{
     IniItem item("option.ini", "Download", "DAChksum");

     int chksum = item.GetIntValue();
     if(chksum >= DHNDL_FLAG_CHKSUM_LEVEL_NONE && chksum <= DHNDL_FLAG_CHKSUM_LEVEL_ALL)
         main_window_->slot_enable_DAChksum(chksum);
     else
         main_window_->slot_enable_DAChksum(DHNDL_FLAG_CHKSUM_LEVEL_NONE);
}

void DownloadWidget::BuildGeneralSetting(
    QSharedPointer<ConsoleMode::GeneralSetting> &gen_setting)
{
    QSharedPointer<ConsoleMode::GeneralArg> arg = QSharedPointer<ConsoleMode::GeneralArg>( new ConsoleMode::GeneralArg());
    QString scatter_path = ui_->comboBox_scatterFilePath->currentText();

    std::string platform = main_window_->main_controller()->GetPlatformSetting()->getLoadPlatformName();
    arg->chip_name=platform;

    HW_StorageType_E storage_type = main_window_->main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType();
    arg->storage_type=storage_type;

    arg->da_file=ui_->lineEdit_agentFilePath->text().toUtf8().constData();
    arg->scatter_file=scatter_path.toUtf8().constData();

    arg->auth_file=ui_->comboBox_authFilePath->currentText().toUtf8().constData();
    arg->cert_file=ui_->lineEdit_certFilePath->text().toUtf8().constData();

    std::list<ConsoleMode::RomItem> items;

    std::list<ImageInfo> image_list;
    main_window_->main_controller()->GetImageInfoList(image_list);

    for(std::list<ImageInfo>::const_iterator it = image_list.begin();
        it != image_list.end(); ++it)
    {
        if(it->visible)
        {
            ConsoleMode::RomItem rom;
            rom.index  = it->index;
            rom.enable = it->enabled;
            rom.path   = it->location;
            items.push_back(rom);
        }
    }

    arg->rom_list = items;
    gen_setting->vSetGeneralArg(arg);
}

bool DownloadWidget::IsScatterLoad()
{
    if(ui_->comboBox_scatterFilePath->currentText().isEmpty())
        return false;

    return true;
}

bool DownloadWidget::ValidateBeforeFormat()
{
    if(ui_->lineEdit_agentFilePath->text().isEmpty())
    {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_LOADDA,
                                       "E_APP_LOAD_DA.htm");

        return false;
    }

    return IsScatterFileLoad();
}

bool DownloadWidget::IsScatterFileLoad()
{
    if(ui_->comboBox_scatterFilePath->currentText().isEmpty())
    {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_LOADSCATTER,
                                       "E_APP_LOAD_SCATTER.htm");

        return false;
    }
    return true;
}

bool DownloadWidget::ValidateBeforeCertDL()
{
    if(ui_->lineEdit_certFilePath->text().isEmpty())
    {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_LOADCERT,
                                       "E_APP_LOAD_AUTH.htm");

        return false;
    }

    return true;
}

bool DownloadWidget::ValidateBeforeDownload()
{
   if(ui_->lineEdit_agentFilePath->text().isEmpty())
   {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_LOADDA, "E_APP_LOAD_DA.htm");

        return false;
    }

    if(ui_->comboBox_scatterFilePath->currentText().isEmpty())
    {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_LOADSCATTER, "E_APP_LOAD_SCATTER.htm");

        return false;
    }

    bool has_rom = false;
    std::list<ImageInfo> images;
    main_window_->main_controller()->GetImageInfoList(images);
    for(std::list<ImageInfo>::const_iterator it = images.begin();
        it != images.end(); ++it)
    {
        if(it->enabled)
        {
            has_rom = true;
            break;
        }
    }

    if(!has_rom)
    {
        main_window_->ShowHelpContents(this, IDS_STRING_WARNING_ROMNOTSELECT,
                                       "E_APP_NONROM_SELECT.htm");

        return false;
    }

    if(scene_ == WIPE_DATA || scene_ == FORMAT_ALL_DOWNLOAD || scene_ == DOWNLOAD_ONLY)
    {
        int index = GetSecRoIndex();

        if(index < 0 && scene_ == WIPE_DATA)
            return false;

        index = main_window_->main_controller()->QueryROMIndex(index);

        int result = main_window_->main_controller()->CheckSecUnlockSecro(index);

        if(result == 0)   // unlock sec-ro image
        {
            if(scene_ == FORMAT_ALL_DOWNLOAD || scene_ == DOWNLOAD_ONLY)
        {
            main_window_->ShowHelpContents(this, IDS_STRING_NOTALLOWED_UNLOCK,
                                           "E_APP_SEC_UNLOCK.htm");

            return false;
        }
        else
        {
            int prompt = QMessageBox::question(this, LoadQString(LANGUAGE_TAG, IDS_STRING_TOOL_NAME),
                                 LoadQString(LANGUAGE_TAG, IDS_STRING_SECURITY_WARNING),
                                 LoadQString(LANGUAGE_TAG, IDS_STRING_YES),
                                 LoadQString(LANGUAGE_TAG, IDS_STRING_NO));

            if(prompt == 1)
                return false;
        }
    }
        else if(result != 0 && scene_ == WIPE_DATA)
        {
            main_window_->ShowHelpContents(this, IDS_STRING_SECURITY_ERROR,
                                           "E_APP_SEC_IMAGE.htm");

            return false;
        }       
    }

    return true;
}

int DownloadWidget::GetSecRoIndex()
{
    QSettings settings(ABS_PATH_C("download_scene.ini"), QSettings::IniFormat);
    settings.beginGroup("Download");
    QStringList wipeList = settings.value("WIPE_DATA").toStringList();
    settings.endGroup();

    for(int i = 0; i < ui_->tableWidget->rowCount(); i++)
    {
        if(ui_->tableWidget->item(i, ColumnEnable)->checkState() == Qt::Checked &&
               wipeList.contains(ui_->tableWidget->item(i, ColumnName)->text()))
            return i;
    }

    return -1;
}

void DownloadWidget::choose_rom_file(int row, int column)
{
    if(column == ColumnLocation)
    {
        QString file = ui_->tableWidget->item(row,ColumnLocation)->text();
        QString new_file = QFileDialog::getOpenFileName(this, "Open File",
                                            file, "All File (*.*)");
        if(!new_file.isEmpty())
        {
            new_file = QDir::toNativeSeparators(new_file);
            //loading this single rom
            main_window_->processing_dialog()->showCancelButton(false);
            main_window_->processing_dialog()->show();

            int item_index = main_window_->main_controller()->QueryROMIndex(row);
            QTableWidgetItem *tableItem = ui_->tableWidget->item(row, ColumnEnable);
            tableItem->setFlags(tableItem->flags()|Qt::ItemIsUserCheckable);
            main_window_->main_controller()->LoadROMAsync(new_file,item_index,
                new SimpleCallback<DownloadWidget>(this,&DownloadWidget::OnLoadRomDone),
                new SimpleCallback<DownloadWidget>(this,&DownloadWidget::OnLoadRomFailed));
        }
    }
}

void DownloadWidget::on_comboBox_Scene_activated(int index)
{
    switch(index)
    {
    case 0:
        scene_ = FORMAT_ALL_DOWNLOAD;
        break;

    case 1:
        scene_ = FIRMWARE_UPGRADE;
        break;

    case 2:
        scene_ = DOWNLOAD_ONLY;
        break;

    case 3:
        scene_ = WIPE_DATA;
    }

    bUpdateScene = false;

    UpdateRomInfoList(scene_);

    bUpdateScene = true;
}

void DownloadWidget::UpdateRomInfoList(Download_Scene scene)
{
    UpdateImageList(false, scene);

    if(scene != DOWNLOAD_ONLY)
        UpdateImageList(true, scene);
    else
        UpdateImageList();

    if(scene == FORMAT_ALL_DOWNLOAD || scene == FIRMWARE_UPGRADE)
    {
        header_->SetChecked(true);
    }
    else if(scene == WIPE_DATA)
    {
        header_->SetChecked(false);
    }
}

void DownloadWidget::on_comboBox_scatterFilePath_activated(const QString &arg1)
{
    QString file_name = arg1;

    QFile file(file_name);

    if(file_name.isEmpty() || file.exists() == false)
        return;

    LoadScatterFile(file_name);

    IniItem item("history.ini", "RecentOpenFile", "lastDir");

    item.SaveStringValue(QDir::toNativeSeparators(file_name));
}

void DownloadWidget::onPlatformChanged()
{
    storage_ = main_window_->main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType();
}

void DownloadWidget::OnScatterChanged(bool showRegion)
{
      ui_->tableWidget->setColumnHidden(columnRegion, !showRegion);
}

void DownloadWidget::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    QTableWidgetItem *tableItem = ui_->tableWidget->item(item->row(), ColumnEnable);
    if(tableItem == NULL)
        return;

    if(!bUpdateScene)
    {
        bUpdateScene = true;
        return;
    }

    LOGD("row %d item changed.\n", item->row());

    int item_index = main_window_->main_controller()->QueryROMIndex(item->row());

    if(item->checkState() == Qt::Checked && !ui_->tableWidget->item(item->row(), ColumnLocation)->text().isEmpty())
    {
        main_window_->main_controller()->EnableROM(item_index, true);
    }
    else
    {
        main_window_->main_controller()->EnableROM(item_index, false);
    }

    UpdateScene();

    if(scene_ == DOWNLOAD_ONLY)
        UpdateCustomSceneSelectItems();

    UpdateHeadviewCheckState();
}


void DownloadWidget::on_checkbox_set_boot_mode_to_meta_clicked()
{
    if(ui_->checkbox_set_boot_mode_to_meta->isChecked())
    {
        EnableBootMode_ComType(true);
        EnableBootMode_ComId(true);
    }
    else
    {
        EnableBootMode_ComType(false);
        EnableBootMode_ComId(false);
    }
}

void DownloadWidget::on_comboBox_authFilePath_currentIndexChanged(int index)
{
    if(index > 0 && index <authFile_historyList_.size())
    {
        authFile_historyList_.move(index, 0);

        ui_->comboBox_authFilePath->clear();
        ui_->comboBox_authFilePath->insertItems(0, authFile_historyList_);
        ui_->comboBox_authFilePath->addItem("");
        ui_->comboBox_authFilePath->setCurrentIndex(0);
    }
}
