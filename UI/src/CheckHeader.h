#ifndef CHECKHEADER_H
#define CHECKHEADER_H

#include <QHeaderView>
#include <QPainter>
#include <QMouseEvent>
#include <QtGui>

namespace Ui{
class CheckHeader;
}

class CheckHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit CheckHeader(Qt::Orientation orientation, QWidget *parent = 0);
    void SetChecked(bool checked);
    bool GetChecked(){return ischecked_;}

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void mousePressEvent(QMouseEvent *e);

private:
    QRect CheckBoxRect(const QRect &sourceRect)const;

    bool ischecked_;

signals:
    void toggled(bool checked);
    
public slots:
    
};

#endif // CHECKHEADER_H
