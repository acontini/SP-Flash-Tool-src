#include <list>
#include "../Public/AppCore.h"
#include "FirmwareUpgradeCommand.h"
#include "../Utility/Utils.h"
#include "../Utility/FileUtils.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/RBHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Logger/Log.h"

#include "../Flow/storhelper.h"
#include "../Flow/romfactory.h"
#include "../Flow/concreterom.h"
#include "../Flow/dlproxy.h"
#include "../Public/RomInfoFactory.h"
#include "../Host/Inc/RuntimeMemory.h"

#define _PRE_CHECK_ALL_

#define theClass    APCore::FirmwareUpgradeCommand

theClass::FirmwareUpgradeCommand(APKey key):
    ICommand(key),
    rb_setting_(NULL),
    wm_setting_(NULL),
    fmt_setting_(NULL),
    dl_setting_(NULL)
{
}

theClass::~FirmwareUpgradeCommand()
{
}


ERROR_T theClass::DoFirmwareUpgrade(
        IRomBase *rom,
        CDLProxy *proxy)
{
    ERROR_T error = ERROR_OK;

#ifndef _PRE_CHECK_ALL_
    if ((error = rom->check(proxy)) != ERROR_OK)
        return error;
#endif

    if (rb_setting_ != NULL)
    {
        if ((error = rom->read(proxy)) != ERROR_OK)
            return error;
    }

    if (fmt_setting_ != NULL)
    {
        if ((error = rom->format(proxy)) != ERROR_OK)
            return error;
    }

    if (dl_setting_ != NULL)
    {
        if ((error = rom->download(proxy)) != ERROR_OK)
            return error;
    }

    if (wm_setting_ != NULL)
    {
        if ((error = rom->restore(proxy)) != ERROR_OK)
            return error;
    }
    return ERROR_OK;
}

ERROR_T theClass::ProduceRomList(
    const QSharedPointer<APCore::Connection> &conn,
    RomBaseList &roms_2dl)
{
    CRomFactory rom_factory(roms_2dl, fmt_setting_->does_format_whole());

    ERROR_T error = ERROR_OK;

    std::list<ROM_INFO> rom_list;

    RomInfoFactory info_factory(key_);
    info_factory.GetRomList(conn, rom_list);

    std::list<ROM_INFO>::iterator it = rom_list.begin();

    while (it != rom_list.end())
    {
        error = rom_factory.produce(*it, dl_scene_);

        if (error != ERROR_OK)
        {
            break;
        }
        ++ it;
    }

    return error;
}

bool theClass::IsPartInRomList(std::list<ROM_INFO> &roms, PART_INFO &rom)
{
    for(std::list<ROM_INFO>::iterator it = roms.begin(); it!=roms.end(); ++it)
    {
        if(strcmp(rom.name, it->name) == 0)
            return true;
    }
    return false;
}

ERROR_T theClass::ProduceFormatRomList(
    const QSharedPointer<APCore::Connection> &conn,
    std::list<PART_INFO> &roms_2format)
{
    //get scatter rom info
    RomBaseList roms_scatter;
    CRomFactory rom_factory_scatter(roms_scatter, fmt_setting_->does_format_whole());
    ERROR_T error = ERROR_OK;
    std::list<ROM_INFO> rom_list_scatter;
    RomInfoFactory info_scatter_factory(key_);
    info_scatter_factory.GetScatterRomList(conn, rom_list_scatter);
    std::list<ROM_INFO>::iterator scatter_it = rom_list_scatter.begin();
    while (scatter_it != rom_list_scatter.end())
    {
        error = rom_factory_scatter.produce(*scatter_it, dl_scene_);
        if (error != ERROR_OK)
        {
            break;
        }
        ++ scatter_it;
    }

    //get target device rom info
    //and compare rom_list_device & rom_list_scatter
    //roms_2format = rom_list_device - rom_list_scatter
    roms_2format.clear();

    //RomBaseList roms_device;
    //CRomFactory rom_factory_device(roms_device, fmt_setting_->does_format_whole());
    std::list<PART_INFO> partition_list_device;
    RomInfoFactory info_device_factory(key_);
    int err_getpart = info_device_factory.GetPartitionList(conn, partition_list_device);
    if(err_getpart != S_DONE)
        return ERROR_PMT_UNAVAILABLE;

    std::list<PART_INFO>::iterator device_it = partition_list_device.begin();
    while (device_it != partition_list_device.end())
    {
        if(!IsPartInRomList(rom_list_scatter, *device_it))
        {
            roms_2format.push_back(*device_it);
        }

        ++ device_it;
    }

    LOGD("roms_2format count: %d", roms_2format.size());

    for(std::list<PART_INFO>::iterator tofmt_it = roms_2format.begin(); tofmt_it != roms_2format.end(); ++tofmt_it)
    {
        LOGD("rom: <%s>: 0x%08llx-0x%08llx-0x%d", tofmt_it->name, tofmt_it->begin_addr, tofmt_it->begin_addr + tofmt_it->image_length, tofmt_it->part_id);
    }

    return error;
}

ERROR_T theClass::DoFormat(PART_INFO &rom, CDLProxy *proxy)
{
    proxy->Add_Rom_2format(rom);
    return ERROR_OK;
}

ERROR_T theClass::DoFirmwareUpgrade(const QSharedPointer<APCore::Connection> &conn,
        CDLProxy *proxy)
{
    std::string errInfo;

    RomBaseList rom_list_2dl;

    ERROR_T error = ProduceRomList(conn,  rom_list_2dl);

    RomBaseList::iterator it;

#ifdef _PRE_CHECK_ALL_
    if (error == ERROR_OK)
    {
        it = rom_list_2dl.begin();

        while (it != rom_list_2dl.end())
        {
            error = (*it)->check(proxy);

            if (error != ERROR_OK)
            {
                errInfo = ErrMessage(it->data(), error);
            }
            ++ it;
        }
    }
#endif

    //fix issue: no download image(not in target load, but in phone) not format yet
    //e.g., partition (frp) is in phone, but not in firmware upgrade target scatter file, it will not be format.

    //only ROM not in target scatter file. ROM in target scatter file will do in "DoFirmwareUpgrade", and not in this list
    std::list<PART_INFO> rom_list_2format;
    error = ProduceFormatRomList(conn, rom_list_2format);
    std::list<PART_INFO>::iterator fmtit;

    if (error == ERROR_OK && errInfo.empty())
    {
        fmtit = rom_list_2format.begin();

        while (fmtit != rom_list_2format.end())
        {
            error = DoFormat(*fmtit, proxy);

            if (error != ERROR_OK)
            {
                errInfo += fmtit->name;
                errInfo += " DoFormat error: ";
                errInfo += error;
                errInfo += "\n";
                break;
            }

            ++ fmtit;
        }
    }

    if (error == ERROR_OK && errInfo.empty())
    {
        it = rom_list_2dl.begin();

        while (it != rom_list_2dl.end())
        {
            error = DoFirmwareUpgrade(it->data(), proxy);

            if (error != ERROR_OK)
            {
                errInfo = ErrMessage(it->data(), error);
                break;
            }

            ++ it;
        }
    }

    if (!errInfo.empty())
    {
        //errInfo.erase(errInfo.length()-1);
        fw_throw_error(error, errInfo.c_str());
    }

    return error;
}

void theClass::exec(
        const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key_);

    CStorHelper storage(conn.data());

    CDLProxy proxy(conn, rb_handle, storage);

    ERROR_T error = DoFirmwareUpgrade(conn, &proxy);

    if (error == ERROR_OK)
    {
        error = proxy.validate_storage(
                    storage.AlignSize(),
                    storage.LogicalSize());
    }

    if (error == ERROR_OK)
    {
        error = proxy.commit(
                    rb_setting_.data(),
                    fmt_setting_.data(),
                    dl_setting_.data(),
                    wm_setting_.data());
    }

    if (error != ERROR_OK)
    {
        fw_throw_error(error);
    }
}

std::string theClass::ErrMessage(
        const IRomBase *rom, ERROR_T err)
{
    std::string msg = rom->name();
    msg += ": ";
    msg += fw_error_text(err);
    msg += "\n";
    return msg;
}
