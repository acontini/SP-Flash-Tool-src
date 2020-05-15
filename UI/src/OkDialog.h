#ifndef OKDIALOG_H
#define OKDIALOG_H

#include <QDialog>

namespace Ui {
    class OkDialog;
}

class OkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OkDialog(QWidget *parent = 0);
    ~OkDialog();

private:
    Ui::OkDialog *ui;
};

#endif // OKDIALOG_H
