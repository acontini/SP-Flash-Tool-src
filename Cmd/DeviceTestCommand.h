#ifndef DEVICETESTCOMMAND_H
#define DEVICETESTCOMMAND_H

#include "ICommand.h"
#include "../Setting/DeviceTestSetting.h"
#include "../Setting/WatchDogSetting.h"
#include "../Arg/BromMemtestArg.h"
#include <QColor>
#include <QString>

namespace APCore
{


class DeviceTestCommand : public ICommand
{
public:
    static DeviceTestCommand *instance;

    DeviceTestCommand(APKey key, const DeviceTestSetting *setting);
    virtual ~DeviceTestCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

private:
    friend class DeviceTestSetting;
    DeviceTestSetting test_setting;

    void UpdateUI(const QString& text, QColor color = Qt::black);

    void BatteryTest(const QSharedPointer<Connection> &conn);

    void BatteryTestAndSleep(const QSharedPointer<Connection> &conn, const int type);

    void GetVoltage(FLASHTOOL_API_HANDLE_T ft_handle);
};

}

#endif // MEMORYTESTCOMMAND_H
