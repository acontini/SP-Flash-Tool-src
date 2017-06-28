#include "TabWidgetBase.h"
#include <QtGui/QApplication>

TabWidgetBase::TabWidgetBase(int index, const QString& name, QTabWidget *parent) :
    QTabWidget(parent),
    tab_parent(parent),
    tab_index(index),
    tab_name(name),
    tab_visible(false)
{
}

void TabWidgetBase::Detach()
{
    int index = tab_parent->indexOf(this);
    if (index != -1)
    {
        tab_parent->removeTab(index);
    }
    tab_visible = false;
}

void TabWidgetBase::Attach()
{
    if (tab_parent->indexOf(this) == -1)
    {
        tab_parent->insertTab(tab_index, this, tab_name);
    }
    tab_visible = true;
}

bool TabWidgetBase::IsSelectedTab()
{
   int curIndex = tab_parent->currentIndex();
   int tabIndex = tab_parent->indexOf(this);

   return (curIndex == tabIndex);
}

void TabWidgetBase::SetTabEnabled(bool enable)
{
   this->setEnabled(enable);
}
