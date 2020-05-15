#include "CoreHandle.h"

#include "Handle/DAHandle.h"
#include "Handle/DLHandle.h"
#include "Handle/RBHandle.h"
#include "Handle/ScretHandle.h"
#include "Handle/AuthHandle.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Err/BromException.h"
#include "../Err/Exception.h"

#define _LAZY_CREATE(v, T)  \
    (v) = (NULL==(v)) ? new T() : (v)

namespace APCore
{

CoreHandle::CoreHandle()
{
    da_handle_ = NULL;
    dl_handle_ = NULL;
    rb_handle_ = new RBHandle();
    sc_handle_ = new ScretHandle();
    auth_handle_ = new AuthHandle();
}

CoreHandle::~CoreHandle()
{
    if(da_handle_ != NULL)
    {
        delete da_handle_;
    }
    if(dl_handle_ != NULL)
    {
        delete dl_handle_;
    }
    if(rb_handle_ != NULL)
    {
        delete rb_handle_;
    }
    if(sc_handle_ != NULL)
    {
        delete sc_handle_;
    }
    if(auth_handle_ != NULL)
    {
        delete auth_handle_;
    }
}

void CoreHandle::LoadDA(const std::string & da_file)
{
    try{
        if (!da_file.empty())
        {
            da_handle()->LoadDA(da_file);
        }
    }catch(BromException e)
    {
        THROW_BROM_EXCEPTION_EX(e);
    }
}

DLHandle *CoreHandle::dl_handle()
{
    _LAZY_CREATE(dl_handle_, DLHandle);
    return dl_handle_;
}

DAHandle *CoreHandle::da_handle()
{
    _LAZY_CREATE(da_handle_, DAHandle);
    return da_handle_;
}

int CoreHandle::CheckSecUnlock(const int index)
{
    return dl_handle()->CheckSecUnlock(index);
}

int CoreHandle::AutoLoadRomImages(const std::string &scat_file)
{
    if (!scat_file.empty())
    {
        return dl_handle()->AutoLoadRomImages(scat_file);
    }
    return S_UNDEFINED_ERROR;
}

bool CoreHandle::GetScatterVersion(char* version)
{
    return dl_handle_->GetScatterVersion(version);
}

bool CoreHandle::GetScatterInfo(SCATTER_Head_Info *p_info)
{
    return dl_handle_->GetScatterInfo(p_info);
}

bool CoreHandle::IsComboSCIDL(){
    return dl_handle_->IsComboSCIDL();
}

int CoreHandle::LoadScatter(
        DL_SCATTER_TYPE type,
        const std::string & scat_file)
{
    if (!scat_file.empty())
    {
        switch(type)
        {
        case NORMAL_SCATTER:
            return dl_handle()->LoadScatter(scat_file);

        case SCI_SCATTER:
            return dl_handle()->LoadSCIScatter(scat_file);

        default:
            break;
    }
    }

    return S_UNDEFINED_ERROR;
}

bool CoreHandle::LoadRomFile(
        const std::string & rom_file,
        const int rom_index)
{
    if (!rom_file.empty())
    {
        return dl_handle()->LoadRomFile(rom_file,rom_index);
    }
    return false;
}

void CoreHandle::LoadSecurityFile(const std::string &sect_file)
{
    sc_handle_->LoadSecurityFile(sect_file);
}

void CoreHandle::LoadAuthFile(const std::string &auth_file)
{
    try{
        auth_handle_->LoadAuthFile(auth_file);
    }catch(const BromException& e)
    {
        THROW_BROM_EXCEPTION_EX(e);
    }
}

}
