#ifndef NORFLASHWIDGET_H
#define NORFLASHWIDGET_H

#include <QWidget>

namespace Ui {
    class NorFlashWidget;
}

class NorFlashWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NorFlashWidget(QWidget *parent = 0);
    ~NorFlashWidget();

    void setNorFlashInfo(const QString& id, const QString& size, const QString& otp_size);

private:
    Ui::NorFlashWidget *ui;
};

#endif // NORFLASHWIDGET_H
