#ifndef AUTHHANDLE_H
#define AUTHHANDLE_H

#include "../../BootRom/flashtool_handle.h"
#include "../../BootRom/brom.h"
#include "SuperHandle.h"
#include <string>

namespace APCore{

class AuthHandle : public SuperHandle<AUTH_HANDLE_T>
{
public:
    AuthHandle();
    ~AuthHandle();

    void LoadAuthFile(const std::string &auth_file);

    bool IsReady();
};

}

#endif // AUTHHANDLE_H
