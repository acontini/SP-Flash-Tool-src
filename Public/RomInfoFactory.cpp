#include "RomInfoFactory.h"
#include "../Logger/Log.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/ResourceManager.h"
#include "../BootRom/flashtool_api.h"
#include "../Conn/Connection.h"
#include "../Utility/Utils.h"

#include <QScopedPointer>


RomInfoFactory::RomInfoFactory(APKey key):
    key_(key)
{
}

#if _DEBUG
static void dump_rom_list(
        const std::list<ROM_INFO> &rom_info_list)
{
    std::list<ROM_INFO>::const_iterator it =
            rom_info_list.begin();

    while (it != rom_info_list.end())
    {
        LOGD("<%s>: 0x%08llx-0x%08llx-0x%d",
             it->name,
             it->begin_addr,
             it->end_addr,
             it->part_id);

        ++ it;
    }

}
#else
#define dump_rom_list(x)
#endif

int RomInfoFactory::GetRomList(const QSharedPointer<APCore::Connection> &conn,
                               std::list<ROM_INFO> &rom_info_list)
{
    unsigned int rom_count(0);
    DL_HANDLE_T dl_handle = GET_DL_HANDLE_T(key_);

    rom_info_list.clear();

    int ret = FlashTool_RomGetCount(conn->FTHandle(), dl_handle, &rom_count);

    if (S_DONE != ret || rom_count < 1) {
        LOGE("FlashTool_RomGetCount() failed, ret(%d).", ret);
    } else {
        QScopedPointer< ROM_INFO, QScopedPointerArrayDeleter<ROM_INFO> > p_roms(new ROM_INFO[rom_count]);
        ROM_INFO *p = p_roms.data();
        ret = FlashTool_RomGetInfoAll(conn->FTHandle(), dl_handle, p, rom_count);
        if (S_DONE == ret) {
            for (unsigned int i(0); i < rom_count; i++) {
                //To unify end_addr
                p[i].end_addr = p[i].begin_addr + p[i].partition_size;
                rom_info_list.push_back((p[i]));
            }
            dump_rom_list(rom_info_list);
        } else {
            LOGE("FlashTool_RomGetInfoAll() failed, ret(%d).", ret);
        }
    }

    return ret;
}

int RomInfoFactory::GetPartitionList(const QSharedPointer<APCore::Connection> &conn,
                               std::list<PART_INFO> &partition_info_list)
{
    unsigned int rom_count(0), partition_count;
    DL_HANDLE_T dl_handle = GET_DL_HANDLE_T(key_);

    partition_info_list.clear();

    //here, partition_count is accurate than rom count;
    //rom count only rom in scatter file, and not include preloader, sgpt, pgpt
    //partition count have all partition on device
    int ret = FlashTool_RomGetCount(conn->FTHandle(), dl_handle, &rom_count);
    if(ret != S_DONE)
        return ret;

    ret = FlashTool_ReadPartitionCount(conn->FTHandle(), &partition_count);
    if(ret != S_DONE)
        return ret;

    LOGD("rom count: %d,  partition_count :%d", rom_count, partition_count);

    PART_INFO* part_info_all_ = new PART_INFO[partition_count+1];

    if (partition_count > 0)
    {
        ret = FlashTool_ReadPartitionInfo(
                    conn->FTHandle(),
                    part_info_all_,
                    partition_count);

        if( S_DONE != ret )
        {
            delete [] part_info_all_;
            part_info_all_ = NULL;
            return false;
        }
    }

    part_info_all_[partition_count].image_length = 0;
    part_info_all_[partition_count].begin_addr = 0;
    part_info_all_[partition_count].name[0] = 0;

    for (unsigned int i=0; i < partition_count; i++)
    {
        partition_info_list.push_back(part_info_all_[i]);
        LOGD("<%s>: 0x%08llx-0x%08llx-0x%d",
             part_info_all_[i].name,
             part_info_all_[i].begin_addr,
             part_info_all_[i].begin_addr + part_info_all_[i].image_length,
             part_info_all_[i].part_id);
    }

    return ret;
}

int RomInfoFactory::GetScatterRomList(const QSharedPointer<APCore::Connection> &conn,
                               std::list<ROM_INFO> &rom_info_list)
{
    unsigned short rom_count(0);
    DL_HANDLE_T dl_handle = GET_DL_HANDLE_T(key_);

    rom_info_list.clear();

    int ret = DL_GetCount(dl_handle, &rom_count);

    if (S_DONE != ret || rom_count < 1) {
        LOGE("DL_GetCount() failed, ret(%d).", ret);
    } else {
        QScopedPointer< ROM_INFO, QScopedPointerArrayDeleter<ROM_INFO> > p_roms(new ROM_INFO[rom_count]);
        ROM_INFO *p = p_roms.data();
        ret = DL_Rom_GetInfoAll(dl_handle, p, rom_count);
        if (S_DONE == ret) {
            for (unsigned int i(0); i < rom_count; i++) {
                //To unify end_addr
                p[i].end_addr = p[i].begin_addr + p[i].partition_size;
                rom_info_list.push_back((p[i]));
            }
            dump_rom_list(rom_info_list);
        } else {
            LOGE("DL_Rom_GetInfoAll() failed, ret(%d).", ret);
        }
    }

    return ret;
}
