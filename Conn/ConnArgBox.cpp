/*
 * ConnArgBox.cpp
 *
 */

#include "ConnArgBox.h"
#include "../Logger/Log.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Handle/DAHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/AuthHandle.h"
#include "../Resource/Handle/ScretHandle.h"


namespace APCore
{

// class ConnArgBox
ConnArgBox::ConnArgBox(
        APKey key,
        int *stop_flag,
        int timeout,
        HW_StorageType_E stor,
        const FlashTool_Connect_Arg &connArg)
    : key_(key), stop_flag_(stop_flag),timeout_(timeout),
      stor_(stor), conn_arg_(connArg),
      ft_handle_(NULL)
{
}

const FlashTool_Connect_Arg *ConnArgBox::FTConnArg()
{
    conn_arg_.m_boot_arg.m_da_handle = GET_DA_HANDLE_T(key_);
    conn_arg_.m_boot_arg.m_auth_handle = GET_AUTH_HANDLE_T(key_);
    conn_arg_.m_boot_arg.m_scert_handle = GET_CERT_HANDLE_T(key_);
    conn_arg_.m_p_dl_handle = GET_DL_HANDLE_T(key_);
    conn_arg_.m_storage_type = stor_;

    return &conn_arg_;
}

} /* namespace APCore */
