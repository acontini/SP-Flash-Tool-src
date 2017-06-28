#ifndef FIRMWAREUPGRADECOMMAND_H
#define FIRMWAREUPGRADECOMMAND_H

#include "ICommand.h"
#include "../Setting/ISetting.h"
#include "../Arg/BromDownloadArg.h"
#include "../Arg/BromFormatArg.h"
#include "../Setting/ReadbackSetting.h"
#include "../Setting/WriteMemorySetting.h"
#include "../Setting/FormatSetting.h"
#include "../Setting/DADownloadAllSetting.h"

#include <QSharedPointer>

#include "../Flow/fw_defs.h"

class IRomBase;
class CDLProxy;

namespace APCore
{

class Connection;

class FirmwareUpgradeCommand : public ICommand
{
public:
    FirmwareUpgradeCommand(APKey key);
    virtual ~FirmwareUpgradeCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);


    void set_readback_setting(
            const QSharedPointer<ReadbackSetting> &readback_setting)
    {
        this->rb_setting_ = readback_setting;
    }

    void set_writememory_setting(
            const QSharedPointer<WriteMemorySetting> &writememory_setting)
    {
        this->wm_setting_ = writememory_setting;
    }

    QSharedPointer<APCore::FormatSetting> get_format_setting(void)
    {
        return this->fmt_setting_;
    }

    void set_format_setting(
            QSharedPointer<FormatSetting> &format_setting)
    {
        this->fmt_setting_ = format_setting;
    }

    void set_download_setting(
            const QSharedPointer<DADownloadAllSetting> &dl_setting)
    {
        this->dl_setting_ = dl_setting;
    }

    void set_download_scene(Download_Scene scene)
    {
        dl_scene_ = scene;
    }

private:
    //Validations
    int validateBeforeRestore();
    int validateBeforeReadback();


private:
    ERROR_T DoFirmwareUpgrade(
        IRomBase *rom,
        CDLProxy *proxy);

    ERROR_T DoFormat(
        PART_INFO &rom,
        CDLProxy *proxy);

    ERROR_T DoFirmwareUpgrade(
        const QSharedPointer<APCore::Connection> &conn,
        CDLProxy *proxy);

    ERROR_T ProduceRomList(
        const QSharedPointer<APCore::Connection> &conn,
        std::list<QSharedPointer<IRomBase> > &roms_2dl);

    ERROR_T ProduceFormatRomList(
        const QSharedPointer<APCore::Connection> &conn,
        std::list<PART_INFO> &roms_2format);

    bool IsPartInRomList(std::list<ROM_INFO> &roms, PART_INFO &rom);

 /*   int GetRomList(
        std::list<ROM_INFO> &rom_info_list,
        U64 stor_size);

    void PushRomInfo(
        std::list<ROM_INFO> &rom_info_list,
        ROM_INFO &rom,
        U64 stor_size);*/

    static std::string ErrMessage(const IRomBase*, ERROR_T);

private:
    QSharedPointer<APCore::ReadbackSetting> rb_setting_;
    QSharedPointer<APCore::WriteMemorySetting> wm_setting_;
    QSharedPointer<APCore::FormatSetting> fmt_setting_;
    QSharedPointer<APCore::DADownloadAllSetting> dl_setting_;
    Download_Scene dl_scene_;
};

}

#endif // FIRMWAREUPGRADECOMMAND_H
