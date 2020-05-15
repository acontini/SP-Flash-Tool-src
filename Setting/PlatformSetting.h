#ifndef PLATFORMSETTING_H
#define PLATFORMSETTING_H

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "../Rules/StorageRule.h"
#include "../Rules/PlatformRule.h"
#include "../BootRom/flashtool_handle.h"

class Platform;
class StorageRule;

namespace APCore{
class IPlatformOb{
public:
    virtual void onPlatformChanged() = 0;
};

class PlatformSetting{
public:
    static const int DEFAULT_BMT_BLOCKS_MT6573;
    static const int DEFAULT_PMT_BLOCKS_MT6573;
    PlatformSetting();
    ~PlatformSetting();

    //called when load a scatter file, will update UI accordingly
    void initByScatterFile(const std::string &scatterFile, std::string& error_hint);
    bool initByNewScatterFile(const DL_PlatformInfo &info, std::string& error_hint);

    //called @ runtime when get DA report, will update platform config, but will not update UI
    void setChipName(const std::string &chipName);

    bool isPlatformMatched();

    bool isDADownloadAllEnabled() const{
        return m_DA_DL_all_enabled;
    }

    void setDADownloadAllEnabled(bool enabled);

    std::string getLoadPlatformName() const;

    std::string getTargetPlatformName() const;

    void setBMTBlocks(unsigned int blocks){
        m_BMT_blocks = blocks;
    }
    unsigned int getBMTBlocks() const{
        return m_BMT_blocks;
    }

    unsigned int getNandReservedBlocks() const {
        return (m_BMT_blocks + PlatformSetting::DEFAULT_PMT_BLOCKS_MT6573);
    }

    void addObserver(IPlatformOb *ob){
        observers.push_back(ob);
    }

    const Platform &getPlatformConfig() const{
        return m_platform;
    }

    const FlashToolStorageConfig &getFlashToolStorageConfig() const {
        return m_storage_config;
    }

    bool is_scatter_file_valid(void) const {
        return is_scatter_file_valid_;
    }

    bool is_storage_type_support(void) const {
        return is_storage_type_support_;
    }

    std::string load_path(void) const {
        return this->load_path_;
    }

    bool supportPowerKeyReset() const {
        return m_platform.supportPowerKeyReset();
    }

    bool is_set_boot_mode_support(void) const
    {
        return m_platform.isSetBootModeSupport();
    }

    bool is_check_rpmb_support(void) const
    {
        return m_platform.isCheckRPMBSupport()
               && m_storage_config.IsEMMCOperation();
    }

    PlatformSetting* Clone();

private:
    void initUsbHighSpeedChipMap();
    void notifyObservers();
    //20110825 modify for nand/emmc/sdmmc storage type switch
    void initStorageOperationTable(void);
    void set_load_path(const std::string& scatter_file);

private:
    std::string JudgeStorageOperation(const std::string& scatt);
    Platform m_platform;
    FlashToolStorageConfig m_storage_config;
    unsigned int m_BMT_blocks;
    bool m_DA_DL_all_enabled;
    std::string m_current_scatter;  //MT6573
    std::string m_current_chip;   //MT6573_S0X
    std::list<IPlatformOb *> observers;
    //20110825 add map for nand/emmc/sdmmc storage type switch
    std::list<std::string> m_storage_oper_table_;
    bool is_scatter_file_valid_;
    bool is_storage_type_support_;
    //add a column to obtain load folder location
    std::string load_path_;
};
}

#endif // PLATFORMSETTING_H
