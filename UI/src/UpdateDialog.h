#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class UpdateDialog;
}

class AsyncUpdater;
class MainWindow;

class UpdateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdateDialog(MainWindow *parent, AsyncUpdater *updater);
    ~UpdateDialog();

    void UpdateUI();
private slots:
    void on_pushButton_remind_clicked();

    void on_pushButton_install_clicked();

private:
    AsyncUpdater *async_updater;
    Ui::UpdateDialog *ui;
    MainWindow *mainWindow;
};

#endif // UPDATEDIALOG_H
