#ifndef AUTOFORMATARGFACTORY_H
#define AUTOFORMATARGFACTORY_H


#include <string>
#include <stdexcept>

#include <QSharedPointer>
#include "AutoFormatArg.h"
#include "../Conn/Connection.h"
#include "../ResourceManager/NonCopyable.h"
#include "../Resource/ResourceManager.h"
#include "AutoFormatArg.h"
#include "NORFormatArg.h"
#include "NANDFormatArg.h"
#include "EMMCFormatArg.h"
#include "SDMMCAutoFormatArg.h"
#include "UFSFormatArg.h"

namespace APCore
{
class AutoFormatArgFactory : private NonCopyable
{
public:
    AutoFormatArgFactory() {}

    ~AutoFormatArgFactory() {}

    QSharedPointer<AutoFormatArg> CreateAutoFormatArg(const HW_StorageType_E& type,const APKey& key,const QSharedPointer<Connection> & conn, const U32 part_id = 0);
private:
    void Dump(void);
private:
};
}
#endif // AUTOFORMATARGFACTORY_H
