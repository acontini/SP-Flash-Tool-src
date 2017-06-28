#ifndef SETBOOTMODECOMMAND_H
#define SETBOOTMODECOMMAND_H

#include "ICommand.h"

namespace APCore
{

class SetBootModeCommand : public ICommand
{
public:
    SetBootModeCommand(APKey key);
    virtual ~SetBootModeCommand();

    virtual void exec(const QSharedPointer<APCore::Connection> &conn);

    void setBootMode(unsigned char mode) {boot_mode_ = mode;}
    void setComType(unsigned char comtype) {com_type_ = comtype;}
    void setComId(unsigned char comid) {com_id_ = comid;}

private:
    unsigned char boot_mode_; //0:normal, 1:meta
    unsigned char com_type_;  //1: uart, 2:usb, 3:wifi
    unsigned char com_id_;    //bit0: adb on/off;  bit1: mobile log on/off;   0:on, 1: off
};

}

#endif // SETBOOTMODECOMMAND_H
