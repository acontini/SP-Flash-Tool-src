#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include "TabWidgetBase.h"

namespace Ui
{
class WelcomeWidget;
}

class MainWindow;

class WelcomeWidget : public TabWidgetBase
{
public:

    WelcomeWidget(QTabWidget *parent, MainWindow *window);
    ~WelcomeWidget();

    DECLARE_TABWIDGET_VFUNCS()

private:
    MainWindow *main_window_;
    Ui::WelcomeWidget *ui_;
};

#endif // WELCOMEWIDGET_H
