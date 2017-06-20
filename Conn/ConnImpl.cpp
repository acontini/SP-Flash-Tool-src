/*
 * ConnImpl.cpp
 *
 */

#include "ConnImpl.h"
#include "ConnMedia.h"
#include "ConnArgBox.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Utility/Utils.h"

namespace APCore
{
/////////////////////////////////////////////////////
// class BromConnection
BromConnection::~BromConnection()
{
}

int BromConnection::Disconnect(
        ConnArgBox *box)
{
    FLASHTOOL_API_HANDLE_T handle = box->FTHandle();

    int ret = FlashTool_Disconnect_BROM(&handle);

    if (S_DONE == ret)
    {
        box->SetFTHandle(handle);
    }

    return ret;
}

int BromConnection::Connect(
        ConnMedia *med,
        ConnArgBox *box)
{
    int ret = med->PreConnect(box);

    if (S_DONE == ret)
    {
        FLASHTOOL_API_HANDLE_T handle = box->FTHandle();

        if(!med->Path().empty())
        {
        ret = FlashTool_Connect_BROM_ByName(
                    med->Path().c_str(),
                    box->FTConnArg(),
                    &handle,
                    box->StopFlag());
        }
        else
        {
            ret = FlashTool_Connect_BROM(Utils::GetPortNumFromStr(med->FriendlyName().c_str()),
                                         box->FTConnArg(),
                                         &handle,
                                         box->StopFlag());
        }

        if (S_DONE == ret)
        {
            box->SetFTHandle(handle);
        }
    }

    return ret;
}

// class DAConnection
DAConnection::~DAConnection()
{
}

int DAConnection::Disconnect(
        ConnArgBox *box)
{
    FLASHTOOL_API_HANDLE_T handle = box->FTHandle();

    int ret = FlashTool_Disconnect(&handle);

    if (S_DONE == ret)
    {
        box->SetFTHandle(handle);
    }

    return ret;
}

int DAConnection::Connect(
        ConnMedia *med,
        ConnArgBox *box)
{
    FLASHTOOL_API_HANDLE_T handle = box->FTHandle();

    int ret = FlashTool_Connect_Download_DA(
                box->FTConnArg(),
                &handle,
                &conn_result_,
                box->StopFlag());

    if (S_DONE == ret)
    {
        box->SetFTHandle(handle);
        ret = med->PostConnect(box);
    }

    return ret;
}

} /* namespace APCore */
