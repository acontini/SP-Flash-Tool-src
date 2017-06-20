#ifndef FIRMWAREUPGRADESETTING_H
#define FIRMWAREUPGRADESETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"
#include "FormatSetting.h"
#include "ReadbackSetting.h"
#include "WriteMemorySetting.h"
#include "DADownloadAllSetting.h"
#include "../Cmd/FirmwareUpgradeCommand.h"

#include "../Logger/Log.h"

#include <string>
#include <QSharedPointer>

namespace APCore
{
class FirmwareUpgradeSetting : public ISetting
{
public:
    FirmwareUpgradeSetting();
    QSharedPointer <APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    //setters
    void set_format_setting(QSharedPointer<APCore::FormatSetting> setting)
    {
        this->format_setting_ = setting;
    }
    void set_download_setting(QSharedPointer<APCore::DADownloadAllSetting> setting)
    {
         this->download_setting_ = setting;
    }
    void set_readback_setting(QSharedPointer<APCore::ReadbackSetting> setting)
    {
          this->readback_setting_ = setting;
    }
    void set_writememory_setting(QSharedPointer<APCore::WriteMemorySetting> setting)
    {
           this->wm_setting_ = setting;
    }

    void set_download_scene(Download_Scene &scene)
    {
          this->scene_ = scene;
    }

    virtual void set_storage_type(HW_StorageType_E type);
private:
    void ConfigOperationByScene(QSharedPointer<APCore::FirmwareUpgradeCommand> cmd,
                                Download_Scene &scene);

private:
    QSharedPointer<APCore::FormatSetting> format_setting_;
    QSharedPointer<APCore::DADownloadAllSetting> download_setting_;
    QSharedPointer<APCore::ReadbackSetting> readback_setting_;
    QSharedPointer<APCore::WriteMemorySetting> wm_setting_ ;
    Download_Scene scene_;
};
}

#endif // FIRMWAREUPGRADESETTING_H
