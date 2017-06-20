#ifndef OTPWRITEDIALOG_H
#define OTPWRITEDIALOG_H

#include <QDialog>
#include "../../BootRom/flashtool_api.h"
#include "../../Utility/constString.h"

namespace Ui {
    class OTPWriteDialog;
}

class OTPWriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OTPWriteDialog(QWidget *parent, Language_Tag tag);
    ~OTPWriteDialog();
    void onScatterChanged(bool showRegion);

    //getters
    OTP_OPERATION get_otp_oper() const;
    unsigned int get_otp_addr() const;
    unsigned int get_otp_len() const;
    std::string get_otp_file() const;

    // Orz | it should be a file name instead of otp data...
    const QSharedPointer<U8> &get_otp_data() const;

    void UpdateUI();

signals:
    void singal_start_otp();

private slots:
    void on_pb_OTPfile_path_clicked();


    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

private:
    void initDefault();

private:
    Ui::OTPWriteDialog *ui;
    QSharedPointer<U8> otp_data_;
    Language_Tag lang_tag;
};

#endif // OTPWRITEDIALOG_H
