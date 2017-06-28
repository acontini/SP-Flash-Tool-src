#ifndef TEMPPLCCTRL_H
#define TEMPPLCCTRL_H

#include <time.h>
#include "../Logger/Log.h"
#include "../Utility/IniItem.h"

//Deadline format: day-month-year
static const long FIRST_LOG_ON_TIME = 946656000;//1-1-2000
static const QString REG_FILE_NAME = "registry.ini";
static const QString REG_CATEGORY_EXPIRE = "Expire";
static const QString REG_ITEM_EXPIRE = "hasExpired";
static const QString REG_CATEGORY_DATA = "Data";
static const QString REG_ITEM_DEADLINE = "Deadline";
static const QString REG_ITEM_LASTDATA = "LastLaunch";
static const QString KEY = "MTK_TMP_Key";

/**
 * Temp version product life cycle controller.
 * Before release, must set deadline in registry.ini, then run flashtool.
 * (This step would encrypt registry.ini)
**/

class TempPlcCtrl
{
public:
    TempPlcCtrl(){}
    bool isExpire();
private:
    bool getLastTime(time_t & _Time);
    bool getDeadline(time_t & _Time);
    bool getHasExpiredBefore();
    void initDeadline();
    void setLastTime(const time_t & _Time);
    void setHasExpiredBefore(const bool expired);
    QString encrypt(const QString & _Src);
    QString timeToString(const time_t & _Time );
    time_t stringToTime(const QString & _StrDate);

    time_t m_now;
    time_t m_last;
    time_t m_deadline;
};

#endif // TEMPPLCCTRL_H
