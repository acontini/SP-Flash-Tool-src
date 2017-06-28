#include "PlatformRule.h"

#include "../XMLParser/XMLDocument.h"

#include <sstream>
#include <cassert>

#include <algorithm>
#include <cctype>
#include <string>

#include <QCoreApplication>
#include <QDir>
#include <QFile>

#include "../Logger/Log.h"
#include "../BootRom/host.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../Utility/FileUtils.h"

using XML::Node;
using XML::Document;

Platform::Platform() : name(), simple_name()
{
    owner_name = "";
    image_lock_support = false;
    usb_highspeed_support = false;
    logical_addr_support = false;
    alert_no_com_port = false;
    auto_format_support = false;
    default_bmt_blocks = 0;
    firmware_upgrade_enable = false;
    is_chip_supported = false;
    is_default_da_dl_all_hs = false;
    dl_auto_detect_battery = true; // ?
    auto_format_all_support = true;
    power_key_reset_support = false;
    is_auto_polling_enable = false;
    auto_polling_upper_limit = 0;
    set_boot_mode_support = false;
    check_rpmb_support = false;
}

void Platform::LoadXML(const XML::Node &node)
{
    std::string temp;

    temp = node.GetAttribute("name");
    this->name = temp;

    temp = node.GetAttribute("simple_name");
    this->simple_name = temp;

    temp = node.GetAttribute("owner_name");
    this->owner_name = temp;

    temp = node.GetAttribute("dl_auto_detect_battery");
    this->dl_auto_detect_battery = parseBoolean(temp);

    temp = node.GetAttribute("image_lock_support");
    this->image_lock_support = parseBoolean(temp);

    temp = node.GetAttribute("usb_highspeed_support");
    this->usb_highspeed_support = parseBoolean(temp);

    temp = node.GetAttribute("alert_no_com_port");
    this->alert_no_com_port = parseBoolean(temp);

    temp = node.GetAttribute("auto_format_support");
    this->auto_format_support = parseBoolean(temp);

    temp = node.GetAttribute("logical_addr_support");
    this->logical_addr_support = parseBoolean(temp);

    temp = node.GetAttribute("firmware_upgrade_enable");
    this->firmware_upgrade_enable = parseBoolean(temp);

    temp = node.GetAttribute("default_da_dl_all_hs");
    this->is_default_da_dl_all_hs = parseBoolean(temp);

    temp = node.GetAttribute("auto_format_all_support");
    this->auto_format_all_support = parseBoolean(temp);

    temp = node.GetAttribute("default_bmt_blocks");
    //std::istringstream iss(W2CA(temp.c_str()));
    //iss>>this->default_bmt_blocks;
    this->default_bmt_blocks = strtoul(temp.c_str(), NULL, 10);
    LOG("default_bmt_blocks is %d.\n", this->default_bmt_blocks);

    temp = node.GetAttribute("is_default_set_format_length");
    this->is_default_set_format_length = parseBoolean(temp);

    temp = node.GetAttribute("reset_key_configuration");
    this->power_key_reset_support = (temp=="P");

    temp = node.GetAttribute("is_auto_polling_enable");
    this->is_auto_polling_enable = parseBoolean(temp);

    temp = node.GetAttribute("auto_polling_upper_limit");
    this->auto_polling_upper_limit = atoi(temp.c_str());

    temp = node.GetAttribute("set_boot_mode_support");
    this->set_boot_mode_support = parseBoolean(temp);

    temp = node.GetAttribute("check_rpmb_support");
    this->check_rpmb_support = parseBoolean(temp);
}

void Platform::SaveXML(XML::Node &node) const
{
    if(node.GetName().empty()) return;
    assert(0 && "Unsupported operation.");
}

std::string Platform::Dump() const {
    std::string content("\n-------------dumping platform config-------------\n");
    content.append("name:").append(name).append("\n");
    content.append("simple_name:").append(simple_name).append("\n");
    content.append("-----------------------------------------------\n");
    return content;
}

bool Platform::parseBoolean(const std::string &value){
    if(value == "true" || value == "True" || value == "TRUE"){
        return true;
    }else /*if(value == "false" || value == "False" || value == "FALSE")*/{
        // default 'false'
        return false;
        //}else{
        //    assert(0 && "Invalid format for boolean value in XML document.");
        //    return false;
    }
}

std::string PlatformTable::parseScatterName(const std::string &value){
    LOG("PlatformTable::parseScatterName() : %s", value.c_str());

    std::map<std::string, Platform>::const_iterator it;
    for(it = simpleNameMap.begin(); it != simpleNameMap.end(); ++it){
        std::string name = it->first;
        if(value.find(name) != std::string::npos)
            return name;

        std::string lowerName(name);
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),::tolower);
        LOG("lower name = %s",lowerName.c_str());

        if(value.find(lowerName) != std::string::npos)
            return name;
    }
    return "";
}

std::string PlatformTable::parseChipName(const std::string &value){
    std::map<std::string, Platform>::const_iterator it;
    for(it = nameMap.begin(); it != nameMap.end(); ++it){
        std::string name = it->first;
        if(value.find(name) != std::string::npos)
            return name;

        std::string lowerName(name);
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),::tolower);
        LOG("PlatformTable::parseChipName(): Lower name = %s",lowerName.c_str());

        if(value.find(lowerName) != std::string::npos)
            return name;
    }
    return "";
}

PlatformTable::PlatformTable(const std::string &filename){
    LoadFile(filename);
}

void PlatformTable::LoadFile(const std::string &filename){
    Document document(filename);
    const Node rootNode = document.GetRootNode();
    assert(rootNode.GetName() == "platform-table");

    Node plNode = rootNode.GetFirstChildNode();

    while(!plNode.IsEmpty())
    {
        Platform pl;
        pl.LoadXML(plNode);

        nameMap[pl.getName()] = pl;
        simpleNameMap[pl.getSimpleName()] = pl;

        plNode = plNode.GetNextSibling();
    }
}

Platform PlatformTable::queryByScatter(const std::string &scatter){
    std::string simpleName = parseScatterName(scatter);
    Platform platform;
    if(simpleNameMap.find(scatter) != simpleNameMap.end())
	{
        platform = simpleNameMap[scatter];
        platform.setChipSupported(true);
	}
	else if( simpleNameMap.find(simpleName) != simpleNameMap.end())
	{
        platform = simpleNameMap[simpleName];
        platform.setChipSupported(true);
    } else {
        platform.setChipSupported(false);
    }
    return platform;  //throw exception
}

Platform PlatformTable::queryByChipName(const std::string &chipName){
    std::string name = parseChipName(chipName);
    Platform platform;

    if(nameMap.find(name) != nameMap.end()){
        platform = nameMap[name];
        platform.setChipSupported(true);
    } else {
        platform.setChipSupported(false);
    }
    return platform;  //throw exception
}

PlatformTable& GetPlatformTable(){
    std::string xml_file = ABS_PATH("platform.xml");
    if(!FileUtils::IsFileExist(xml_file)) {
        LOGE("xml(%s) is NOT existed!", xml_file.c_str());
        THROW_APP_EXCEPTION(FT_FIND_PLATFORM_ERROR, "");
    }

    static PlatformTable plTable(xml_file);

    return plTable;
}

