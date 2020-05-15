#ifndef USBCOMFINDERWRAPPER_H
#define USBCOMFINDERWRAPPER_H
#include "../BootRom/com_enum.h"

#ifdef _WIN32
#include <windows.h>
#else
#include "../Linux/linux_flash_tool.h"
#endif
#include <string>
//#include <set>
#include <list>

using std::list;
using std::string;
//using std::set;

class USBCOMFinderWrapper {
public:
#if 0
    const static std::string PID_PRELOADER;
    const static std::string VID_PRELOADER;
    const static std::string PID_BROM;
    const static std::string VID_BROM;
#endif
    static int getComPortInfo(unsigned short *p_com_port_array, unsigned short *p_com_port_count);
    static int getComPortInfoStrs(std::list<std::string> &com_port_strs);
#if 0
    static void ScanExistUSBVComPort( std::set<string>  &exist_symbollic_str,
                                      std::set<string> &exist_port_str,
                                      const std::string& pid,
                                      const std::string& vid);
    static bool GetNewCreatedUSBVComPort(string &usb_symbolic_str,
                                         string &usb_port_str,
                                         std::set<string> &exist_com_port_str,
                                         const std::string& pid,
                                         const std::string& vid);
#endif
private:
    USBCOMFinderWrapper();
};

#endif // USBCOMFINDERWRAPPER_H
