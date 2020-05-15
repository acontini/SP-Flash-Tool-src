#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <list>
#include "GeneralSetting.h"
#include "CommandSetting.h"
#include "CommandLineArguments.h"
#include <QSharedPointer>

namespace ConsoleMode
{

class Config
{
public:
    Config();
    Config(const CommandLineArguments& args);
    ~Config();

    void LoadFile(const std::string &file_name, bool efuse_read_only = false, bool reboot = false);
    void SaveFile(const std::string &file_name);

    QSharedPointer<GeneralSetting> pclGetGeneralSetting()
    {
        return m_pclGeneralSetting;
    }

    QSharedPointer<CommandSetting> pclGetCommandSetting()
    {
        return m_pclCommandSetting;
    }

    void vSetGeneralSetting(const QSharedPointer<GeneralSetting>& setting)
    {
        m_pclGeneralSetting = setting;
    }

    void vAddCommandSetting(const QSharedPointer<ISetting>& setting)
    {
        m_pclCommandSetting->vAddSetting(setting);
    }

    void vSetCommandSetting(const QSharedPointer<CommandSetting>& setting)
    {
        m_pclCommandSetting = setting;
    }

    bool fgIsCommboFmt(QSharedPointer<AppCore>& app,const APKey& key) const; //should call after general command executing
    HW_StorageType_E eGetStorageType() const;

    std::string eGetDAFile() const;
   // std::string clGetChipName() const;

private:
    Config(const Config&);
    Config& operator=(const Config&);
    QSharedPointer<GeneralSetting> m_pclGeneralSetting;
    QSharedPointer<CommandSetting> m_pclCommandSetting;

};

}

#endif // CONFIG_H
