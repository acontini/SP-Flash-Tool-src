#ifndef SCRETHANDLE_H
#define SCRETHANDLE_H

#include "../../BootRom/flashtool_handle.h"
#include "SuperHandle.h"
#include <string>

namespace APCore {

class ScretHandle : public SuperHandle<SCERT_HANDLE_T>
{
public:
    ScretHandle();
    ~ScretHandle();

    void LoadSecurityFile(
            const std::string &security_file);

    bool IsReady();
};

}
#endif // SCRETHANDLE_H
