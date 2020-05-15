#include "USBSetting.h"
#include "../XMLParser/XMLDocument.h"
#include "../XMLParser/XMLNode.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"

#include "../Utility/FileUtils.h"
#include "../Host/Inc/RuntimeMemory.h"

#include <QtGlobal>
#include <QTime>

#define LGE_PRELOADER_SUPPORT

QSharedPointer<USBSetting> USBSetting::inst;

USBSetting::USBSetting(const std::string &setting_file, bool CertDL):
    is_certDL(CertDL),prefer_com_port_(-1), forbid_stop_(false)
{
    XML::Document document(setting_file);
    const XML::Node root_node = document.GetRootNode();
    Q_ASSERT(root_node.GetName() == "usb-setting");

    XML::Node child_node = root_node.GetFirstChildNode();

    USBInfoPair infoPair;
    int id_index = BOOTROM_VCOM;

    while(!child_node.IsEmpty())
    {
        Q_ASSERT(child_node.GetName() == "usb");

        const std::string type = child_node.GetAttribute("name");

        infoPair.device_pid = child_node.GetAttribute("pid");
        infoPair.device_vid = child_node.GetAttribute("vid");
        infoPair.guid_type = child_node.GetAttribute("guid-type");

        if(type == "BOOTROM_VCOM")
        {
            id_index = BOOTROM_VCOM;
        }
        else if(type == "PRELOADER_VCOM")
        {
            id_index = PRELOADER_VCOM;
        }
        else if(type == "DA_HIGHSPEED_VCOM")
        {
           id_index = DA_HIGHSPEED_VCOM;
        }
        else if(type == "COMPOSITE_PRELOADER_VCOM")
        {
            id_index= COMPOSITE_PRELOADER_VCOM;
        }
        else if(type == "COMPOSITE_DA_VCOM")
        {
           id_index = COMPOSITE_DA_VCOM;
        }

        usbID_map_[id_index] = infoPair;

        child_node = child_node.GetNextSibling();
    }
}

USBSetting *USBSetting::instance(bool CertDL /*= false*/)
{
    if(inst == NULL)
    {
        inst = QSharedPointer<USBSetting>(
                    new USBSetting(ABS_PATH("usb_setting.xml"), CertDL));
    }
    return inst.data();
}

void USBSetting::AddUSBInstance(USBInfoPair info)
{
     this->usb_port_pool_.push_back(info);
}

void USBSetting::AddUSBPool(usb_speed speedType)
{
    usb_port_pool_.clear();

    switch(speedType)
    {
    case NORMAL_SPEED:

        AddUSBInstance(usbID_map_[BOOTROM_VCOM]);

        if(!is_certDL)
        {
            AddUSBInstance(usbID_map_[PRELOADER_VCOM]);
#ifdef LGE_PRELOADER_SUPPORT
            AddUSBInstance(usbID_map_[COMPOSITE_PRELOADER_VCOM]);
#endif
        }

        break;

    case DA_HIGH_SPEED:

        if(!is_certDL)
        {
            AddUSBInstance(usbID_map_[DA_HIGHSPEED_VCOM]);
#ifdef LGE_PRELOADER_SUPPORT
            AddUSBInstance(usbID_map_[COMPOSITE_DA_VCOM]);
#endif
        }

        break;
    }
}

bool USBSetting::Search(int *stop_flag, int timeout)
{
    bool is_found = false;

    std::vector<USBID> id_array(this->usb_port_pool_.size());
    USBID *p_usb_id_array = static_cast<USBID*>(&id_array[0]);

    std::list<USBInfoPair>::iterator it = this->usb_port_pool_.begin();
    for (size_t i = 0; it != this->usb_port_pool_.end(); it++, i++)
    {
        p_usb_id_array[i].usb_pid = it->device_pid.c_str();
        p_usb_id_array[i].usb_vid = it->device_vid.c_str();
        p_usb_id_array[i].guid_type = it->guid_type.c_str();
    }

    USBScanArg usb_scan_arg = {const_cast<const USBID *>(p_usb_id_array),
                               id_array.size(),timeout};
    USBScanResult usb_scan_result = {"", "", stop_flag};

    if (SearchUSBPortPool(&usb_scan_arg, &usb_scan_result))
    {
        is_found = true;
    }

    LOGD("Scaning USB port %s", is_found?"succeeded!":"failed!");
    return is_found;
}

bool USBSetting::SearchUSBPortPool(USBScanArg *usb_scan_arg,
                                   USBScanResult *usb_scan_result)
{
    bool is_found = false;

    if(prefer_com_port_ <= 0)
    {
        is_found = FindUSBDevice(usb_scan_arg, usb_scan_result, forbid_stop_);
    }
    else
    {
        LOGI("Searching user specified com port COM%d",prefer_com_port_);
        is_found = FindSpecialUSBDevice(usb_scan_arg, usb_scan_result, prefer_com_port_);
    }

    if (is_found) {
        this->usb_device_path_ = usb_scan_result->p_symbolic_name;
        this->usb_friendly_name_ = usb_scan_result->p_friendly_name;
        LOGI("USB port is obtained. path name(%s), port name(%s)",
             this->usb_device_path_.c_str(),
             this->usb_friendly_name_.c_str());
    } else {
        LOGE("SearchUSBPortPool failed!");
    }

    //reset forbid_stop_ flag, otherwise next dl will timeout in 5s when search comport
    forbid_stop_=false;
    return is_found;
}
