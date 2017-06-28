#ifndef WATCHDOGSETTING_H
#define WATCHDOGSETTING_H

#include "ISetting.h"

namespace APCore
{

class WatchDogSetting : public ISetting
{
public:
    WatchDogSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;
/*
    WatchDogSetting* clone() const
    {
        return new WatchDogSetting(*this);
    }
  // "clone" is not called ever
  // and, what's the point of non-virtual clone?
*/
};

}
#endif // WATCHDOGSETTING_H
