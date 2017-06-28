#include "GeneralCommand.h"
#include "../Setting/PlatformSetting.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include <list>

namespace ConsoleMode
{

GeneralCommand::GeneralCommand(QSharedPointer<GeneralArg> arg, const QSharedPointer<AppCore>& app, const APKey& key):m_pclArg(arg),m_pclApp(app),m_clApKey(key)
{
}

bool GeneralCommand::exec()
{
    LOGI("General Command exec");
    bool result =  LoadDA();

    if(!result)
        return false;

    if(!this->m_pclArg->scatter_file.empty())
    {
        result = result && LoadScatterFile() && LoadRoms();
        if(!result)
            return false;
    }

    return LoadAuthFile() && LoadCertFile();
}

bool GeneralCommand::LoadScatterFile()
{
    LOGI("General Command ::LoadScatterFile");
    bool is_validate = false;
    try{
        m_pclApp->LoadScatter(NORMAL_SCATTER, m_clApKey, this->m_pclArg->scatter_file);

        DL_PlatformInfo info;
        std::string error_hint;
        QSharedPointer<APCore::PlatformSetting> platform =
                QSharedPointer<APCore::PlatformSetting>(new APCore::PlatformSetting());

        memset(&info, 0, sizeof(info));
        m_pclApp->GetPlatformInfo(m_clApKey, info);
        is_validate = platform->initByNewScatterFile(info, error_hint);
        if(is_validate && m_pclArg->chip_name.empty())
        {
            m_pclArg->chip_name = platform->getLoadPlatformName();
            m_pclArg->storage_type = platform->getFlashToolStorageConfig().GetStorageType();
        }
    }
    catch(const BaseException& exception)
    {
        LOGE(exception.err_msg().c_str());
    }
    return is_validate;
}

bool GeneralCommand::LoadRoms()
{
    LOGI("General Command ::LoadRoms");
    bool result = true;
    try{
        if(!this->m_pclArg->rom_list.empty())
        {
            LOGI("load roms in rom list");
            std::list<ConsoleMode::RomItem>::const_iterator it =
                    this->m_pclArg->rom_list.begin();

            for(;it!=this->m_pclArg->rom_list.end(); ++it)
            {
                //for not enable img, no need to check LoadROM ok or not
                if(it->enable)
                {
                    result &= m_pclApp->LoadROM(m_clApKey, it->path, it->index);
                }
                m_pclApp->EnableROM(m_clApKey, it->index, it->enable);
            }
        }
        else
        {
            LOGI("rom list is empty, auto load all roms");
            result = m_pclApp->AutoLoadRomImages(m_clApKey, this->m_pclArg->scatter_file);

            std::list<ImageInfo> image_list;
            m_pclApp->GetROMList(m_clApKey,image_list,FORMAT_ALL_DOWNLOAD);

            for(std::list<ImageInfo>::const_iterator it = image_list.begin();
                it != image_list.end(); ++it)
            {
                if(it->visible)
                {
                    ConsoleMode::RomItem rom;
                    rom.index  = it->index;
                    rom.enable = it->enabled;
                    rom.path   = it->location;
                    this->m_pclArg->rom_list.push_back(rom);
                    LOGI("rom list:Index(%d), Enable(%d),Path:%s",rom.index,rom.enable,rom.path.c_str());
                }
            }
        }
    }
    catch(const BaseException& exception)
    {
        LOGE(exception.err_msg().c_str());
        result = false;
    }

    return result;
}

bool GeneralCommand::LoadDA()
{
    LOGI("General Command ::LoadDA");
    bool is_validate = false;
    if(!this->m_pclArg->da_file.empty())
    {
        try
        {
            m_pclApp->LoadDA(m_clApKey,this->m_pclArg->da_file);
            is_validate = true;
        }
        catch(const BaseException& exception)
        {
            LOGE(exception.err_msg().c_str());
        }
    }
    return is_validate;
}

bool GeneralCommand::LoadCertFile()
{
    LOGI("General Command ::LoadCertFile");
    bool is_validate = true;
    if(!this->m_pclArg->cert_file.empty())
    {
        try
        {
            m_pclApp->LoadCertFile(m_clApKey,this->m_pclArg->cert_file);
        }
        catch(const BaseException& exception)
        {
            is_validate = false;
            LOGE(exception.err_msg().c_str());
        }
    }
    return is_validate;
}

bool GeneralCommand::LoadAuthFile()
{
    LOGI("General Command ::LoadAuthFile");
    bool is_validate = true;
    if(!this->m_pclArg->auth_file.empty())
    {
        try
        {
            m_pclApp->LoadAuthFile(m_clApKey,this->m_pclArg->auth_file);
        }
        catch(const BaseException& exception)
        {
            is_validate = false;
            LOGE(exception.err_msg().c_str());
        }
    }
    return is_validate;
}

}
