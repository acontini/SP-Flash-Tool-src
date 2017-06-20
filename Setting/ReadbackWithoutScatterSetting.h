#ifndef READBACKWITHOUTSCATTERSETTING_H
#define READBACKWITHOUTSCATTERSETTING_H

#include "ISetting.h"
#include "./BootRom/flashtoolex_struct.h"

namespace APCore
{

class ReadbackWithoutScatterSetting : public ISetting
{
public:
    ReadbackWithoutScatterSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    ReadbackWithoutScatterSetting* clone() const {
          return new ReadbackWithoutScatterSetting(*this);
    }

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

private:
    std::vector<op_part_list_t> items;
};

}
#endif // READBACKWITHOUTSCATTERSETTING_H
