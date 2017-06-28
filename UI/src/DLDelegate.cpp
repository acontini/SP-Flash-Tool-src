#include "DLDelegate.h"
#include "MainController.h"
#include "ProcessingDialog.h"
#include "./Utility/version.h"

DLDelegate::DLDelegate(DL_SCATTER_TYPE scatterType,
                       TabWidgetBase * widget,
                       MainWindow *window,
                       QObject *parent) :
    QObject(parent),
    scatterType_(scatterType),
    widget_(widget),
    main_window_(window),
    has_uncheck_item_(false),
    table_widget_(NULL)
{
}

void DLDelegate::LoadScatterFile(const QString &file_path,
                                 ICallback *callback,
                                 ICallback *failed_callback)
{
    main_window_->processing_dialog()->showCancelButton(false);
    main_window_->processing_dialog()->show();

    if(scatterType_ == SCI_SCATTER)
    {
        main_window_->main_controller()->LoadSCIScatterAsync(
                    file_path,
                    callback,
                    failed_callback
                    );
    }
}

void DLDelegate::onPlatformChanged()
{
    storage_ = main_window_->main_controller()->GetPlatformSetting()->getFlashToolStorageConfig().GetStorageType();

    widget_->UpdateUI();
}

void DLDelegate::updateTableWidgetList(QTableWidget*widget,
                                       std::list<ImageInfo> &imageList)
{
    int row_count = 0;

    for(std::list<ImageInfo>::const_iterator it = imageList.begin();
        it != imageList.end(); ++it)
    {
        if(it->visible)
            row_count++;
    }

    widget->setRowCount(row_count);

    QTableWidgetItem *item;
    int row = 0;

    for(std::list<ImageInfo>::const_iterator it = imageList.begin();
        it != imageList.end(); ++it)
    {
        if(!it->visible)
            continue;

        item = widget->item(row, ColumnBeginAddr);

        if(NULL == item)
        {
            item = new QTableWidgetItem();
            widget->setItem(row, ColumnBeginAddr, item);
        }

        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

        if(storage_ == HW_STORAGE_NAND)
        {
            item->setText(QString("0x%1").arg(it->begin_addr,8,16,QChar('0')));
        }
        else
        {
            item->setText(QString("0x%1").arg(it->begin_addr,16,16,QChar('0')));
        }

        item = widget->item(row, ColumnName);
        if (item == NULL) {
            item = new QTableWidgetItem();
            widget->setItem(row, ColumnName, item);
        }
        item->setText(it->name.c_str());

        item = widget->item(row, ColumnEnable);
        if (item == NULL) {
            item = new QTableWidgetItem();
            widget->setItem(row, ColumnEnable, item);
        }

        if(scatterType_ == SCI_SCATTER)
        {
            main_window_->main_controller()->EnableROM(it->name, it->enabled);
        }
        else if(scatterType_ == NORMAL_SCATTER)
        {
            int item_index = main_window_->main_controller()->QueryROMIndex(item->row());

            main_window_->main_controller()->EnableROM(item_index, it->enabled);
        }

        if (it->enabled)
        {
            item->setCheckState(Qt::Checked);
        }
        else
        {
            item->setCheckState(Qt::Unchecked);
            has_uncheck_item_ = true;
        }

        row++;
    }
}

bool DLDelegate::onLoadEnd()
{
    main_window_->processing_dialog()->hide();

    if(InitialPlatform())
    {
        std::list<ImageInfo> image_list;
        main_window_->main_controller()->GetImageInfoList(image_list);
        main_window_->ResetStatus();
        updateTableWidgetList(table_widget_, image_list);

        return true;
    }

    return false;
}

void DLDelegate::onLoadFailed()
{
    main_window_->processing_dialog()->hide();

    main_window_->ResetStatus();
}

bool DLDelegate::InitialPlatform()
{
    DL_PlatformInfo info;
    std::string error_hint;
    bool is_validate(false);

    memset(&info, 0, sizeof(info));

    main_window_->main_controller()->GetPlatformInfo(info);

    is_validate = main_window_->main_controller()->GetPlatformSetting()->initByNewScatterFile(info, error_hint);

    if(!is_validate)
    {
         return false;
    }

    return true;
}

bool DLDelegate::hasEnableRom()
{
    std::list<ImageInfo> images;
    main_window_->main_controller()->GetImageInfoList(images);
    for(std::list<ImageInfo>::const_iterator it = images.begin();
        it != images.end(); ++it)
    {
        if(it->enabled)
        {
            return true;
        }
    }

    return false;
}

void DLDelegate::startSCIDownload()
{
    main_window_->main_controller()->SetPlatformSetting();
    main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
    main_window_->main_controller()->QueueAJob(main_window_->CreateSCIDownloadSetting());

    if(!ToolInfo::IsCustomerVer())
        main_window_->main_controller()->QueueAJob(main_window_->CreateWatchDogSetting());

    main_window_->main_controller()->StartExecuting(
                    new SimpleCallback<MainWindow>(main_window_,&MainWindow::DoFinished));
}

void DLDelegate::stopExecute()
{
    main_window_->main_controller()->StopByUser();
}

