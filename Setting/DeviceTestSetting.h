#ifndef DEVICETESTSETTING_H
#define DEVICETESTSETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"

#include <QString>
#include <QColor>

namespace APCore
{

typedef void (*DEVICE_TEST_UI_CALLBACK)(const QString&,QColor);

class DeviceTestSetting : public ISetting
{
public:
    DeviceTestSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    //getters
    void set_cb_gui(DEVICE_TEST_UI_CALLBACK cb) { cb_gui_ = cb; }
    void set_battery_test(bool enable) { battery_test_ = enable; }
    bool is_battery_test() { return battery_test_ ;}
    void set_with_battery(bool enable) { with_battery_ = enable; }
    bool is_with_battery() { return with_battery_ ;}
    void set_without_battery(bool enable) { without_battery_ = enable; }
    bool is_without_battery() { return without_battery_ ;}
    void set_auto_detect(bool enable) { auto_detect_ = enable; }
    bool is_auto_detect() { return auto_detect_ ;}
    DEVICE_TEST_UI_CALLBACK cb_gui(){ return cb_gui_;}

private:
    DEVICE_TEST_UI_CALLBACK cb_gui_;

    bool battery_test_;
    bool with_battery_;
    bool without_battery_;
    bool auto_detect_;
};

}

#endif // DEVICETESTSETTING_H
