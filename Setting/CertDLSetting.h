#ifndef CERTDLSETTING_H
#define CERTDLSETTING_H

#include "ISetting.h"

namespace APCore
{

class CertDLSetting : public ISetting
{
public:
    CertDLSetting();

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    CertDLSetting* clone() const
    {
        return new CertDLSetting(*this);
    }
};

}

#endif // CERTDLSETTING_H
