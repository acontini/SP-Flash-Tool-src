#ifndef OTPREADDIALOG_H
#define OTPREADDIALOG_H

#include <QDialog>
#include "../../Utility/constString.h"

namespace Ui {
    class OTPReadDialog;
}

class OTPReadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OTPReadDialog(QWidget *parent, Language_Tag _tag);
    ~OTPReadDialog();
    void onScatterChanged(bool showRegion);

    //getters
    unsigned int get_otp_addr() const;
    unsigned int get_otp_len() const;
    std::string get_otp_file() const;

    void UpdateUI();

signals:
    void signal_start_otp();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Language_Tag lang_tag;
    Ui::OTPReadDialog *ui;   
};

#endif // OTPREADDIALOG_H
