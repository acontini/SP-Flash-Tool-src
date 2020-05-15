#ifndef UFSWIDGET_H
#define UFSWIDGET_H

#include <QWidget>

namespace Ui {
    class UfsWidget;
}

class UfsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UfsWidget(QWidget *parent = 0);
    ~UfsWidget();

    void setUfsInfo(const QString &lu0_size, const QString &lu1_size,
                    const QString &lu2_size/*, const QString &lu3_size*/);

private:
    Ui::UfsWidget *ui;
};

#endif // UFSWIDGET_H
