#include "TempPlcCtrl.h"
#include <QStringList>

bool TempPlcCtrl::isExpire()
{
    // If has expired before, don't need check any more, return true derectly
    if (getHasExpiredBefore() == true)
    {
        LOG("Has Expired Before!!");
        return true;
    }

    time(&m_now);
    LOG("Current time is %s",ctime(&m_now));

    getLastTime(m_last);
    if(m_last == FIRST_LOG_ON_TIME)
    {
        initDeadline();
    }

    // Check deadline
    getDeadline(m_deadline);
    if (m_deadline == -1)
    {
        LOG("Deadline lost");
        return true;
    }

    if(m_now < m_last || m_last < 0)
    {
        LOG("Last launch time is error!! Last launch time %s\n, current time is %s",
            ctime(&m_last),ctime(&m_now));
        return true;
    }
    setLastTime(m_now);

    if (m_now > m_deadline)
    {
        LOG("The temp version tool only can be used before %s\n! Current time is %s",
            ctime(&m_deadline),ctime(&m_now));
        setHasExpiredBefore(true);
        return true;
    }

    return false;
}

//Get last launch time
bool TempPlcCtrl::getLastTime(time_t & _Time)
{
    IniItem item(REG_FILE_NAME, REG_CATEGORY_DATA, REG_ITEM_LASTDATA);
    QString data = item.GetStringValue();

    LOG("Last launch time string: %s",data.toStdString().c_str());

    QString result = encrypt(data);

    _Time = stringToTime(result);

    return _Time > 0;
}

//Get deadline
bool TempPlcCtrl::getDeadline(time_t & _Time)
{
    IniItem item(REG_FILE_NAME, REG_CATEGORY_DATA, REG_ITEM_DEADLINE);
    QString data = item.GetStringValue();

    LOG("Deadline string: %s",data.toStdString().c_str());

    QString result = encrypt(data);

    _Time = stringToTime(result);
    return _Time > 0;
}

//Whether tool has expired before
bool TempPlcCtrl::getHasExpiredBefore()
{
    IniItem item(REG_FILE_NAME, REG_CATEGORY_EXPIRE, REG_ITEM_EXPIRE);
    return item.GetBooleanValue();
}

void TempPlcCtrl::initDeadline()
{
    IniItem item(REG_FILE_NAME, REG_CATEGORY_DATA, REG_ITEM_DEADLINE);
    QString date_str = item.GetStringValue();

    QString data = encrypt(date_str);
    //LOG("Encrypt result: %s", data.toStdString().c_str());

    item.SaveStringValue(data);

}

void TempPlcCtrl::setLastTime(const time_t & _Time)
{
    QString date_str = timeToString(_Time);
    LOG("Date: %s", date_str.toStdString().c_str());

    QString data = encrypt(date_str);
    LOG("Encrypt result: %s", data.toStdString().c_str());

    IniItem item(REG_FILE_NAME, REG_CATEGORY_DATA, REG_ITEM_LASTDATA);
    item.SaveStringValue(data);
}

void TempPlcCtrl::setHasExpiredBefore(const bool expired)
{
    IniItem item(REG_FILE_NAME, REG_CATEGORY_EXPIRE, REG_ITEM_EXPIRE);
    item.SaveBooleanValue(expired);
    LOG("Has expired before: %d", expired);
}

QString TempPlcCtrl::encrypt(const QString & _Src)
{
    QByteArray src = _Src.toAscii();
    QByteArray key = KEY.toAscii();
    int size = src.size() < key.size() ? src.size() : key.size();
    QByteArray des = src;
    for (int i = 0; i < size; ++i)
    {
        des[i] = src[i]^key[i];
    }
    QString _Des = QString::fromAscii(des);
    LOG("Encrypt result: %s", _Des.toStdString().c_str());

    return _Des;
}

QString TempPlcCtrl::timeToString( const time_t & _Time )
{
    LOG("Time: %s",ctime(&_Time));
    struct tm iTm;
    iTm = *localtime(&_Time);

    int day = iTm.tm_mday;
    int mon = iTm.tm_mon+1;
    int year = iTm.tm_year+1900;

    QString date = QString::number(day).append("-")
            .append(QString::number(mon)).append("-")
            .append(QString::number(year));
    LOG("Date: %s",date.toStdString().c_str());

    return date;
}


time_t TempPlcCtrl::stringToTime(const QString & _StrDate)
{

    LOG("Str: %s",_StrDate.toStdString().c_str());
    QStringList list = _StrDate.split("-");
    if (list.size() < 3)
    {
        LOG("Str error ");
        return -1;
    }

    int iYear = list.at(2).toInt();
    int iMonth = list.at(1).toInt();
    int iDay = list.at(0).toInt();

    struct tm sourcedate;
    memset(&sourcedate, 0, sizeof(sourcedate));

    sourcedate.tm_mday = iDay;
    sourcedate.tm_mon = iMonth-1;
    sourcedate.tm_year = iYear-1900;
    sourcedate.tm_isdst = -1;

    time_t time = mktime(&sourcedate);

    LOG("Time: %s",ctime(&time));

    return time;
}
