#include "ScretHandle.h"
#include "../../Logger/Log.h"
#include "../../Err/Exception.h"

namespace APCore {


ScretHandle::ScretHandle()
{
    SCERT_Create(&handle_t_);
    LOG("ctor of ScretHandle");
}

ScretHandle::~ScretHandle()
{
    if(handle_t_ != NULL)
    {
        SCERT_Unload(handle_t_);
        SCERT_Destroy(&handle_t_);
        handle_t_ = NULL;
    }

    LOG("dtor of ScretHandle");
}

void ScretHandle::LoadSecurityFile(const std::string &security_file)
{
    int ret;

    ret = SCERT_Unload(handle_t_);

    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }

    ret = SCERT_Load(handle_t_, security_file.c_str());

    if(ret != S_DONE)
    {
        if(ret == S_FTHND_HANDLE_BUSY_NOW)
            LOG("Program is busy! Cannot reload CERT file right now.");
        else
            LOG("Load CERT file failed.");

        THROW_BROM_EXCEPTION(ret, 0);
    }
}

bool ScretHandle::IsReady()
{
    if(handle_t_ == NULL)
    {
        return false;
    }

    SCERT_INFO scert_info;

    int ret = SCERT_IsReady(handle_t_, &scert_info, _TRUE);
    if(ret != S_DONE)
    {
        LOG("DA_IsReady failed(%d)",ret);
        return false;
    }

    return true;
}

}
