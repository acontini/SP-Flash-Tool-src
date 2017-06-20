#ifndef CHECKRPMBSETTING_H
#define CHECKRPMBSETTING_H

#include "ISetting.h"

namespace APCore
{

class CheckRPMBSetting : public ISetting
{
public:
    CheckRPMBSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;
};

}

#endif // SETBOOTMODESETTING_H
