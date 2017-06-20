#ifndef DLWITHOUTSCATTERCOMMAND_H
#define DLWITHOUTSCATTERCOMMAND_H

#include "ICommand.h"
#include "../BootRom/flashtoolex_struct.h"
#include <vector>
#include "../Conn/Connection.h"
#include "../Public/AppTypes.h"

namespace APCore
{
class DLWithoutScatterCommand : public ICommand
{
public:
    DLWithoutScatterCommand(APKey key);
    virtual ~DLWithoutScatterCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_partion_list(std::vector<op_part_list_t> items){
        part_list_ = items;
    }

    void set_cb_op_progress(CB_OPERATION_PROGRESS cb){
        cb_op_progress_ = cb;
    }

    void set_cb_stage_message(CB_STAGE_MESSAGE cb){
        cb_stage_message_ = cb;
    }

    static void __stdcall cb_dl_operation_progress(void* arg, transfer_phase phase,
                                                   unsigned int progress, unsigned long long data_xferd,
                                                   unsigned long long data_total,
                                                   const struct cbs_additional_info * cbs_add_info = NULL);

    static void __stdcall cb_dl_stage_message(void *usr_arg, const char* message);

private:
    op_part_list_t* get_partion_list();

private:
    std::vector<op_part_list_t> part_list_;
    CB_OPERATION_PROGRESS cb_op_progress_;
    CB_STAGE_MESSAGE cb_stage_message_;
    std::string da_;
};

}
#endif // DLWITHOUTSCATTERCOMMAND_H
