#ifndef ENCRYDIALOG_H
#define ENCRYDIALOG_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class EncryDialog;
}

class MainWindow;

class EncryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EncryDialog(QWidget *parent, MainWindow *window);
    ~EncryDialog();

    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *);
    void UpdateUI();

private:
    bool checkPassword();
    
private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_lineEdit_password_returnPressed();

private:
    Ui::EncryDialog *ui;
    QString strPassword_;
    QString strPassword_day_;
    QString strPassword_week_;
    QString strPassword_month_;

    MainWindow *main_windows_;
};

#endif // ENCRYDIALOG_H
