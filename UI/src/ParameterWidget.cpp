#include "ParameterWidget.h"
#include "ui_ParameterWidget.h"
#include "MainWindow.h"
#include "MainController.h"
#include "ICallback.h"
#include "../../Logger/Log.h"
#include "../../Utility/constString.h"
#include "../../Host/Inc/RuntimeMemory.h"

ParameterWidget::ParameterWidget(QTabWidget *parent, MainWindow *window) :
    TabWidgetBase(6, tr("&Parameter"), parent),
    ui_(new Ui::ParameterWidget),
    main_window_(window)
{
    ui_->setupUi(this);

    otp_write_dialog_ = new OTPWriteDialog(this, main_window_->GetLanguageTag());
    otp_read_dialog_ = new OTPReadDialog(this, main_window_->GetLanguageTag());
}

ParameterWidget::~ParameterWidget()
{
    delete otp_write_dialog_;
    delete otp_read_dialog_;

    delete ui_;
}

void ParameterWidget::LockOnUI()
{
    ui_->STOP->setEnabled(true);
    ui_->UPDATE->setEnabled(false);
    ui_->OTP->setEnabled(false);
    ui_->frame_OTP->setEnabled(false);
}

void ParameterWidget::DoFinished()
{
    ui_->STOP->setEnabled(false);
    ui_->UPDATE->setEnabled(true);
    ui_->OTP->setEnabled(true);
    ui_->frame_OTP->setEnabled(true);
}

void ParameterWidget::UpdateUI()
{
    ui_->retranslateUi(this);
    otp_write_dialog_->UpdateUI();
    otp_read_dialog_->UpdateUI();
}

void ParameterWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_PARAMETER);

    tab_widget->setTabText(index, label);
}

void ParameterWidget::SetShortCut(int cmd, const QString &shortcut)
{
    switch(cmd)
    {
    case PARA_UPDATE:
        ui_->UPDATE->setShortcut(shortcut);
        ui_->UPDATE->setToolTip(shortcut);
        break;

    case PARA_STOP:
        ui_->STOP->setShortcut(shortcut);
        ui_->STOP->setToolTip(shortcut);
        break;
    }
}

void ParameterWidget::OnScatterChanged(bool showRegion)
{
    otp_write_dialog_->onScatterChanged(showRegion);
    otp_read_dialog_->onScatterChanged(showRegion);
}

void ParameterWidget::on_OTPSettingBtn_clicked()
{
    if(ui_->OTP_Write->isChecked())
    {
        main_window_->GetOkDialog()->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_WRITE_OTP_OK));
        otp_write_dialog_->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_OTP_WRITE_SETTING));
        otp_write_dialog_->show();
        if(QDialog::Accepted == otp_write_dialog_->exec())
        {
            QString str = QString::fromLocal8Bit((otp_write_dialog_->get_otp_file()).c_str());
            QString ss;
            ss.sprintf("addr(0x%08X), len(0x%08X), ",
                       otp_write_dialog_->get_otp_addr(),
                       otp_write_dialog_->get_otp_len());
            ss.append(str);
            ui_->OTP_WriteInfoEdit->setText(ss);
        }
    }
    if(ui_->OTP_Read->isChecked())
    {
          main_window_->GetOkDialog()->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_READ_OTP_OK));
          otp_read_dialog_->setWindowTitle(LoadQString(main_window_->GetLanguageTag(), IDS_STRING_OTP_READ_SETTING));
          otp_read_dialog_->show();
          if(QDialog::Accepted == otp_read_dialog_->exec())
          {
             QString str = QString::fromLocal8Bit((otp_read_dialog_->get_otp_file()).c_str());
             QString ss;
             ss.sprintf("addr(0x%08X), len(0x%08X), ",
                         otp_read_dialog_->get_otp_addr(),
                         otp_read_dialog_->get_otp_len());
             ss.append(str);
             ui_->OTPReadInfoEdit->setText(ss);
          }
    }
}

void ParameterWidget::on_UPDATE_clicked()
{
    if(this->validateBeforeOTP())
    {
        main_window_->main_controller()->SetPlatformSetting();
        main_window_->main_controller()->SetConnSetting(main_window_->CreateConnSetting());
        main_window_->main_controller()->QueueAJob(main_window_->CreateOTPSetting());
        main_window_->main_controller()->StartExecuting(
                new SimpleCallback<MainWindow>(main_window_,&MainWindow::DoFinished));
        main_window_->LockOnUI();
    }
}

void ParameterWidget::on_OTP_clicked()
{
    bool uiState;
    uiState = ui_->OTP->isChecked();

    ui_->gb_OTP_para->setEnabled(uiState);
    ui_->OTP_Write->setEnabled(uiState);
    ui_->OTP_Read->setEnabled(uiState);
    ui_->OTP_ReadInfoLabel->setEnabled(uiState);
    ui_->OTP_WriteInfoLabel->setEnabled(uiState);
    ui_->OTP_WriteInfoEdit->setEnabled(uiState);
    ui_->OTPReadInfoEdit->setEnabled(uiState);
    ui_->OTPSettingBtn->setEnabled(uiState);
}

bool ParameterWidget::validateBeforeOTP()
{
   bool ret = true;

   if(ui_->OTP->isChecked()&&
           ui_->OTP_WriteInfoEdit->text().isEmpty()&&
           ui_->OTP_Write->isChecked())
   {
       main_window_->ShowHelpContents(this, IDS_STRING_NULL_WRITE_LOCATION_OTP,
                                      "E_APP_NULL_WRITE_FILE_OTP.htm");

       LOG("Please select OTP write file first!", __FUNCTION__);
       ret = false;
   }
   else if(ui_->OTP->isChecked()&&
           ui_->OTPReadInfoEdit->text().isEmpty()&&
           ui_->OTP_Read->isChecked())
   {
       main_window_->ShowHelpContents(this, IDS_STRING_NULL_READ_LOCATION_OTP,
                                      "E_APP_NULL_READ_FILE_OTP.htm");

       LOG("Please select OTP read file first!", __FUNCTION__);
       ret = false;
   }
   return ret;
}

OTP_OPERATION ParameterWidget::get_operation() const
{
    OTP_OPERATION operation = OTP_READ;

    if(ui_->OTP_Read->isChecked())
    {
       operation = OTP_READ;
    }

    else if(ui_->OTP_Write->isChecked())
    {
       operation = this->otp_write_dialog_->get_otp_oper();
    }

    return operation;
}

void ParameterWidget::on_STOP_clicked()
{
    main_window_->main_controller()->StopByUser();
}

