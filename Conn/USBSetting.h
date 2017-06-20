#ifndef USBSETTING_H
#define USBSETTING_H

#include <iostream>
#include <list>
#include <map>
#include <qsharedpointer.h>

#include "../Host/Inc/DeviceConn.h"

typedef enum
{
    NORMAL_SPEED,
    DA_HIGH_SPEED
}usb_speed;

typedef struct
{
    std::string device_pid;
    std::string device_vid;
    std::string guid_type;
}USBInfoPair;

typedef enum
{
    BOOTROM_VCOM,
    PRELOADER_VCOM,
    DA_HIGHSPEED_VCOM,
    COMPOSITE_PRELOADER_VCOM,
    COMPOSITE_DA_VCOM
}USB_ID_TYPE;

class USBSetting
{
public:
    static USBSetting *instance(bool CertDL = false);

    bool Search(int * stop_flag, int timeout);

    const std::string& symbolic(){return usb_device_path_;}
    const std::string& friendly(){return usb_friendly_name_;}
    void AddUSBPool(usb_speed speedType);
    void SetPreferComPort (int com_port){prefer_com_port_ = com_port;}
    void SetForibStop(bool forbid){forbid_stop_ = forbid;}

private:
    USBSetting(const std::string &setting_file, bool CertDL);
    void AddUSBInstance(USBInfoPair info);
    bool SearchUSBPortPool(USBScanArg *usb_scan_arg,
                           USBScanResult *usb_scan_result);

    std::list<USBInfoPair> usb_port_pool_;

    std::map<int, USBInfoPair>usbID_map_;

    bool is_certDL;

    std::string usb_device_path_;
    std::string usb_friendly_name_;

    int prefer_com_port_;
    bool forbid_stop_;
    //static USBSetting *inst;
    static QSharedPointer <USBSetting> inst;
};

#endif // USBSETTING_H
