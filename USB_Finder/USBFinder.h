#ifndef _USBFINDER_H_
#define _USBFINDER_H_

#if __GNUC__ >= 4
#define LIBEXPORT extern"C" __attribute__ ((visibility ("default")))
#else
#error  "the compiler is not supported!!"
#endif

#define PORTNAMELENGTH 128
#define PORTNUMBER 128

typedef struct __USB_DEVICE_INFO
{
    short vid;
    short pid;

} USB_DEVICE_INFO;


LIBEXPORT bool FindUSBPort(const USB_DEVICE_INFO info[], char *portName);

#endif
