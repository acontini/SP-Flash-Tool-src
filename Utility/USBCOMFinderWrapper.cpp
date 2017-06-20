#include "USBCOMFinderWrapper.h"
#include "../BootRom/com_enum.h"
#include "../Host/Inc/RuntimeMemory.h"
#include <algorithm>

USBCOMFinderWrapper::USBCOMFinderWrapper()
{

}

int USBCOMFinderWrapper::getComPortInfo(unsigned short *p_com_port_array, unsigned short *p_com_port_count)
{
    int ret = 0;
    ret = ComPortEnumerate(p_com_port_array, p_com_port_count);
    return ret;
}

int USBCOMFinderWrapper::getComPortInfoStrs(std::list<std::string> &com_port_strs)
{
    com_port_strs.clear();
    unsigned short n_max = 20;
    COM_ENUM_PORT* ports = new COM_ENUM_PORT[n_max];
    memset(ports, 0x00, n_max *sizeof(COM_ENUM_PORT));
    COM_ENUM_ARG arg;
    memset(&arg,0x00,sizeof(arg));
    int ret = ComPortEnumerateByArg(&arg, ports, &n_max);

    if(ret == COM_ENUM_COM_ARRAY_TOO_SMALL)
    {
        delete [] ports;

#define MAX_EXPAND_PORT_NUM 10
        n_max += MAX_EXPAND_PORT_NUM;
        ports = new COM_ENUM_PORT[n_max];
        memset(ports, 0x00, n_max *sizeof(COM_ENUM_PORT));
        ret = ComPortEnumerateByArg(&arg, ports, &n_max);
    }

    if(ret == COM_ENUM_OK)
    {
        for (int idx = 0; idx < n_max; idx++)
            com_port_strs.push_back(ports[idx].name);
    }

    delete [] ports;

    return ret;
}
#if 0
/************just for windows, we need to implement it for multi platform begin*************************/
bool USBCOMFinderWrapper::GetNewCreatedUSBVComPort(string &usb_com_port_str,
                                           string &usb_port_str,
                                           std::set<string> &exist_com_port_str,
                                                   const std::string& pid,
                                                   const std::string& vid) {
    std::set<string> current_exist_symbolic_str;
    std::set<string> current_exist_port_str;
    set <string>::size_type current_com_port_size;
    std::set<string>::iterator it_current_com_port;

    std::list<string>   result_port_str_test;
    std::list<string>::iterator  it_last_result_str;

    //Get current exist com port
    ScanExistUSBVComPort(current_exist_symbolic_str, current_exist_port_str,pid,vid);

    it_current_com_port = current_exist_symbolic_str.begin();
    current_com_port_size = current_exist_symbolic_str.size();

    if ( 1 == current_com_port_size )
    {
        usb_com_port_str = *it_current_com_port;
        usb_port_str = *(current_exist_port_str.begin());
        return true;
    }

    it_last_result_str = set_difference(current_exist_symbolic_str.begin(),
                                        current_exist_symbolic_str.end(),
                                        exist_com_port_str.begin(),
                                        exist_com_port_str.end(),
                                        result_port_str_test.begin());


    if(result_port_str_test.begin() == it_last_result_str)
    {
        return false;
    }

    usb_com_port_str =  *it_last_result_str;
    return true;
}

void USBCOMFinderWrapper::ScanExistUSBVComPort( std::set<string>  &exist_symbollic_str,
                                                std::set<string> &exist_port_str,
                                                const std::string& pid,
                                                const std::string& vid)
{
#ifdef WIN32

    const int DEV_PATH_BUFSIZE = 512;
    const int PORT_NAME_BUFSIZE = 512;

    unsigned long    device_number = 0;
    char        device_path[DEV_PATH_BUFSIZE];
    unsigned long    device_path_length;
    char        port_name[PORT_NAME_BUFSIZE];
    unsigned long    port_name_real_length;


    device_number = GetDeviceNumber(InterfaceClassGuidConstant, pid.c_str(), vid.c_str());

    if( device_number == 0) {
        return;
    }

    for(unsigned int i=0; i<device_number; i++)
    {
        if(GetPresentDevicePath(InterfaceClassGuidConstant, pid.c_str(), vid.c_str(), i,
            port_name, PORT_NAME_BUFSIZE, (unsigned long *) &port_name_real_length,
            device_path, DEV_PATH_BUFSIZE, (unsigned long *) &device_path_length))
        {
            exist_symbollic_str.insert(device_path);
            exist_port_str.insert(port_name);

        }
        else
        {
        }
    }
#else
    char portName[PORTNAMELENGTH] = {0};
    if(GetUSBPortName(pid.c_str(),vid.c_str(),portName))
    {
        LOGI("\nFind USB in linux.....%s", portName);
        exist_symbollic_str.insert(portName);
        exist_port_str.insert(portName);
    }

    return;

#endif
}
/************just for windows, we need to implement it for multi platform end*************************/
#endif
