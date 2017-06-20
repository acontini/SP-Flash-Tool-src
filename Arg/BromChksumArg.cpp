#include "BromChksumArg.h"

#include "../Logger/Log.h"


BromChksumArg::BromChksumArg(const RomMemChecksumArg &chksumValue)
{
    memset(&chksum_arg_, 0, sizeof(RomMemChecksumArg));

    this->chksum_arg_.chksum = chksumValue.chksum;
    this->chksum_arg_.index = chksumValue.index;
    this->chksum_arg_.m_cb_rom_mem_checksum = chksumValue.m_cb_rom_mem_checksum;
    this->chksum_arg_.m_cb_rom_mem_checksum_arg = chksumValue.m_cb_rom_mem_checksum_arg;
    this->chksum_arg_.m_cb_rom_mem_checksum_init = chksumValue.m_cb_rom_mem_checksum_init;
    this->chksum_arg_.m_cb_rom_mem_checksum_init_arg = chksumValue.m_cb_rom_mem_checksum_init_arg;
}

BromChksumArg::BromChksumArg()
{
    this->FillDefaultValue();
}

BromChksumArg::~BromChksumArg()
{}

void BromChksumArg::FillDefaultValue()
{
    this->chksum_arg_.chksum = 0;
    this->chksum_arg_.index = 0;
    this->chksum_arg_.m_cb_rom_mem_checksum = cb_rom_mem_checksum;
    this->chksum_arg_.m_cb_rom_mem_checksum_init = cb_rom_mem_checksum_init;
    this->chksum_arg_.m_cb_rom_mem_checksum_arg = NULL;
    this->chksum_arg_.m_cb_rom_mem_checksum_init_arg = NULL;
    this->chksum_arg_.p_stopflag = NULL;
}

int BromChksumArg::cb_rom_mem_checksum_init(void *usr_arg, const char *image_name)
{
    LOGI("Begin to calculate image(%s) checksum value...", image_name);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromChksumArg::cb_rom_mem_checksum(unsigned char finished_percentage, U64 finished_bytes, U64 total_bytes, void *usr_arg)
{
    static unsigned int lastReportProgress = 0;

    if((finished_percentage != lastReportProgress) &&
         (finished_percentage % 10 == 0))
    {
        LOGI("%d%% has been calculated, %llu / %llu", finished_percentage, finished_bytes, total_bytes);
        lastReportProgress = finished_percentage;
    }

    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

void BromChksumArg::Dump()
{
    LOGD("checksum index(%d)", this->chksum_arg_.index);
    LOGD("checksum checksum value(%d)", this->chksum_arg_.chksum);
}
