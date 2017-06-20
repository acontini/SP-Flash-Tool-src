#ifndef CONNBROMUSBSETTING_H
#define CONNBROMUSBSETTING_H

#include "ConnSetting.h"
#include "../Arg/BootRomArg.h"

namespace APCore
{

enum USBSpeed{
    USBHighSpeed,
    USBFullSpeed
};

enum USBPower{
    AutoDetect,
    WithBattery,
    WithoutBattery
};

class ConnBromUSBSetting : public ConnSetting
{
public:
    ConnBromUSBSetting(USBSpeed speed, USBPower power);
    ConnBromUSBSetting(const XML::Node &node);

    virtual Connection *CreateConnection(
            APKey key, HW_StorageType_E stor, bool pwr_key_reset);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    void set_cb_dl_da_init(
            CALLBACK_WRITE_BUF_PROGRESS_INIT cb_dl_da_init)
    {
        cb_download_da_init_ = cb_dl_da_init;
    }

    void set_cb_dl_da(
            CALLBACK_WRITE_BUF_PROGRESS cb_dl_da)
    {
        cb_download_da_ = cb_dl_da;
    }

    void set_cb_checkusbstatus_init(
            CALLBACK_USB_STATUS_INIT cb_usb_stat_init)
    {
        cb_usb_stat_init_ = cb_usb_stat_init;
    }

    DA_LOG_LEVEL_E get_da_log_level()
    {
        return m_da_log_level;
    }

    void set_da_log_level(unsigned int log_level)
    {
        m_da_log_level = (DA_LOG_LEVEL_E)log_level;
    }

private:
    USBSpeed speed_;
    USBPower power_;
    DA_LOG_LEVEL_E m_da_log_level;

    CALLBACK_WRITE_BUF_PROGRESS_INIT cb_download_da_init_;
    CALLBACK_WRITE_BUF_PROGRESS      cb_download_da_;
    CALLBACK_USB_STATUS_INIT         cb_usb_stat_init_;
};

}

#endif // CONNBROMUSBSETTING_H
