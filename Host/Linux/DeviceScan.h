#ifndef DEVICESCANTHREAD_H
#define DEVICESCANTHREAD_H

#include <time.h>

typedef struct __USB_DEVICE_INFO
{
    short vid;
    short pid;

} USB_DEVICE_INFO;

class DeviceScan
{
public:
    explicit DeviceScan(USB_DEVICE_INFO* info);
    ~DeviceScan();
    bool FindDeviceUSBPort(size_t ports_count, char *portName, int* p_stop_flag, const int& d_time_out);

    int init_hotplug_sock();
    bool GetDeviceInfo(size_t ports_count, const char *buf, char *portName);

    bool VerifyDeviceInfo(size_t ports_count, const USB_DEVICE_INFO *inst);

    bool WaitForDeviceReady(const char *path);

    void GetDeviceID();

private:
    USB_DEVICE_INFO* usb_info_;
    time_t start_time_;
    time_t end_time_;
    int hotplug_sock_;
};

#endif // DEVICESCANTHREAD_H
