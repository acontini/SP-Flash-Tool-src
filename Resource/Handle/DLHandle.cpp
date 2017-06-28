#include "DLHandle.h"

#include "../../Logger/Log.h"
#include "../../Err/Exception.h"

namespace APCore
{

DLHandle::DLHandle()
{
    int ret = DL_Create(&handle_t_);
    LOG("DL_Create(%d)", ret);
}

DLHandle::~DLHandle()
{
    if(handle_t_ != NULL)
    {
        DL_Rom_UnloadAll(handle_t_);
        DL_Destroy(&handle_t_);
        handle_t_ = NULL;
    }
    LOG("dtor of DLHandle");
}

int DLHandle::AutoLoadRomImages(const std::string &scat_file)
{
    int ret = DL_AutoLoadRomImages(handle_t_, scat_file.c_str());

    if(ret != S_DONE)
    {
        LOG("DL_AutoLoad failed, ret = %d.", ret);
    }
    return ret;
}

int DLHandle::LoadScatter(const std::string &scat_file)
{
    int ret = DL_LoadScatter(handle_t_,scat_file.c_str(),NULL);
    if(ret != S_DONE && ret != S_STOP)
    {
        LOG("DL_LoadScatter failed, ret = %d.", ret);
    }
    return ret;
}

int DLHandle::LoadSCIScatter(const std::string &sci_file)
{
    int ret = DL_LoadSCIScatter(handle_t_, sci_file.c_str());

    if(S_DONE != ret)
    {
        LOG("DL_LoadSCIScatter failed, ret = %d.", ret);
    }

    return ret;
}

bool DLHandle::IsComboSCIDL()
{
    bool isCombo_sci = false;

    int ret = DL_IsComboSCI(handle_t_, isCombo_sci);

    if(ret != S_DONE)
    {
        LOG("Get SCI Image info failed. (%d)", ret);
        return false;
    }


    return isCombo_sci;
}

bool DLHandle::GetPlatformInfo(DL_PlatformInfo &info)
{
    int ret = DL_GetPlatformInfo(handle_t_, &info);
    if(ret != S_DONE)
    {
        LOG("Get Platform Infomation failed. (%d)", ret);
        return false;
    }
    else
    {
        //add for new ARCH lib
        if(info.m_bbchip_type == UNKNOWN_BBCHIP_TYPE)
        {
            SCATTER_Head_Info shi;
            DL_GetScatterInfo(handle_t_, &shi);
            strcpy(info.m_szBBChipType, shi.platform);
        }
        //end
        LOG("Platform Info: storage(%s), chip(%s)", info.m_szStorageType, info.m_szBBChipType);
        return true;
    }
}

bool DLHandle::GetScatterVersion(char* version)
{
    int ret = DL_GetScatterVersion(handle_t_, version);

    if(ret != S_DONE)
    {
        LOG("Get Scatter version failed. (%d)", ret);
        return false;
    }
    else
    {
        LOG("Scatter version(%s)", version);
        return true;
    }
}

bool DLHandle::GetScatterInfo(SCATTER_Head_Info *p_info)
{
    int ret = DL_GetScatterInfo(handle_t_, p_info);

    if(ret != S_DONE)
    {
        LOG("Get Scatter info failed. (%d)", ret);
        return false;
    }
    else
    {
        LOG("Scatter version(%s)", p_info->version);
        return true;
    }
}

bool DLHandle::LoadRomFile(const std::string &rom_file, const int rom_index)
{
    LOG("Loading Rom File: %s",rom_file.c_str());

    int ret;
    ret = DL_Rom_Unload(handle_t_, rom_index);
    if(ret != S_DONE)
    {
        LOG("Unload before LoadRomFile failed(%d)",ret);
        return false;
    }

    ret = DL_Rom_Load(handle_t_,rom_index,rom_file.c_str());
    if(ret != S_DONE)
    {
        LOG("LoadRomFile failed(%d)",ret);
        return false;
    }
    return true;
}

int DLHandle::CheckSecUnlock(const int rom_index)
{
    return DL_CheckSecUnlockSecro(handle_t_, rom_index);
}

std::list<ROM_INFO> DLHandle::GetAllRomInfo()
{
    unsigned short rom_count;
    ROM_INFO rom_info[MAX_LOAD_SECTIONS];

    int ret = DL_GetCount(handle_t_, &rom_count);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    ret = DL_Rom_GetInfoAll(handle_t_, rom_info, MAX_LOAD_SECTIONS);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    std::list<ROM_INFO> rom_info_list;
    for(unsigned int i=0; i<rom_count; i++)
    {
        rom_info_list.push_back(rom_info[i]);
    }

    return rom_info_list;
}

void DLHandle::EnableROM(const int rom_index, bool enable)
{
    int ret = DL_Rom_SetEnableAttr(handle_t_, rom_index, enable?_TRUE:_FALSE);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
}

void DLHandle::EnableROM(const std::string &rom_name, bool enable)
{
    int ret = DL_Rom_SetEnableAttrByRomName(handle_t_, rom_name.c_str(), enable?_TRUE:_FALSE);

    if(S_DONE != ret)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

 void DLHandle::EnableDAChksum(int level)
 {
     LOGD("set DL_SetChecksumLevel(%d).", level);
     int ret = DL_SetChecksumLevel(handle_t_, level);
     if (S_DONE != ret)
     {
        LOGE("DL_SetChecksumEnable() failed, error code(%d).", ret);
        THROW_BROM_EXCEPTION(ret,0);
     }
 }

 void DLHandle::SetStopLoadFlag(int* stop_flag)
 {
     LOGD("set DL_SetStopFlag(%d).",*stop_flag);
     int ret = DL_SetStopLoadFlag(handle_t_, stop_flag);
     if (S_DONE != ret)
     {
        LOGE("DL_SetStopFlag() failed, error code(%d).", ret);
        THROW_BROM_EXCEPTION(ret,0);
     }
 }
}
