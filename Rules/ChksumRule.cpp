#include "ChksumRule.h"
#include "../Logger/Log.h"
#include "../Utility/FileUtils.h"
#include "../Host/Inc/RuntimeMemory.h"

#include <QDir>
#include <QSettings>
#include <string>
#include <algorithm>


const std::string ChksumRule::kChksumFileName("Checksum.ini");

ChksumRule::ChksumRule(const std::string &path)
{
   IniChksumMap(path);
}

ChksumRule::~ChksumRule()
{}

void ChksumRule::IniChksumMap(const std::string &path)
{
    std::string chksum_file_path = path + QDir::separator().toLatin1() + kChksumFileName;
    QString file_path = QString(QString::fromLocal8Bit(chksum_file_path.c_str()));

    if(!FileUtils::IsFileExist(chksum_file_path))
    {
        LOG("Checksum File(%s) is not found!", chksum_file_path.c_str());
        this->is_do_chksum_ = false;
        return;
    }

    QSettings *chksum_setting = new QSettings(file_path, QSettings::IniFormat);

    chksum_setting->beginGroup("IsEnableChecksum");
    is_do_chksum_ = chksum_setting->value("CHECKSUM_SWITCH").toBool();
    LOGD("Check if do checksum(%d)", is_do_chksum_);
    chksum_setting->endGroup();

    chksum_setting->beginGroup("CheckSum");
    QStringList chksum_list = chksum_setting->allKeys();
    std::string chksum_name;
    unsigned int chksum_value = 0;
    bool ok = true;
    for(int i = 0; i< chksum_list.count(); i++)
    {
        chksum_value = chksum_setting->value(chksum_list.at(i)).toString().toUInt(&ok,0);
        chksum_name = std::string(chksum_list.at(i).toLocal8Bit());
        image_chksum_map_.insert(std::make_pair(chksum_name, chksum_value));
        LOGD("chksum name(%s), chksum value(0x%x)", chksum_name.c_str(), chksum_value);
    }

    if(chksum_setting!=NULL)
    {
        delete chksum_setting;
    }

}

unsigned int ChksumRule::QueryChksumByImageName(const std::string &image_name)
{
    std::string Image_Name = image_name;

    std::map<std::string, unsigned int>::iterator it = image_chksum_map_.find(Image_Name);
    if(it!=image_chksum_map_.end())
    {
        return it->second;
    }
    else
    {
        LOGD("Checksum Value for (%s) is not found in the map.", image_name.c_str());
        return 0;
    }
}
