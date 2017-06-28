/*
 * DAHandle.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: MTK81019
 */

#include "DAHandle.h"

#include "../../Logger/Log.h"
#include "../../Err/Exception.h"

namespace APCore {

DAHandle::DAHandle()
{
    DA_Create(&handle_t_);
    LOG("ctor of DAHandle");
}

DAHandle::~DAHandle()
{
    DA_Unload(handle_t_);
    DA_Destroy(&handle_t_);
    LOG("dtor of DAHandle");
}

void DAHandle::LoadDA(const std::string & da_file)
{
    int ret;

    ret = DA_Unload(handle_t_);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
    ret = DA_Load(handle_t_, da_file.c_str(), _FALSE, _FALSE);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

void DAHandle::GetDAInfo(DA_INFO *da_info)
{
    int ret;

    ret = DA_GetInfo(handle_t_,da_info);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

bool DAHandle::IsReady()
{
    if(handle_t_ == NULL)
    {
        return false;
    }

    DA_INFO da_info;
    int ret = DA_IsReady(handle_t_, &da_info, _TRUE);
    if(ret != S_DONE)
    {
        LOG("DA_IsReady failed(%d)",ret);
        return false;
    }
    return true;
}

} /* namespace APCore */
