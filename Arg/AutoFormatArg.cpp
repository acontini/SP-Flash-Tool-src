#include "AutoFormatArg.h"
#include "../Err/Exception.h"
#include "../Host/Inc/RuntimeMemory.h"

APCore::AutoFormatArg::AutoFormatArg():
format_start_address_(0),
format_length_(0)
{
}

APCore::AutoFormatArg::AutoFormatArg(const AutoFormatArg& ref)
{
    this->format_start_address_ = ref.format_start_address_;
    this->format_length_ = ref.format_length_;
}

APCore::AutoFormatArg& APCore::AutoFormatArg::operator =(const AutoFormatArg& ref)
{
    if (&ref != this) {
        this->format_start_address_ = ref.format_start_address_;
        this->format_length_ = ref.format_length_;
    }
    return *this;
}

APCore::AutoFormatArg::~AutoFormatArg() {
}

bool APCore::AutoFormatArg::QueryPartLayout(const QSharedPointer<Connection> &conn,
                                            const std::string part_name,
                                            unsigned int &addr, unsigned int &len)
{
    int ret = S_DONE;
    bool found = false;
    unsigned int rom_count = 0;
    PART_INFO* part_info = NULL;
    std::string name_str;


    ret = FlashTool_ReadPartitionCount(
                conn->FTHandle(), &rom_count);
    if(ret != S_DONE)
    {
        LOGE("FlashTool_ReadPartitionCount failed(%d)", ret);
        THROW_BROM_EXCEPTION(ret, 0);
    }

    part_info = new PART_INFO[rom_count];
    ret = FlashTool_ReadPartitionInfo(
                conn->FTHandle(), part_info, rom_count);
    if(ret != S_DONE)
    {
        LOGE("FlashTool_ReadPartitionInfo failed(%d)", ret);
        if(part_info != NULL)
        {
            delete part_info;
            part_info = NULL;
        }
        THROW_BROM_EXCEPTION(ret,0);
    }

    for(unsigned int i=0; i<rom_count; i++)
    {
        LOG("part[%d]: %s, addr(0x%llx), len(0x%llx)", i,
            part_info[i].name,
            part_info[i].begin_addr,
            part_info[i].image_length);

        name_str = part_info[i].name;

        if(name_str.find(part_name) != std::string::npos)
        {
            addr = part_info[i].begin_addr;
            len  = part_info[i].image_length;
            found = true;
            LOG("NVRAM partition found.");
            break;
        }
    }


    if(part_info != NULL)
    {
        delete part_info;
        part_info = NULL;
    }

    return found;
}


