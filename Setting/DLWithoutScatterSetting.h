#ifndef DLWITHOUTSCATTERSETTING_H
#define DLWITHOUTSCATTERSETTING_H

#include "ISetting.h"
#include "./BootRom/flashtoolex_struct.h"

namespace APCore
{

class DLWithoutScatterSetting : public ISetting
{
public:
    DLWithoutScatterSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    DLWithoutScatterSetting* clone() const {
        return new DLWithoutScatterSetting(*this);
    }

    void SetPartionList(std::vector<op_part_list_t> &items){
        partList_ = items;
    }

    void set_cb_operation_progress(CB_OPERATION_PROGRESS cb){
        cb_operation_progress_ = cb;
    }

    void set_cb_stage_message(CB_STAGE_MESSAGE cb){
        cb_stage_message_ = cb;
    }

private:
    std::vector<op_part_list_t>   partList_;
    CB_OPERATION_PROGRESS       cb_operation_progress_;
    CB_STAGE_MESSAGE            cb_stage_message_;
};

}

#endif // DLWITHOUTSCATTERSETTING_H
