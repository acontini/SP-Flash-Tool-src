#include "DeviceTestCommand.h"
#include "../Err/Exception.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"

#include "../Logger/Log.h"
#include <QCoreApplication>
#include <QTime>

namespace APCore
{


DeviceTestCommand *DeviceTestCommand::instance = NULL;

inline int CHECK_TEST_RESULT(int ret)
{
    LOG("checking flashtoollib ret value(%d)",ret);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }
    return ret;
}

DeviceTestCommand::DeviceTestCommand(APKey key, const DeviceTestSetting *setting)
    : ICommand(key)
{
    if(setting != NULL)
    {
        test_setting = *setting;
    }
    DeviceTestCommand::instance = this;
}

DeviceTestCommand::~DeviceTestCommand()
{

}

void DeviceTestCommand::exec(const QSharedPointer<Connection> &conn)
{
    if(test_setting.is_battery_test())
    {
        BatteryTest(conn);
    }
}

void DeviceTestCommand::UpdateUI(const QString& text, QColor color)
{
    DEVICE_TEST_UI_CALLBACK callback = test_setting.cb_gui();
    if(callback != NULL)
    {
        callback(text, color);
    }
}

void DeviceTestCommand::BatteryTest(const QSharedPointer<Connection> &conn)
{
    UpdateUI("============\t Battery Test \t============\n", Qt::cyan);
    if(test_setting.is_with_battery())
    {
        UpdateUI("======\t With Battery \t======\n", Qt::darkBlue);
        BatteryTestAndSleep(conn, FORCE_CHARGE_OFF);
    }

    if(test_setting.is_without_battery())
    {
        UpdateUI("======\t Without Battery \t======\n", Qt::darkBlue);
        BatteryTestAndSleep(conn, FORCE_CHARGE_ON);
    }

    if(test_setting.is_auto_detect())
    {
        UpdateUI("======\t Auto Detect Battery \t======\n", Qt::darkBlue);
        BatteryTestAndSleep(conn, FORCE_CHARGE_AUTO);
    }

    UpdateUI("============\t Battery Test Done!! \t============\n", Qt::cyan);
}


void DeviceTestCommand::BatteryTestAndSleep(const QSharedPointer<Connection> &conn, const int type)
{
    LOG("BatteryTest, type = %d!", type);
    if(conn->isDAConnected())
    {
        QSharedPointer<APCore::WatchDogSetting> ws(new APCore::WatchDogSetting());
        ws->CreateCommand(key_)->exec(conn);

        conn->Disconnect();

        QTime time;
        time.start();
        while(time.elapsed() < 500)
        {
           QCoreApplication::processEvents();
        }
        LOGD("BatteryTest disconnect,wait 500ms end!");

    }
    conn->setPowerType(type);
    conn->ConnectDA();
    GetVoltage(conn->FTHandle());
}

void DeviceTestCommand::GetVoltage(FLASHTOOL_API_HANDLE_T ft_handle)
{
    FlashTool_CheckBattery_Result bt_result;
    memset(&bt_result, 0, sizeof(bt_result));
    int ret = FlashTool_Check_Battery(ft_handle,NULL, &bt_result);
    LOG("ret = %d, GetVoltage = %d!", ret, bt_result.bat_voltage_value);
    if(ret == S_UNSUPPORTED_OPERATION)
    {
        UpdateUI("Battery is nonexistence OR Function is not supported!\n", Qt::darkYellow);
    }
    else if ( ret == S_DONE)
    {
        UpdateUI(QString().sprintf("Battery is %d\n", bt_result.bat_voltage_value), Qt::darkBlue);
    }
    else
    {
        CHECK_TEST_RESULT(ret);
    }
}

}
