#ifndef DOWNLOADHANDLE_H
#define DOWNLOADHANDLE_H

#include "../../BootRom/flashtool_handle.h"
#include "SuperHandle.h"
#include <string>
#include <list>

namespace APCore
{

class DLHandle : public SuperHandle<DL_HANDLE_T>
{
public:
    DLHandle();
    ~DLHandle();

    int AutoLoadRomImages(const std::string &scat_file);
    int LoadScatter(const std::string &scat_file);
    int LoadSCIScatter(const std::string &sci_file);
    bool LoadRomFile(const std::string &rom_file, const int rom_index);
    bool GetScatterVersion(char* version);
    bool GetScatterInfo(SCATTER_Head_Info *p_info);
    bool IsComboSCIDL();
    int  CheckSecUnlock(const int rom_index);
    bool GetPlatformInfo(DL_PlatformInfo &info);

    std::list<ROM_INFO> GetAllRomInfo();
    void EnableROM(const int rom_index, bool enable);
    void EnableROM(const std::string &rom_name, bool enable);
    void EnableDAChksum(int level);
    void SetStopLoadFlag(int* stop_flag);
};

}

#endif // DOWNLOADHANDLE_H
