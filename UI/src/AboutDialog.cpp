#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "MainWindow.h"
#include "MainController.h"

#include "../../BootRom/brom.h"
#include "../../Utility/version.h"
#include "../../Host/Inc/RuntimeMemory.h"

const QString AboutDialog::S_BUILD("Build: ");
const QString AboutDialog::S_COMMENT("Comment:");
const QString AboutDialog::S_BUILD_DATE("Build-Date: ");
const QString AboutDialog::S_DA_V_PRE("DA_v");

AboutDialog::AboutDialog(MainWindow *window, QWidget *parent) :
    QDialog(parent),
    main_window(window),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

  // this->setStyleSheet("background-image:url(:/images/background.jpg);");

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    UpdateUI();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::UpdateVersionInfo()
{
    //tool version info
    ui->label_title->setText(ToolInfo::ToolName().c_str());

    if(ToolInfo::IsCustomerVer())
        ui->label_comment->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_CUSTOM_RELEASE) + ToolInfo::Comments().c_str());
    else
        ui->label_comment->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_INTERNEL_RELEASE) + ToolInfo::Comments().c_str());

    ui->label_build_date->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_BUILD) + ToolInfo::BuildInfo().c_str());

    //da info
    DA_INFO da_info;
    if(main_window->main_controller()->GetDAInfo(&da_info))
    {
        ui->label_DA->setText(QString().sprintf("DA_v%s",da_info.version));
        ui->label_DA_Build_Date->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_BUILD_DATE) + QString().sprintf("%s", da_info.last_modified_date));
    }
    else
    {
        ui->label_DA->setText("");
        ui->label_DA_Build_Date->setText("");
    }

    //library version info
    const char *p_version      = NULL;
    const char *p_release_type = NULL;
    const char *p_build_date   = NULL;

    Brom_GetDLLInfo(&p_version,&p_release_type,&p_build_date,NULL);
    ui->label_Brom->setText(QString().sprintf("libflashtool.so v%s",p_version));
    ui->label_Brom_Release_Type->setText(p_release_type);
    ui->label_Brom_Build_Date->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_BUILD_DATE) + QString().sprintf("%s", p_build_date));
}

void AboutDialog::UpdateUI()
{
    UpdateVersionInfo();

    setWindowTitle(LoadQString(main_window->GetLanguageTag(), IDS_STRING_ABOUT_TOOL));
    ui->groupBox_DA_and_DLL_Info->setTitle(LoadQString(main_window->GetLanguageTag(), IDS_STRING_DA_INFO));
    ui->label_copyright->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_COPY_RIGHT));
    ui->pushButton_Ok->setText(LoadQString(main_window->GetLanguageTag(), IDS_STRING_OK));
}
