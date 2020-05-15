#ifndef DADOWNLOADALLSETTING_H
#define DADOWNLOADALLSETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"
#include "../Public/AppCore.h"

namespace APCore
{
class RomItem
{
public:
    RomItem():path(),name(),index(0),enable(false){}

    std::string path;
    std::string name;
    unsigned int index;
    bool enable;
};

class DADownloadAllSetting : public ISetting
{
public:
    DADownloadAllSetting();

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    DADownloadAllSetting* Clone() const
    {
          return new DADownloadAllSetting(*this);
    }

    void InitialRomList();

    void set_cb_da_report(CALLBACK_DA_REPORT cb)
    {
        cb_da_report = cb;
    }

    void set_cb_bl_dl_init(CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS_INIT cb)
    {
        cb_bl_dl_init = cb;
    }

    void set_cb_bl_dl_prog(CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS cb)
    {
        cb_bl_dl_prog = cb;
    }

    void set_cb_dl_init(CALLBACK_DOWNLOAD_PROGRESS_INIT cb)
    {
        cb_dl_init = cb;
    }

    void set_cb_dl_prog(CALLBACK_DOWNLOAD_PROGRESS cb)
    {
        cb_dl_prog = cb;
    }

    void set_cb_da_chksum_init(const CALLBACK_CHECKSUM_PROGRESS_INIT cb)
    {
        cb_da_chksum_init = cb;
    }

    void set_cb_da_chksum_progress(const CALLBACK_CHECKSUM_PROGRESS cb)
    {
       cb_da_chksum_progress = cb;
    }

    void set_cb_cert_init(const CALLBACK_SECURITY_PRE_PROCESS_NOTIFY cb)
    {
        sec_dl_init = cb;
    }

    void set_cb_cert_progress(const CALLBACK_SECURITY_POST_PROCESS_NOTIFY cb)
    {
        sec_dl_prog = cb;
    }

    const std::list<RomItem>& rom_list() const
    {
        return rom_list_;
    }

    void set_rom_list(const std::list<RomItem>& roms)
    {
        rom_list_ = roms;
    }

    const std::string& scatter_file() const
    {
        return scatter_file_;
    }

    void set_scatter_file(const std::string &file)
    {
        scatter_file_ = file;
    }

    void set_download_scene(const Download_Scene &scene)
    {
        scene_ = scene;
    }

private:
    CALLBACK_DA_REPORT  cb_da_report;
    CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS_INIT cb_bl_dl_init;
    CALLBACK_BOOTLOADER_DOWNLOAD_PROGRESS cb_bl_dl_prog;
    CALLBACK_CHECKSUM_PROGRESS_INIT cb_da_chksum_init;
    CALLBACK_CHECKSUM_PROGRESS cb_da_chksum_progress;
    CALLBACK_DOWNLOAD_PROGRESS_INIT cb_dl_init;
    CALLBACK_DOWNLOAD_PROGRESS cb_dl_prog;
    CALLBACK_SECURITY_PRE_PROCESS_NOTIFY sec_dl_init;
    CALLBACK_SECURITY_POST_PROCESS_NOTIFY sec_dl_prog;

    std::string scatter_file_;
    std::list<RomItem> rom_list_;

    AppCore* app;
    APKey* session_id;
    std::string chip_name_;
    Download_Scene scene_;
};

}

#endif // DADOWNLOADALLSETTING_H
