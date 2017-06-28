#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include "TabWidgetBase.h"
#include "OTPWriteDialog.h"
#include "OTPReadDialog.h"
#include "ScatterObserver.h"

namespace Ui
{
class ParameterWidget;
}
class MainWindow;

class ParameterWidget : public TabWidgetBase, public IScatterObj
{
    Q_OBJECT
public:
    explicit ParameterWidget(QTabWidget *parent, MainWindow *window);
    ~ParameterWidget();

    DECLARE_TABWIDGET_VFUNCS()

    virtual void OnScatterChanged(bool showRegion);

    OTPWriteDialog* otp_write_dialog() const {return otp_write_dialog_;}
    OTPReadDialog* otp_read_dialog() const {return otp_read_dialog_;}
    OTP_OPERATION get_operation() const;
private:
    Ui::ParameterWidget *ui_;
    MainWindow *main_window_;
    OTPWriteDialog *otp_write_dialog_;
    OTPReadDialog *otp_read_dialog_;

private:
    bool validateBeforeOTP();
signals:

public slots:

private slots:
    void on_OTPSettingBtn_clicked();
    void on_UPDATE_clicked();
    void on_OTP_clicked();
    void on_STOP_clicked();
};

#endif // PARAMETERWIDGET_H
