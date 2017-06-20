#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
namespace Ui
{
class AboutDialog;
}

class MainWindow;
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(MainWindow *window, QWidget *parent = 0);
    ~AboutDialog();

    void UpdateVersionInfo();
    void UpdateUI();

    static const QString S_BUILD;
    static const QString S_COMMENT;
    static const QString S_BUILD_DATE;
    static const QString S_DA_V_PRE;

private:
    MainWindow *main_window;
    Ui::AboutDialog *ui;

signals:

public slots:

};

#endif // ABOUTDIALOG_H
