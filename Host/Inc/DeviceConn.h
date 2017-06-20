/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001 ~ 2013
*
*****************************************************************************/

#include <stddef.h>

#ifndef _DEVICE_CONN_H
#define _DEVICE_CONN_H

typedef struct {
    const char *usb_pid;
    const char *usb_vid;
    const char *guid_type;
} USBID;

typedef struct {
    const USBID *p_usb_id_set;
    size_t ports;
    int time_out;
} USBScanArg;

typedef struct {
    char p_friendly_name[256];
    char p_symbolic_name[256];
    int *p_stop_flag;
} USBScanResult;

bool FindUSBDevice(const USBScanArg *p_arg, USBScanResult *p_result, bool forbidStop);
bool FindSpecialUSBDevice(const USBScanArg *p_arg, USBScanResult *p_result, int prefer_com_port);

#endif
