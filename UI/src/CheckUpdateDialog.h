#ifndef CHECKUPDATEDIALOG_H
#define CHECKUPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class CheckUpdateDialog;
}

class AsyncUpdater;
class MainWindow;

class CheckUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckUpdateDialog(MainWindow *parent, AsyncUpdater *updater);
    ~CheckUpdateDialog();

    void SetCheckUpdateInfo(const QString &str);
    void SetOKButtonVisible(const bool& show);

    void UpdateUI();
signals:
    void signal_check_finish(const bool &bUpdate);

private slots:
    void on_pushButton_ok_clicked();

private:
    AsyncUpdater *async_checker;
    Ui::CheckUpdateDialog *ui;
    MainWindow *mainWindow;
};

#endif // CHECKUPDATEDIALOG_H
