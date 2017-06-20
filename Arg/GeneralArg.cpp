#include "GeneralArg.h"
namespace ConsoleMode
{

GeneralArg::GeneralArg():da_file(),scatter_file(),auth_file(),cert_file(),chip_name(),storage_type(HW_STORAGE_NONE){}

GeneralArg::GeneralArg(const GeneralArg& arg)
{
    this->da_file = arg.da_file;
    this->scatter_file = arg.scatter_file;
    this->auth_file = arg.auth_file;
    this->cert_file = arg.cert_file;
    this->chip_name = arg.chip_name;
    this->storage_type = arg.storage_type;
    this->rom_list = arg.rom_list;
}
GeneralArg *GeneralArg::Clone()
{
    return new GeneralArg(*this);
}

std::string da_file;
std::string scatter_file;
std::string auth_file;
std::string cert_file;
std::string chip_name;
HW_StorageType_E storage_type;
std::list<RomItem> rom_list;

GeneralArg& GeneralArg::operator=(const GeneralArg& arg)
{
    this->da_file = arg.da_file;
    this->scatter_file = arg.scatter_file;
    this->auth_file = arg.auth_file;
    this->cert_file = arg.cert_file;
    this->chip_name = arg.chip_name;
    this->storage_type = arg.storage_type;
    this->rom_list = arg.rom_list;
    return (*this);
}
}
