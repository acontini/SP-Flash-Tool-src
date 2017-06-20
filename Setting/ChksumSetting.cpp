#include "ChksumSetting.h"

#include "../Logger/Log.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../XMLParser/XMLNode.h"

namespace APCore{

ChksumSetting::ChksumSetting()
    :cb_checksum_init_(NULL),
     cb_checksum_progress_(NULL),
     chksum_value_(0),
     index_(0),
     stop_flag_(NULL),
     chksum_level_(0)
{}

ChksumSetting::ChksumSetting(const XML::Node &node)
    :cb_checksum_init_(NULL),
     cb_checksum_progress_(NULL),
     chksum_value_(0),
     index_(0),
     stop_flag_(NULL)
{
    LoadXML(node);
}


ChksumSetting::~ChksumSetting()
{}

BromChksumArg ChksumSetting::GetBromChksumArg() const
{
   BromChksumArg chksumArg;

   if(cb_checksum_init_ != NULL)
   {
       chksumArg.set_init_cb(cb_checksum_init_);
   }

   if(cb_checksum_progress_ != NULL)
   {
       chksumArg.set_chksum_cb(cb_checksum_progress_);
   }

   chksumArg.set_chksum_value(chksum_value_);
   chksumArg.set_rom_index(index_);
   chksumArg.set_stop_flag(stop_flag_);

   return chksumArg;
}

ChksumSetting* ChksumSetting::Clone() const
{
    return new ChksumSetting(*this);
}

void ChksumSetting::LoadXML(const XML::Node &node)
{
    Q_ASSERT(node.GetName() == "checksum-level");
    std::string strChecksumLevel = node.GetText();
    if(strChecksumLevel == "none")
        chksum_level_ = DHNDL_FLAG_CHKSUM_LEVEL_NONE;
    else if(strChecksumLevel == "usb")
        chksum_level_ = DHNDL_FLAG_CHKSUM_LEVEL_USB;
    else if(strChecksumLevel == "storage")
        chksum_level_ = DHNDL_FLAG_CHKSUM_LEVEL_STORAGE;
    else if(strChecksumLevel == "both")
        chksum_level_ = DHNDL_FLAG_CHKSUM_LEVEL_USB | DHNDL_FLAG_CHKSUM_LEVEL_STORAGE;
    else
        chksum_level_ = DHNDL_FLAG_CHKSUM_LEVEL_NONE;
}

void ChksumSetting::SaveXML(XML::Node &node) const
{
    std::string strChecksumLevel = "none";
    switch(chksum_level_)
    {
    case DHNDL_FLAG_CHKSUM_LEVEL_NONE:
        strChecksumLevel = "none";
        break;
    case DHNDL_FLAG_CHKSUM_LEVEL_USB:
        strChecksumLevel = "usb";
        break;
    case DHNDL_FLAG_CHKSUM_LEVEL_STORAGE:
        strChecksumLevel = "storage";
        break;
    case (DHNDL_FLAG_CHKSUM_LEVEL_USB | DHNDL_FLAG_CHKSUM_LEVEL_STORAGE):
        strChecksumLevel = "both";
        break;
    default:
        strChecksumLevel = "none";
    }
    node.AppendChildNode("checksum-level", strChecksumLevel);
}

ChksumSetting* ChksumSetting::FromXML(const XML::Node &node)
{
    return new ChksumSetting(node);
}

}
