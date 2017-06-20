#ifndef SDMMCWIDGET_H
#define SDMMCWIDGET_H

#include <QWidget>

namespace Ui {
    class SDMMCWidget;
}

class SDMMCWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SDMMCWidget(QWidget *parent = 0);
    ~SDMMCWidget();

    void setSdmmcInfo(const QString &id, const QString &size);

private:
    Ui::SDMMCWidget *ui;
};

#endif // SDMMCWIDGET_H
