#ifndef SETBOOTMODESETTING_H
#define SETBOOTMODESETTING_H

#include "ISetting.h"

namespace APCore
{

class SetBootModeSetting : public ISetting
{
public:
    SetBootModeSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    void setBootMode(unsigned char boot_mode) {boot_mode_ = boot_mode;}
    void setComType(unsigned char com_type) {com_type_ = com_type;}
    void setComId(unsigned char com_id) {com_id_ = com_id;}

private:
    unsigned char boot_mode_; //0:normal, 1:meta
    unsigned char com_type_;  //1: usb, 2:uart
    unsigned char com_id_;    //bit0: adb on/off;  bit1: mobile log on/off;   0:on, 1: off
};

}

#endif // SETBOOTMODESETTING_H
