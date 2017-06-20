#include "SCIDownloadWidget.h"
#include "ui_SCIDownloadWidget.h"
#include "MainWindow.h"
#include "MainController.h"
#include "CheckHeader.h"
#include "ICallback.h"
#include "ProcessingDialog.h"
#include "./Setting/PlatformSetting.h"
#include "./Utility/IniItem.h"
#include "ScatterObserver.h"

SCIDownloadWidget::SCIDownloadWidget(QTabWidget* parent, MainWindow *window) :
    TabWidgetBase(8, tr("&SCI Download"), parent),
    ui(new Ui::SCIDownloadWidget),
    main_window_(window),
    header_(new CheckHeader(Qt::Horizontal, this)),
    delegate_(new DLDelegate(SCI_SCATTER, this, main_window_, this)),
    iscombo_sci_(false)
{
    ui->setupUi(this);

    setAcceptDrops(true);

    ui->tableWidget->setHorizontalHeader(header_);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    header_->SetChecked(true);

    CreateSignalConnection();

    main_window_->main_controller()->GetPlatformSetting()->addObserver(this);
}

SCIDownloadWidget::~SCIDownloadWidget()
{
    delete header_;

    delete ui;
}

void SCIDownloadWidget::CreateSignalConnection()
{
    connect(header_, SIGNAL(sectionClicked(int)),
                     this, SLOT(slot_OnHeaderView_click(int)));

    connect(this, SIGNAL(signal_sci_load_finished()),
            this, SLOT(slot_OnSCILoadEnd()));

    connect(this, SIGNAL(signal_sci_load_failed()),
            this, SLOT(slot_OnSCILoadFailed()));
}

void SCIDownloadWidget::LockOnUI()
{
    ui->pushButton_SCIDownload->setEnabled(false);
    ui->pushButton_Stop->setEnabled(true);
    ui->pushButton_SCIscatterLoading->setEnabled(false);
    ui->tableWidget->setEnabled(false);
}

void SCIDownloadWidget::DoFinished()
{
    ui->pushButton_SCIDownload->setEnabled(true);
    ui->pushButton_Stop->setEnabled(false);
    ui->pushButton_SCIscatterLoading->setEnabled(true);
    ui->tableWidget->setEnabled(true);
}

void SCIDownloadWidget::UpdateUI()
{
    ui->retranslateUi(this);
}

void SCIDownloadWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_SCIDOWNLOAD);

    tab_widget->setTabText(index, label);
}

void SCIDownloadWidget::SetShortCut(int , const QString &)
{

}

void SCIDownloadWidget::onPlatformChanged()
{
    delegate_->onPlatformChanged();
}

void SCIDownloadWidget::OnLoadSCIScatterEnd()
{
    emit signal_sci_load_finished();
}

void SCIDownloadWidget::OnLoadSCIScatterFailed()
{
    emit signal_sci_load_failed();
}

void SCIDownloadWidget::slot_OnSCILoadEnd()
{
    delegate_->setTableWidget(ui->tableWidget);

    if(!delegate_->onLoadEnd())
        ui->lineEdit_Path->setText(tr(""));

    if(delegate_->has_uncheck_item())
        header_->SetChecked(false);
    else
        header_->SetChecked(true);

    iscombo_sci_ = main_window_->main_controller()->IsComboSCIDownload();

    main_window_->scatter_observer()->notifyObservers(iscombo_sci_);
}

void SCIDownloadWidget::slot_OnSCILoadFailed()
{
    delegate_->onLoadFailed();

    ui->lineEdit_Path->setText(tr(""));    
}

void SCIDownloadWidget::LoadSCIScatterFile(const QString &file_path)
{
    delegate_->LoadScatterFile(TO_NATIVE_(file_path),
                               new SimpleCallback<SCIDownloadWidget>(this, &SCIDownloadWidget::OnLoadSCIScatterEnd),
                               new SimpleCallback<SCIDownloadWidget>(this, &SCIDownloadWidget::OnLoadSCIScatterFailed)
                               );
}

void SCIDownloadWidget::on_pushButton_SCIscatterLoading_clicked()
{
    IniItem item("history.ini", "SCILastDir", "lastDir");

    QString last_dir = item.GetStringValue();

    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("Open SCI Scatter File"),
                                                     last_dir,
                                                     tr("Bin File (*.bin)"));

    item.SaveStringValue(TO_NATIVE_(file_name));

    if(!file_name.isEmpty())
    {
        LoadSCIScatterFile(file_name);

        ui->lineEdit_Path->setText(TO_NATIVE_(file_name));
    }
}

void SCIDownloadWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        QPoint point = event->pos();

        point = QWidget::mapFromGlobal(point);

        QRect rect = ui->tableWidget->geometry();

        if(rect.contains(event->pos()))
        {
            event->acceptProposedAction();
        }
    }
}

void SCIDownloadWidget::dropEvent(QDropEvent *event)
{
    if(!ui->tableWidget->underMouse())
    {
        return;
    }

    QList<QUrl> urls = event->mimeData()->urls();

    if(urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
        return;

    LoadSCIScatterFile(fileName);

    ui->lineEdit_Path->setText(TO_NATIVE_(fileName));
}

void SCIDownloadWidget::UpdateImageList(bool checked)
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, ColumnEnable);

        item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);

        item = ui->tableWidget->item(i, ColumnName);

        main_window_->main_controller()->EnableROM(item->text().toStdString(), checked);
    }
}

void SCIDownloadWidget::slot_OnHeaderView_click(int index)
{
    if(index == 0)
    {
        UpdateImageList(header_->GetChecked());
    }
}

bool SCIDownloadWidget::ValidBeforeDownload()
{
    if(ui->lineEdit_Path->text().isEmpty())
    {
        QMessageBox::warning(this,  LoadQString(LANGUAGE_TAG, IDS_STRING_TOOL_NAME),
                                     LoadQString(LANGUAGE_TAG, IDS_STRING_WARNING_LOADSCI),
                                     LoadQString(LANGUAGE_TAG, IDS_STRING_OK),
                                     LoadQString(LANGUAGE_TAG, IDS_STRING_CANCEL),
                                     LoadQString(LANGUAGE_TAG, IDS_STRING_HELP));
        return false;
    }

    if(!delegate_->hasEnableRom())
    {
        QMessageBox::warning(this,  LoadQString(LANGUAGE_TAG, IDS_STRING_TOOL_NAME),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_WARNING_ROMNOTSELECT),
                             LoadQString(LANGUAGE_TAG, IDS_STRING_OK));
        return false;
    }

    return true;
}

void SCIDownloadWidget::on_pushButton_SCIDownload_clicked()
{
    if(ValidBeforeDownload())
    {
        delegate_->startSCIDownload();

        main_window_->LockOnUI();
        main_window_->GetOkDialog()->setWindowTitle(LoadQString(LANGUAGE_TAG, IDS_STRING_DOWNLOAD_OK));
    }
}

void SCIDownloadWidget::on_pushButton_Stop_clicked()
{
    if(NULL != delegate_)
        delegate_->stopExecute();
}
