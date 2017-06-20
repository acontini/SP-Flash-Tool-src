#ifndef CHIPINFOWIDGET_H
#define CHIPINFOWIDGET_H

#include <QWidget>

namespace Ui {
    class ChipInfoWidget;
}

class ChipInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChipInfoWidget(QWidget *parent = 0);
    ~ChipInfoWidget();

    void setGeneralInfo(const QString& chip_name, const QString& chip_version, const QString& ext_clock,
                        const QString &ram_type, const QString&ram_size, const QString& sram_size);

private:
    Ui::ChipInfoWidget *ui;
};

#endif // CHIPINFOWIDGET_H
