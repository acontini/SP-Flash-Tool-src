#ifndef NANDWIDGET_H
#define NANDWIDGET_H

#include <QWidget>

namespace Ui {
    class NandWidget;
}

class NandWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NandWidget(QWidget *parent = 0);
    ~NandWidget();

    void setNandInfo(const QString &id, const QString& size, const QString &page_size,
                     const QString &spare_size, const QString &page_per_block,
                     const QString &bmt_exist);

private:
    Ui::NandWidget *ui;
};

#endif // NANDWIDGET_H
