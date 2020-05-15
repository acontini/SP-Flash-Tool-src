#ifndef READBACKSETTING_H
#define READBACKSETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"
#include "../Public/AppTypes.h"
#include "../Public/AppCore.h"
#include <iostream>
#include <list>

namespace APCore
{

class ReadbackSetting : public ISetting
{
public:
    ReadbackSetting();

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    void set_cb_readback_init(CALLBACK_READBACK_PROGRESS_INIT cb)
    {
        cb_readback_init_ = cb;
    }

    void set_cb_readback_progress(CALLBACK_READBACK_PROGRESS cb)
    {
        cb_readback_progress_ = cb;
    }

    std::list<ReadbackItem> GetReadbackItems() const
    {
        return items;
    }

    bool IsPhysicalReadbackEnabled() const
    {
        return m_IsphysicalReadback;
    }

    void set_physicalReadbackEnabled(bool enable)
    {
        m_IsphysicalReadback = enable;
    }

    void set_readbackItems(std::list<ReadbackItem>& _items)
    {
        items = _items;
    }

    void AppendReadbackItem(const ReadbackItem& item);

private:
    std::list<ReadbackItem> items;
    bool m_IsphysicalReadback;

    CALLBACK_READBACK_PROGRESS_INIT cb_readback_init_;
    CALLBACK_READBACK_PROGRESS cb_readback_progress_;

};

}

#endif // READBACKSETTING_H
