#ifndef TABWIDGETBASE_H
#define TABWIDGETBASE_H

#include <QTabWidget>
#include "../../Logger/Log.h"

#define DECLARE_TABWIDGET_VFUNCS()    \
    virtual void LockOnUI();   \
    virtual void DoFinished();    \
    virtual void UpdateUI();  \
    virtual void SetTabLabel(QTabWidget *, int );\
    virtual void SetShortCut(int , const QString &);

class TabWidgetBase : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidgetBase(int index, const QString& name, QTabWidget *parent);
    virtual ~TabWidgetBase(){}

    virtual void LockOnUI() = 0;
    virtual void DoFinished() = 0;

    virtual void UpdateUI() = 0;
    virtual void SetTabLabel(QTabWidget *tab_widget, int index) = 0;

    virtual void SetShortCut(int cmd, const QString &shortcut) = 0;

    void Detach();
    void Attach();

    bool IsSelectedTab();
    void SetTabEnabled(bool enable);
protected:
    QTabWidget * tab_parent;
    const int tab_index;
    const QString tab_name;

    bool tab_visible;

signals:

public slots:

};

#endif // TABWIDGETBASE_H
