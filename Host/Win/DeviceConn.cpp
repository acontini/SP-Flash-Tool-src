/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001 ~ 2013
*
*****************************************************************************/
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
#include <string>
#include <assert.h>

#include "../../BootRom/com_enum.h"
#include "../../BootRom/brom.h"
#include "../Inc/DeviceConn.h"

#define IsInterface(guid)  \
    strcmp(guid, "port-guid-interface")

bool FindUSBDevice(const USBScanArg *p_arg, USBScanResult *p_result, bool forbidStop)
{
    assert(NULL != p_arg && NULL != p_result);

    bool is_found(false);
    COM_FILTER_LIST_S com_filter;
    COM_PROPERTY_S com_property;
    com_filter.m_uCount = p_arg->ports;
    com_filter.m_eType = WHITE_LIST;
    memset(&com_property, 0, sizeof(COM_PROPERTY_S));
    char **pp_filter = NULL;
    bool guids[10];

    std::vector<char*> buffer(com_filter.m_uCount);
    pp_filter = static_cast<char**>(&buffer[0]);

    std::string pid_vid_str;
    std::vector<std::string> pid_vid_list;
    for (size_t i = 0; i < p_arg->ports; i++) {
        pid_vid_str = std::string("VID_")
                    + p_arg->p_usb_id_set[i].usb_vid
                    + std::string("&PID_")
                    + p_arg->p_usb_id_set[i].usb_pid;
        pid_vid_list.push_back(pid_vid_str);
        pp_filter[i] = const_cast<char*>(pid_vid_list[i].c_str());

        if(IsInterface(p_arg->p_usb_id_set[i].guid_type) == 0)
            guids[i] = true;
        else
            guids[i] = false;
    }
    com_filter.m_ppFilterID = pp_filter;
    com_filter.m_bInterface = guids;

    int no_stop = 0;
    int ret = GetCOMPortWithFilter(&com_filter,
                                   &com_property,
                                   forbidStop? &no_stop: p_result->p_stop_flag,
                                   forbidStop? 30 : (p_arg->time_out)/1000);

    if(S_DONE == ret)
    {
        strcpy(p_result->p_friendly_name, com_property.m_rFriendly);
        strcpy(p_result->p_symbolic_name, com_property.m_rSymbolic);
        is_found = true;
    }

    return is_found;
}

bool FindSpecialUSBDevice(const USBScanArg *p_arg, USBScanResult *p_result, int prefer_com_port)
{
    assert(NULL != p_arg && NULL != p_result);

    bool is_found(false);
    COM_FILTER_LIST_S com_filter;
    COM_PROPERTY_S com_property;
    com_filter.m_uCount = p_arg->ports;
    com_filter.m_eType = WHITE_LIST;
    memset(&com_property, 0, sizeof(COM_PROPERTY_S));
    char **pp_filter = NULL;
    bool guids[10];

    std::vector<char*> buffer(com_filter.m_uCount);
    pp_filter = static_cast<char**>(&buffer[0]);

    std::string pid_vid_str;
    std::vector<std::string> pid_vid_list;
    for (size_t i = 0; i < p_arg->ports; i++) {
        pid_vid_str = std::string("VID_")
                    + p_arg->p_usb_id_set[i].usb_vid
                    + std::string("&PID_")
                    + p_arg->p_usb_id_set[i].usb_pid;
        pid_vid_list.push_back(pid_vid_str);
        pp_filter[i] = const_cast<char*>(pid_vid_list[i].c_str());

        if(IsInterface(p_arg->p_usb_id_set[i].guid_type) == 0)
            guids[i] = true;
        else
            guids[i] = false;
    }
    com_filter.m_ppFilterID = pp_filter;
    com_filter.m_bInterface = guids;

    int ret = GetSpecialCOMPortWithFilter(&com_filter,
                                          prefer_com_port,
                                          &com_property,
                                          p_result->p_stop_flag,
                                          (p_arg->time_out)/1000);

    if(S_DONE == ret)
    {
        strcpy(p_result->p_friendly_name, com_property.m_rFriendly);
        strcpy(p_result->p_symbolic_name, com_property.m_rSymbolic);
        is_found = true;
    }

    return is_found;
}
