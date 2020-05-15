#ifndef READBACKWITHOUTSCATTERCOMMAND_H
#define READBACKWITHOUTSCATTERCOMMAND_H

#include "ICommand.h"
#include "../Conn/Connection.h"
#include "../Public/AppTypes.h"
#include "../BootRom/flashtoolex_struct.h"
#include <vector>

namespace APCore
{
class ReadbackWithoutScatterCommand : public ICommand
{
public:
    ReadbackWithoutScatterCommand(APKey key);
    virtual ~ReadbackWithoutScatterCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_partion_list(std::vector<op_part_list_t> items){
        part_list_ = items;
    }

    static void __stdcall cb_rb_operation_progress(void* arg, enum transfer_phase phase,
                                                   unsigned int progress,
                                                   unsigned long long data_xferd,
                                                   unsigned long long data_total,
                                                   const struct cbs_additional_info * cbs_add_info = NULL);

    static void __stdcall cb_rb_stage_message(void *usr_arg, const char* message);

private:
    op_part_list_t* get_partion_list();
    void UpdateUploadPartitionInfo(partition_info_list_t* pReadback_part_list, const op_part_list_t* flist, partition_info_list_t* p_part_list);

private:
    std::vector<op_part_list_t> part_list_;
};

}
#endif // READBACKWITHOUTSCATTERCOMMAND_H
