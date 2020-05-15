#ifndef COREHANDLE_H
#define COREHANDLE_H

#include <iostream>
#include "../BootRom/flashtool_handle.h"
#include "../Utility/Utils.h"

namespace APCore
{
class DAHandle;
class DLHandle;
class RBHandle;
class ScretHandle;
class AuthHandle;

class CoreHandle
{
public:
    CoreHandle();
    ~CoreHandle();

    void LoadDA(const std::string &da_file);
    int LoadScatter(DL_SCATTER_TYPE type,
                     const std::string &scat_file);
    bool LoadRomFile(const std::string &rom_file, const int rom_index);
    void LoadSecurityFile(const std::string &sect_file);
    void LoadAuthFile(const std::string &auth_file);
    int AutoLoadRomImages(const std::string &scat_file);
    bool GetScatterVersion(char* version);
    bool GetScatterInfo(SCATTER_Head_Info *p_info);
    bool IsComboSCIDL();
    int CheckSecUnlock(const int index);

    DAHandle *da_handle() ;
    DLHandle *dl_handle() ;
    RBHandle *rb_handle() const { return rb_handle_; }
    ScretHandle *sc_handle() const { return sc_handle_;}
    AuthHandle *auth_handle() const { return auth_handle_;}

private:
    DAHandle *da_handle_;
    DLHandle *dl_handle_;
    RBHandle *rb_handle_;
    ScretHandle *sc_handle_;
    AuthHandle *auth_handle_;
};

}

#endif // COREHANDLE_H
