/*
 * Connection.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: MTK81019
 */

#include "Connection.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

// class Connection
const std::string &Connection::Device() const
{
    return p_media_->Path();
}

const std::string &Connection::FriendlyName() const
{
    return p_media_->FriendlyName();
}

int Connection::USBStatus() const
{
    return p_media_->USBStatus();
}

FLASHTOOL_API_HANDLE_T Connection::FTHandle() const
{
    return p_arg_box_->FTHandle();
}

APKey Connection::ap_key() const
{
    return p_arg_box_->ap_key();
}

DA_REPORT_T &Connection::da_report()
{
    if (connected_<= 1)
        THROW_BROM_EXCEPTION(S_DA_HANDLE_IS_NOT_READY, 0);

    DAConnection *p =
            (DAConnection*)p_inner_conn_.data();

    return p->Result();
}

const BOOT_RESULT &Connection::boot_result()
{
    if (connected_<= 0)
        THROW_BROM_EXCEPTION(S_DA_HANDLE_IS_NOT_READY, 0);

    return *FlashTool_GetBootResult(FTHandle());
}

int Connection::ConnectBROM()
{
    int ret = S_DONE;

    if (0 == connected_)
    {
        p_inner_conn_ = _SHARED_PTR<ConnBase>(
                new BromConnection());

        LOGI("Connecting to BROM...");

        ret = p_inner_conn_->Connect(
                    p_media_.data(),
                    p_arg_box_.data());

        if (S_DONE == ret)
        {
            ++ connected_;
            NotifyBromConnected(boot_result(), p_media_.data()->FriendlyName());
            LOGI("BROM connected");
        }
        else
        {
            LOGE("Connect BROM failed: %s", StatusToString(ret));
            THROW_BROM_EXCEPTION(ret, 0);
        }
    }
    else if (connected_ > 1)
    {
        LOGE("DA already connected, BROM no longer available");
        THROW_BROM_EXCEPTION(S_BROM_CONNECT_TO_BOOTLOADER_FAIL, 0);
    }

    return ret;
}

int Connection::ConnectDA(CONN_DA_END_STAGE stage, _BOOL enableDRAMin1stDA)
{
    int ret = S_DONE;

    if (0 == connected_)
    {
        ret = ConnectBROM();
    }

    if (1 == connected_)
    {
        p_inner_conn_ = _SHARED_PTR<ConnBase>(
                new DAConnection());

        LOGI("Downloading & Connecting to DA...");

        p_arg_box_->SetConnDAEndStage(stage);
        p_arg_box_->Set1stDAEnableDRAM(enableDRAMin1stDA);
        LOGI("connect DA end stage: %d, enable DRAM in 1st DA: %d", stage, enableDRAMin1stDA);

        ret = p_inner_conn_->Connect(
                    p_media_.data(),
                    p_arg_box_.data());

        if (S_DONE == ret)
        {
            ++ connected_;
            NotifyDAConnected(da_report(), p_media_.data()->FriendlyName(),
                              p_media_.data()->USBStatus());
            LOGI("DA Connected");
        }
        else
        {
            LOGE("Failed to Connect DA: %s", StatusToString(ret));
            THROW_BROM_EXCEPTION(ret, 0);
        }
    }

    return ret;
}

int Connection::Disconnect()
{
    LOGI("Disconnect!");
    int ret = S_DONE;

    if (p_inner_conn_ != NULL)
    {
        ret = p_inner_conn_->Disconnect(
                    p_arg_box_.data());

        if (S_DONE == ret)
        {
            connected_ = 0;
            /*
            NotifyDisconnected();
            */
        }
    }
    return ret;
}

void Connection::NotifyBromConnected(
        const BOOT_RESULT &res,
        const std::string &friend_name) const
{
    _SHR_OBSERVER_ITERC it = observer_list_.begin();

    while (it != observer_list_.end())
    {
        (*it)->OnBromConnected(res, friend_name);
        ++ it;
    }
}

void Connection::NotifyDAConnected(
        const DA_REPORT_T &res,
        const std::string &friend_name,
        const int usb_status) const
{
    _SHR_OBSERVER_ITERC it = observer_list_.begin();

    while (it != observer_list_.end())
    {
        (*it)->OnDAConnected(res, friend_name, usb_status);
        ++ it;
    }
}

void Connection::setPowerType(int force_chg)
{
    p_arg_box_->SetPowerType(force_chg);
}
} /* namespace APCore */
