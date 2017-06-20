#include "CommandSetting.h"
#include "../XMLParser/XMLDocument.h"
#include "../Host/Inc/RuntimeMemory.h"
#include <QtGlobal>
namespace ConsoleMode
{
CommandSetting::CommandSetting():
    efuse_read_only_(false)
{
	 vClearSetting();
}

CommandSetting::~CommandSetting()
{
}

CommandSetting::CommandSetting(const XML::Node &node, bool efuse_read_only, bool reboot)
{
    efuse_read_only_ = efuse_read_only;
    isNeedEnableWatchDog_ = reboot;

    LoadXML(node);
}

void CommandSetting::vSetCommand(const std::string &command)
{
    if(!command.empty())
    {
      //  vClearSetting();

        vCreateSetting(command, XML::Node(NULL));

        if( isNeedEnableWatchDog_ == true)
        {
            QSharedPointer<ISetting> p2 = CreateWatchDogSetting();
            if(!p2.isNull())
                vAddSetting(p2);
        }
    }

}

void CommandSetting::SaveXML(XML::Node &node) const
{
    LOGI("CommandSetting::SaveXML");
    for(std::list<QSharedPointer<ISetting> >::const_iterator it = m_aclCommandSettings.begin();
        it != m_aclCommandSettings.end(); ++it)
    {
        (*it)->SaveXML(node);
    }
    LOGI("CommandSetting::SaveXML Done!");
}

void CommandSetting::LoadXML(const XML::Node &node)
{
  //  vClearSetting();

    XML::Node child_node = node.GetFirstChildNode();
    while(!child_node.IsEmpty())
    {
        const std::string node_name = child_node.GetName();
        vCreateSetting(node_name, child_node);
        child_node = child_node.GetNextSibling();
    }

    if( isNeedEnableWatchDog_ == true)
    {
        QSharedPointer<ISetting> p2 = CreateWatchDogSetting();
        if(!p2.isNull())
            vAddSetting(p2);
    }
}

void CommandSetting::vCreateSetting(const std::string& command, const XML::Node &node)
{
    QSharedPointer<ISetting> p1;

    if ("format"==command)
    {
        p1 = CreateFormatSetting();//new APCore::FormatSetting();
        LOGI("Format Command::Create");
    }
    else if("combo-format"==command)
    {
        p1 = CreateComboFormatSetting();//new APCore::ComboFormatSetting();
        LOGI("Combo Format Command::Create");
    }
    else if("da-download-all"==command)//not support by cmd
    {
        p1 = CreateDownloadSetting();//new APCore::DADownloadAllSetting();
    }
    else if("download-only"==command)
    {
        p1 = CreateDLOnlySetting();//new APCore::DLOnlySetting();
        isNeedEnableWatchDog_ = true;
		LOGI("Download Command::Create");
        LOGI("DownloadCommand::Execute");
    }
    else if("format-download"==command)
    {
        p1 = CreateDLPlugFmtAllSetting();//new APCore::DLPlugFmtAllSetting();
        isNeedEnableWatchDog_ = true;
        LOGI("DLPlugFmtAll Command::Create");
        LOGI("DownloadCommand::Execute");
    }
    else if ("download-withoutScatter" == command){
        p1 = CreateDLWithoutScatterSetting();
        isNeedEnableWatchDog_ = true;
        LOGI("Download without scatter command::Create");
        LOGI("DownloadCommand::Execute");
    }
    else if("cert-download"==command)
    {
        p1 = CreateCertDLSetting();//new APCore::CertDLSetting();
        LOGI("Cert download::Create");
        LOGI("Cert download::Execute");
    }
    else if("readback"==command)
    {
        p1 = CreateReadbackSetting();//new APCore::ReadbackSetting();
        LOGI("Readback ::Create");
    }
    else if("readback-withoutscatter" == command){
        p1 = CreateReadbackWithoutScatterSetting();
        LOGI("Readback without scatter::Create");
    }
    else if("firmware-upgrade"==command)
    {
        p1 = CreateFirmwareUpgradeSetting();//new APCore::FirmwareUpgradeSetting();
        isNeedEnableWatchDog_ = true;
        LOGI("Firmware Upgrade Command::Create");
        LOGI("DownloadCommand::Execute");   // why? it's a lie...
    }
    else if("efuse"==command)
    {
        p1 = CreateEfuseSetting();
        ((APCore::EfuseSetting*)p1.data())->SetOnlyRead(efuse_read_only_);
        LOGI("Efuse Command::Create");
    }
    else if("write-memory"==command)
    {
        p1 = CreateWriteMemorySetting();
        LOGI("Write Memory Command::Create");   // why? it's a lie...
    }

    if(!p1.isNull())
    {
        if(!node.IsEmpty())
            p1->LoadXML(node);
        vAddSetting(p1);
    }
}

QSharedPointer<APCore::DADownloadAllSetting> CommandSetting::CreateDownloadSetting()
{
    QSharedPointer<APCore::DADownloadAllSetting> setting(new APCore::DADownloadAllSetting());
    return setting;
}

QSharedPointer<APCore::FormatSetting> CommandSetting::CreateFormatSetting()
{
    QSharedPointer<APCore::FormatSetting> setting(new APCore::FormatSetting());

    setting->set_auto_format(true);
    setting->set_physical_fmt(false);
    setting->set_auto_format_flag(APCore::FormatSetting::Format_Except_BL);

    return setting;
}

QSharedPointer<APCore::ComboFormatSetting> CommandSetting::CreateComboFormatSetting()
{
    QSharedPointer<APCore::ComboFormatSetting> setting(new APCore::ComboFormatSetting());

    setting->set_format_setting(this->CreateFormatSetting());

    return setting;
}

QSharedPointer<APCore::DLOnlySetting> CommandSetting::CreateDLOnlySetting()
{
    QSharedPointer<APCore::DLOnlySetting> setting(new APCore::DLOnlySetting());

    setting->set_download_setting(this->CreateDownloadSetting());

    return setting;
}

QSharedPointer<APCore::DLWithoutScatterSetting> CommandSetting::CreateDLWithoutScatterSetting(){
    QSharedPointer<APCore::DLWithoutScatterSetting> setting(new APCore::DLWithoutScatterSetting());

    return setting;
}

QSharedPointer<APCore::ComboFormatSetting> CommandSetting::CreateDLPlugFmtAllFormatSetting()
{

    QSharedPointer<APCore::ComboFormatSetting> setting = this->CreateComboFormatSetting();
    setting->format_setting()->set_auto_format_flag(APCore::FormatSetting::Format_ALL);
    setting->format_setting()->set_physical_fmt(true);

    return setting;
}

QSharedPointer<APCore::DLPlugFmtAllSetting> CommandSetting::CreateDLPlugFmtAllSetting()
{
    QSharedPointer<APCore::DLPlugFmtAllSetting> setting(new APCore::DLPlugFmtAllSetting());

    setting->set_format_setting(this->CreateDLPlugFmtAllFormatSetting());
    setting->set_download_setting(this->CreateDownloadSetting());

    return setting;
}

QSharedPointer<APCore::FormatSetting> CommandSetting::CreateFirmwareUpgradeFormatSetting()
{
    QSharedPointer<APCore::FormatSetting> setting(new APCore::FormatSetting());

    setting->set_auto_format(true);
    setting->set_physical_fmt(true);
    setting->set_auto_format_flag(APCore::FormatSetting::Format_ALL);

    return setting;
}

QSharedPointer<APCore::FirmwareUpgradeSetting> CommandSetting::CreateFirmwareUpgradeSetting()
{
    QSharedPointer<APCore::FirmwareUpgradeSetting> setting(new APCore::FirmwareUpgradeSetting());
    setting->set_format_setting(this->CreateFirmwareUpgradeFormatSetting());
    setting->set_download_setting(this->CreateDownloadSetting());
    setting->set_readback_setting(this->CreateReadbackSetting());
    setting->set_writememory_setting(this->CreateWriteMemorySetting());
    Download_Scene scene = FIRMWARE_UPGRADE;
    setting->set_download_scene(scene);

    return setting;
}

QSharedPointer<APCore::WatchDogSetting> CommandSetting::CreateWatchDogSetting()
{
    QSharedPointer<APCore::WatchDogSetting> setting(new APCore::WatchDogSetting());

    return setting;
}

QSharedPointer<APCore::ReadbackSetting> CommandSetting::CreateReadbackSetting()
{
    QSharedPointer<APCore::ReadbackSetting> setting(new APCore::ReadbackSetting());
    setting->set_physicalReadbackEnabled(false);

    return setting;
}

QSharedPointer<APCore::ReadbackWithoutScatterSetting> CommandSetting::CreateReadbackWithoutScatterSetting()
{
    QSharedPointer<APCore::ReadbackWithoutScatterSetting> setting(new APCore::ReadbackWithoutScatterSetting());

    return setting;
}

QSharedPointer<APCore::WriteMemorySetting> CommandSetting::CreateWriteMemorySetting()
{
    QSharedPointer<APCore::WriteMemorySetting> setting(new APCore::WriteMemorySetting());

    return setting;
}

QSharedPointer<APCore::CertDLSetting> CommandSetting::CreateCertDLSetting()
{
    QSharedPointer<APCore::CertDLSetting> setting(new APCore::CertDLSetting());

    return setting;
}

QSharedPointer<APCore::EfuseSetting> CommandSetting::CreateEfuseSetting()
{
    QSharedPointer<APCore::EfuseSetting> setting(new APCore::EfuseSetting());

    return setting;
}

QSharedPointer<APCore::ICommand> CommandSetting::pclCreateCommand(const APKey & key , const HW_StorageType_E& storage_type,
                                                                  const std::string &da, const bool& is_combo_fmt) const
{
    LOGV("DownloadCommand::Creat MacroCommand");
    QSharedPointer<APCore::MacroCommand> macro_cmd = QSharedPointer<APCore::MacroCommand>(new APCore::MacroCommand(key));
    std::list<QSharedPointer<ISetting> >::const_iterator it = m_aclCommandSettings.begin();
    for(;it != m_aclCommandSettings.end(); ++it)
    {

        APCore::ComboFormatSetting *fmt = dynamic_cast<APCore::ComboFormatSetting*>(it->data());
        if(NULL != fmt)
        {
            fmt->set_is_combo_dl(is_combo_fmt);
        }

        APCore::DLPlugFmtAllSetting *dlfmt = dynamic_cast<APCore::DLPlugFmtAllSetting*>(it->data());

        if(NULL != dlfmt)
        {
            dlfmt->set_combo_fmt_flag(is_combo_fmt);
        }

        (*it)->set_storage_type(storage_type);
        (*it)->set_da(da);
        QSharedPointer<APCore::ICommand> cmd = (*it)->CreateCommand(key);
        macro_cmd->vAddCommand(cmd);
    }
    return macro_cmd;
}

std::list<QSharedPointer<ISetting> > CommandSetting::aclGetCmdSettingList() const
{
    return m_aclCommandSettings;
}

void CommandSetting::vAddSetting(const QSharedPointer<ISetting>& setting)
{
    m_aclCommandSettings.push_back(setting);
}

void CommandSetting::vClearSetting()
{
    LOGI("Clear all Commands!!");
    isNeedEnableWatchDog_ = false;
    m_aclCommandSettings.clear();
}
}
