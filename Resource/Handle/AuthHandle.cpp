#include "AuthHandle.h"

#include "../../Logger/Log.h"
#include "../../Err/Exception.h"

namespace APCore{

AuthHandle::AuthHandle()
{
    AUTH_Create(&handle_t_);
    LOG("ctor of authHandle");
}

AuthHandle::~AuthHandle()
{
    if(handle_t_ != NULL)
    {
        AUTH_Unload(handle_t_);
        AUTH_Destroy(&handle_t_);
        handle_t_ = NULL;
    }
    LOG("dtor of auth handle.");
}

void AuthHandle::LoadAuthFile(const std::string &auth_file)
{
    int ret;

    ret = AUTH_Unload(handle_t_);
    if(ret != S_DONE)
    {
        LOG("Unload before LoadAuthFile failed(%d)", ret);
        THROW_BROM_EXCEPTION(ret, 0);
    }

    ret = AUTH_Load(handle_t_, auth_file.c_str());

    if(ret != S_DONE)
    {
        LOG("Load auth file failed(%d), ret");
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

bool AuthHandle::IsReady()
{
    AUTH_INFO auth_info;

    if(S_DONE == AUTH_IsReady(handle_t_, &auth_info, _TRUE))
    {
        return true;
    }
    else
    {
        return false;
    }
}

}
