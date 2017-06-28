#ifndef PLATFORMRULE_H
#define PLATFORMRULE_H

#include <iostream>
#include <map>
#include <string>

#include "../XMLParser/XMLSerializable.h"

using std::string;
using std::istringstream;

class Platform : public XML::Serializable
{
public:
    Platform();

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    std::string Dump() const;

    //getters
    string getName() const{
        return name;
    }
    string getSimpleName() const{
        return simple_name;
    }
    string getOwnerName() const{
        return owner_name;
    }
    bool supportDLAutoDetectBattery() const{
        return dl_auto_detect_battery;
    }
    bool isDefaultDADownloadAllHS()const{
        return is_default_da_dl_all_hs;
    }
    bool supportUSBHighSpeed() const{
        return usb_highspeed_support;
    }
    bool supportLogicalAddress() const{
        return logical_addr_support;
    }
    bool supportImageLock() const{
        return image_lock_support;
    }
    bool supportAutoFormat() const{
        return auto_format_support;
    }
    bool doesAlertNoCOMPort() const{
        return alert_no_com_port;
    }
    unsigned int getDefaultBMTBlocks() const{
        return default_bmt_blocks;
    }
    bool supportFirmwareUpgrade() const {
        return firmware_upgrade_enable;
    }
    void setChipSupported(bool is_support) {
        is_chip_supported = is_support;
    }
    bool isChipSupported() const {
        return is_chip_supported;
    }
    bool isDefaultSetFormatLength() const {
        return is_default_set_format_length;
    }
    bool supportAutoFormatAll() const{
        return auto_format_all_support;
    }

    bool supportPowerKeyReset() const {
        return power_key_reset_support;
    }

    bool isAutoPollingEnable() const {
        return is_auto_polling_enable;
    }

    unsigned int autoPollingUpperLimit() const{
        return auto_polling_upper_limit;
    }

    bool isSetBootModeSupport(void) const
    {
        return set_boot_mode_support;
    }

    bool isCheckRPMBSupport(void) const
    {
        return check_rpmb_support;
    }
private:
    bool parseBoolean(const std::string &value);

    string name;
    string simple_name;
    string owner_name;

    bool image_lock_support;
    bool usb_highspeed_support;
    bool logical_addr_support;
    bool alert_no_com_port;
    bool auto_format_support;
    unsigned int default_bmt_blocks;
    bool firmware_upgrade_enable;
    bool is_default_da_dl_all_hs;
    //Judge scatter load is supported by platform.xml
    bool is_chip_supported;
    //Modify to support auto detect battery
    bool dl_auto_detect_battery;
    //Enable set default format length
    bool is_default_set_format_length;
    bool auto_format_all_support;
    bool power_key_reset_support;
    bool is_auto_polling_enable;
    unsigned int  auto_polling_upper_limit;
    bool set_boot_mode_support;
    bool check_rpmb_support;
};


class PlatformTable
{
public:
    PlatformTable(const std::string &filename);

    Platform queryByScatter(const std::string &scatter);
    Platform queryByChipName(const std::string &chipName);

private:
    void LoadFile(const std::string &filename);

    //parse out 'MT65XX' from scatter file name
    std::string parseScatterName(const std::string &value);

    //parse out 'MT65XX_SXX' from chip name
    std::string parseChipName(const std::string &value);

    std::map<std::string, Platform> nameMap;
    std::map<std::string, Platform> simpleNameMap;

    friend PlatformTable& GetPlatformTable();
};

PlatformTable& GetPlatformTable();

#endif //PLATFORMRULE

