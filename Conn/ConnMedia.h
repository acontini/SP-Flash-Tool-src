/*
 * Connection.h
 *
 *  Created on: Aug 27, 2011
 *      Author: MTK81019
 */

#ifndef _CONN_MEDIA_H_
#define _CONN_MEDIA_H_

#include <string>
#include "../BootRom/flashtool.h"

namespace APCore
{

class ConnArgBox;

class ConnMedia
{
public:
    virtual ~ConnMedia()
    {
    }

    const std::string &Path() const
    {
        return dev_path_;
    }

    const std::string &FriendlyName() const
    {
        return friendly_name_;
    }

    int USBStatus() const
    {
        return usb_status_;
    }

    void setFriendlyName(std::string name)
    {
        friendly_name_ = name;
    }

    void setHighSpeedFriendlyName(std::string name)
    {
        friendly_name_h_ = name;
    }

public:
    virtual int PreConnect(ConnArgBox*) = 0;
    virtual int PostConnect(ConnArgBox*) = 0;

protected:
    std::string dev_path_;
    std::string friendly_name_;
    std::string friendly_name_h_;
    int usb_status_;
};

class ConnMediaUART : public ConnMedia
{
public:
    ConnMediaUART(const std::string &path)
    {
        dev_path_ = path;
    }

    virtual int PreConnect(ConnArgBox*)
    {
        return S_DONE;
    }

    virtual int PostConnect(ConnArgBox*)
    {
        return S_DONE;
    }
};

class ConnMediaUSB : public ConnMedia
{
public:
    ConnMediaUSB(bool hi_speed)
        : to_hi_speed_(hi_speed)
    {
    }

    virtual int PreConnect(ConnArgBox*);
    virtual int PostConnect(ConnArgBox*);

protected:
    bool Search(ConnArgBox*);
    int  GetUsbStatus(const FLASHTOOL_API_HANDLE_T p_ft_handle);

protected:
    bool to_hi_speed_;
};

} /* namespace APCore */
#endif /* _CONN_MEDIA_H_ */
