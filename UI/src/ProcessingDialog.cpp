#include "ProcessingDialog.h"
#include "ProcessingAnimation.h"
#include "ui_ProcessingDialog.h"
#include "../../Host/Inc/RuntimeMemory.h"

#include <QtGui>

ProcessingDialog::ProcessingDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ProcessingDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    animation = new ProcessingAnimation(this);
    ui->animationLayout->addWidget(animation);
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    showCancelButton(false);
}

ProcessingDialog::~ProcessingDialog()
{
    delete ui;
    delete animation;
}

void ProcessingDialog::showDialog(const QString& message)
{
    this->show();
    animation->setAnimated(true);
    ui->Message->setText(message);
}

void ProcessingDialog::hideDialog()
{
    animation->setAnimated(false);
    this->close();
}

void ProcessingDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        drag_position = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void ProcessingDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - drag_position);
        event->accept();
    }
}

void ProcessingDialog::showCancelButton(bool show)
{
    ui->pushButton_cancel->setVisible(show);
}

void ProcessingDialog::on_pushButton_cancel_clicked()
{
    emit user_cancel_processing();
}
