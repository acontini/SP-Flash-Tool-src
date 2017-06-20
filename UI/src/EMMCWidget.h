#ifndef EMMCWIDGET_H
#define EMMCWIDGET_H

#include <QWidget>

namespace Ui {
    class EMMCWidget;
}

class EMMCWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EMMCWidget(QWidget *parent = 0);
    ~EMMCWidget();

    void setEmmcInfo(const QString &boot1_size, const QString &boot2_size,
                     const QString &rmpb_size, const QString &gp1_size,
                     const QString &gp2_size, const QString &gp3_size,
                     const QString &gp4_size, const QString &ua_size);

private:
    Ui::EMMCWidget *ui;
};

#endif // EMMCWIDGET_H
