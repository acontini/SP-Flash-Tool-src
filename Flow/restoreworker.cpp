#include <string>
#include <sys/stat.h>

#include "fw_defs.h"
#include "storhelper.h"
#include "NvramVerify.h"
#include "restoreworker.h"
#include "../Logger/Log.h"
#include "../Utility/FileUtils.h"

using std::string;

////////////////////////////////////////////////
// class CRestoreWorker
////////////////////////////////////////////////
CRestoreWorker::CRestoreWorker(
        std::string part_name, U64 addr, U64 leng, U64 dest, NUTL_ReadFlag_E flag, unsigned short part)
    : part_name_(part_name),
      addr_(addr),
      leng_(leng),
      dest_(dest),
      flag_(flag),
      part_(part)
{
}

CRestoreWorker::~CRestoreWorker()
{
}

bool CRestoreWorker::SetChipId(
        const char chip_id[],
        const size_t length)
{
    if (!FileUtils::MakeParentBackupDir())
    {
         LOGE("Create backup parent folder failed!");
         return false;
    }

    base_dir_ = BasePath(chip_id, length);

    return !base_dir_.empty();
}

std::string CRestoreWorker::BasePath(
        const char chip_id[],
        const size_t length)
{
    return FileUtils::ComposeDirNameByChipID(
                (unsigned char*)chip_id, length);
}

std::string CRestoreWorker::BasePathNoCreateIfNotExist(const char chip_id[], const size_t length)
{
    return FileUtils::ComposeDirNameByChipID(
                (unsigned char*)chip_id, length, FileUtils::kBackupFolder, false);
}

bool CRestoreWorker::Verify(
            const CStorHelper *) const
{
    struct stat info;
    memset(&info, 0, sizeof(info));

    // for nand read page + spare, the autual read length = leng_ / pageSize * (pageSize + SpareSize)
    //so info.st_size > leng_, for simply, we will omit check it

    if(flag_ == NUTL_READ_PAGE_SPARE_WITH_ECCDECODE)
        return true;

    return (stat(Path().c_str(), &info) == 0 &&
        info.st_size == (long long)leng_);
}

string CRestoreWorker::Path() const
{
    Q_ASSERT(!base_dir_.empty());
    return base_dir_ + C_SEP_STR + part_name_ + ".raw";
}


////////////////////////////////////////////////
// class CDefaultRestoreWorker
////////////////////////////////////////////////
CDefaultRestoreWorker::CDefaultRestoreWorker(
        std::string part_name, U64 addr, U64 leng, U64 dest, NUTL_ReadFlag_E flag, unsigned short part)
    :CRestoreWorker(part_name, addr, leng, dest, flag, part)
{
}

string CDefaultRestoreWorker::ChksumFilePath() const
{
    Q_ASSERT(!base_dir_.empty());
    return base_dir_ + C_SEP_STR + part_name_ + ".chk";
}

bool CDefaultRestoreWorker::Stamp(
        const CStorHelper *stor)
{
    if(!CRestoreWorker::Verify(stor))
    {
        LOGD("base verification failed (%s)!", part_name().c_str());
        return false;
    }

    unsigned char chksum = 0;

    if (!FileUtils::ComputeCheckSum(Path(), chksum))
    {
        LOGD("failed to calculate check sum (%s)!", part_name().c_str());
        return false;
    }

    LOGD("saving check sum for rom(%s, 0x%08llx-0x%08llx): 0x%02x",
         part_name().c_str(), addr(), end(), chksum);

    return FileUtils::SaveCheckSum(
                ChksumFilePath(),
                chksum);
}

bool CDefaultRestoreWorker::Verify(
        const CStorHelper *stor) const
{
    LOGD("checksum verifying: %08llx-%08llx",
         addr(), end());

    if(!CRestoreWorker::Verify(stor))
    {
        LOGD("base verification failed (%s)!", part_name().c_str());
        return false;
    }

    unsigned char saved_chksum = 0;
    unsigned char calcu_chksum = 0;

    const string chksumpath = ChksumFilePath();

    if (!FileUtils::LoadCheckSum(chksumpath, saved_chksum))
    {
        LOGD("failed to load check sum (%s)!", part_name().c_str());
        return false;
    }

    if (!FileUtils::ComputeCheckSum(Path(), calcu_chksum))
    {
        LOGD("failed to calculate check sum (%s)!", part_name().c_str());
        return false;
    }

    LOGD("check sum for rom(%s, 0x%08llx-0x%08llx): 0x%02x/0x%02x",
         part_name().c_str(), addr(), end(), saved_chksum, calcu_chksum);

    return (saved_chksum ==  calcu_chksum);
}

bool CDefaultRestoreWorker::Clean()
{
    FileUtils::QDeleteFile(Path());
    FileUtils::QDeleteFile(ChksumFilePath());
    return true;
}


////////////////////////////////////////////////
// class CNvRamRestoreWorker
////////////////////////////////////////////////
CNvRamRestoreWorker::CNvRamRestoreWorker(
        std::string part_name, U64 addr, U64 leng, U64 dest,NUTL_ReadFlag_E flag, unsigned short part)
    :CRestoreWorker(part_name, addr, leng, dest, flag, part)
{
}

bool CNvRamRestoreWorker::Stamp(
        const CStorHelper *stor)
{
    return Verify(stor);
}

bool CNvRamRestoreWorker::Verify(
        const CStorHelper *stor) const
{
    if(!CRestoreWorker::Verify(stor))
    {
        LOGD("base verification failed (%s)!", part_name().c_str());
        return false;
    }

    return DoNvRamVerify(
                Path().c_str(),
                stor->BlockSize());
}

bool CNvRamRestoreWorker::Clean()
{
    FileUtils::QDeleteFile(Path());
    return true;
}
