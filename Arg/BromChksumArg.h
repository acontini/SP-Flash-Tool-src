#ifndef BROM_CHKSUM_ARG_H
#define BROM_CHKSUM_ARG_H

#include "../BootRom/flashtool_api.h"

class BromChksumArg{
public:
    BromChksumArg(void);
    explicit BromChksumArg(const RomMemChecksumArg &chksumArg);

    ~BromChksumArg();

    RomMemChecksumArg GetChkusmArg(void) const
    {
        return this->chksum_arg_;
    }

    void set_chksum_value(unsigned int value)
    {
        this->chksum_arg_.chksum = value;
    }

    void set_rom_index(unsigned short index)
    {
        this->chksum_arg_.index = index;
    }

    void set_init_cb(const CALLBACK_ROM_MEM_CHECKSUM_PROGRESS_INIT cb)
    {
        this->chksum_arg_.m_cb_rom_mem_checksum_init = cb;
    }

    void set_chksum_cb(const CALLBACK_ROM_MEM_CHECKSUM_PROGRESS cb)
    {
        this->chksum_arg_.m_cb_rom_mem_checksum = cb;
    }

    void set_init_cb_arg(void *host_pointer)
    {
        this->chksum_arg_.m_cb_rom_mem_checksum_init_arg = host_pointer;
    }

    void set_chksum_cb_arg(void *host_pointer)
    {
        this->chksum_arg_.m_cb_rom_mem_checksum_arg = host_pointer;
    }

    void set_stop_flag(int *p_stop)
    {
        this->chksum_arg_.p_stopflag = p_stop;
    }

    static int __stdcall cb_rom_mem_checksum_init(void *usr_arg,  const char* image_name);
    static int __stdcall cb_rom_mem_checksum(unsigned char finished_percentage, U64 finished_bytes,
                                               U64 total_bytes, void *usr_arg);


    void Dump();
private:
    void FillDefaultValue();

private:
    RomMemChecksumArg chksum_arg_;

};
#endif
