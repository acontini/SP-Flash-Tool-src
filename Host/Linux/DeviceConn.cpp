/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001 ~ 2013
*
*****************************************************************************/

#include <vector>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <qglobal.h>

#include "../Inc/DeviceConn.h"
#include "DeviceScan.h"

bool FindUSBDevice(const USBScanArg *p_arg, USBScanResult *p_result, bool forbidStop)
{
    assert(NULL != p_arg && NULL != p_result);
    bool is_found(false);
    char port_name[128] = {0};
    std::vector<USB_DEVICE_INFO> buffer(p_arg->ports);
    USB_DEVICE_INFO *p0 = static_cast<USB_DEVICE_INFO*>(&buffer[0]);

    for ( size_t i = 0; i < p_arg->ports; i++) {
        p0[i].vid = strtol(p_arg->p_usb_id_set[i].usb_vid, NULL, 16);
        p0[i].pid = strtol(p_arg->p_usb_id_set[i].usb_pid, NULL, 16);
    }

    DeviceScan scan(p0);
    int no_stop = 0;
    if (scan.FindDeviceUSBPort(p_arg->ports, port_name,
                               forbidStop?&no_stop:p_result->p_stop_flag,
                               forbidStop?30000:p_arg->time_out))
    {
        strcpy(p_result->p_friendly_name, port_name);
        strcpy(p_result->p_symbolic_name,port_name);
        is_found = true;
    }

    return is_found;
}

bool FindSpecialUSBDevice(const USBScanArg *p_arg, USBScanResult *p_result, int prefer_com_port)
{
    Q_UNUSED(p_arg);
    Q_UNUSED(p_result);
    Q_UNUSED(prefer_com_port);
    return false;
}
