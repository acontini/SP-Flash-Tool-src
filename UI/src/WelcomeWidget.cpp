#include "WelcomeWidget.h"
#include "MainWindow.h"
#include "ui_WelcomeWidget.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include <QtWebKit>

WelcomeWidget::WelcomeWidget(QTabWidget *parent, MainWindow *window):
    TabWidgetBase(0, tr("W&elcome"), parent),
    main_window_(window),
    ui_(new Ui::WelcomeWidget)
{
    ui_->setupUi(this);
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui_;
}

void WelcomeWidget::LockOnUI()
{
}

void WelcomeWidget::DoFinished()
{
}

void WelcomeWidget::UpdateUI()
{
    ui_->retranslateUi(this);
}

void WelcomeWidget::SetShortCut(int cmd, const QString &shortcut)
{
    LOGI("The cmd is %d and shortcut is %s", cmd, shortcut.toLocal8Bit().constData());
}

void WelcomeWidget::SetTabLabel(QTabWidget *tab_widget, int index)
{
    QString label = LoadQString(LANGUAGE_TAG, IDS_STRING_WELCOME);

    tab_widget->setTabText(index, label);
}
