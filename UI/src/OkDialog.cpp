#include "OkDialog.h"
#include "ui_OkDialog.h"
#include "../../Host/Inc/RuntimeMemory.h"

OkDialog::OkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OkDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

OkDialog::~OkDialog()
{
    delete ui;
}
