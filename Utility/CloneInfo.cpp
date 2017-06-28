#include "CloneInfo.h"
#include "./BootRom/host.h"
#include "./BootRom/flashtool.h"
#include "./Logger/Log.h"
#include "FileUtils.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>

void CloneInfo::SaveXML(XML::Node &node) const
{
    XML::Node rootNode(node.AppendChildNode("partition"));
    //add attribute
    rootNode.SetAttribute("name", partition_name_);

    rootNode.SetAttribute("part_id", EmmcPartToString((EMMC_Part_E)part_id_));

    std::ostringstream start_addr;
    start_addr<<std::hex<<"0x"
             <<std::setfill('0')<<std::setw(16)
            <<start_addr_;
    rootNode.SetAttribute("start_addr", start_addr.str());

    std::ostringstream length;
    length<<std::hex<<"0x"
         <<std::setfill('0')<<std::setw(16)
        <<partition_len_;
    rootNode.SetAttribute("length", length.str());

    //add child node
    rootNode.AppendChildNode("file_path", image_path_);

    std::ostringstream chksum;
    chksum <<std::hex << "0x"
           <<std::setfill('0') <<std::setw(8)
           << (unsigned int)chksum_value_;
    LOG("chksum(%s)", chksum.str().c_str());
    rootNode.AppendChildNode("chksum", chksum.str());
}

static unsigned int GetPartIDFromString(std::string strPartID)
{
    /*
     EMMC_PART_UNKNOWN = 0
    ,EMMC_PART_BOOT1
    ,EMMC_PART_BOOT2
    ,EMMC_PART_RPMB
    ,EMMC_PART_GP1
    ,EMMC_PART_GP2
    ,EMMC_PART_GP3
    ,EMMC_PART_GP4
    ,EMMC_PART_USER
    */
    if(strPartID == "EMMC_PART_USER")       return 8;
    if(strPartID == "EMMC_PART_BOOT1")      return 1;
    if(strPartID == "EMMC_PART_BOOT2")      return 2;
    if(strPartID == "EMMC_PART_RPMB")       return 3;
    if(strPartID == "EMMC_PART_GP1")        return 4;
    if(strPartID == "EMMC_PART_GP2")        return 5;
    if(strPartID == "EMMC_PART_GP3")        return 6;
    if(strPartID == "EMMC_PART_GP4")        return 7;
    if(strPartID == "EMMC_PART_UNKNOWN")    return 0;
    return 0;
}

void CloneInfo::LoadXML(const XML::Node &node)
{
    XML::Node child = node.GetFirstChildNode();

    partition_name_ = node.GetAttribute("name").c_str();

    std::string strPartID = node.GetAttribute("part_id").c_str();
    part_id_ = GetPartIDFromString(strPartID);

    sscanf(node.GetAttribute("start_addr").c_str(),"%llX",&start_addr_);

    sscanf(node.GetAttribute("length").c_str(), "%llX", &partition_len_);

    while(!child.IsEmpty())
    {
        const std::string name = child.GetName();

        if(name == "file_path")
            image_path_ = child.GetText().c_str();
        else if(name == "chksum")
            chksum_value_ = strtoul(child.GetText().c_str(), 0, 16);
        else
        {
            LOGE("Invalid clone info item type(%s)!", child.GetText().c_str());
        }

        child = child.GetNextSibling();
    }
}

bool CloneInfo::verify(const std::string &dir)
{
    std::string path = dir + C_SEP_STR + partition_name_;

    if(!FileUtils::IsFileExist(path))
    {
        return false;
    }

    unsigned char chksum = 0;

    FileUtils::ComputeCheckSum(path, chksum);

    if(chksum != chksum_value_)
        return false;

    return true;
}

