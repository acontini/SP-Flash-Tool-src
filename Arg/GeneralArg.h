#ifndef GENERALARG_H
#define GENERALARG_H
#include "../BootRom/DOWNLOAD.H"
#include <string>
#include <list>

namespace ConsoleMode
{

class RomItem
{
public:
    RomItem():path(),index(0),enable(false){}

    std::string path;
    unsigned int index;
    bool enable;
};

class GeneralArg
{
public:
    GeneralArg();
    GeneralArg(const GeneralArg& arg);
    GeneralArg *Clone();

    std::string da_file;
    std::string scatter_file;
    std::string auth_file;
    std::string cert_file;
    std::string chip_name;
    HW_StorageType_E storage_type;
    std::list<RomItem> rom_list;

    GeneralArg& operator=(const GeneralArg& arg);
};

}
#endif // GENERALARG_H
