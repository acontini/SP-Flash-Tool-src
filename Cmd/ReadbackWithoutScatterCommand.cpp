#include "ReadbackWithoutScatterCommand.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../BootRom/flashtoolex_api.h"
#include "../BootRom/type_define.h"

namespace APCore
{

ReadbackWithoutScatterCommand::ReadbackWithoutScatterCommand(APKey key):
    ICommand(key)
{
}

ReadbackWithoutScatterCommand::~ReadbackWithoutScatterCommand()
{

}

op_part_list_t* ReadbackWithoutScatterCommand::get_partion_list()
{
    int count = part_list_.size();

    op_part_list_t *list = new op_part_list_t[count];
    if (list){
        for(int i = 0; i < count; i++){
            op_part_list_t item = part_list_.at(i);

            strncpy(list[i].part_name, item.part_name, strlen(item.part_name) + 1);
            strncpy(list[i].file_path, item.file_path, strlen(item.file_path) + 1);
        }
    }

    return list;
}

void ReadbackWithoutScatterCommand::UpdateUploadPartitionInfo(const op_part_list_t* flist, partition_info_list_t* p_part_list)
{
    int count = part_list_.size();
    for(int i = 0; i < count; i++)
    {
        if(strcmp(p_part_list->part[i].part_name, flist[i].part_name) == 0
                && strcmp(p_part_list->part[i].file_name, flist[i].file_path) != 0)
        {
            strncpy(p_part_list->part[i].file_name, flist[i].file_path, strlen(flist[i].file_path) + 1);
            continue;
        }
    }
}

void ReadbackWithoutScatterCommand::exec(const QSharedPointer<Connection> &conn){
    Q_UNUSED(conn);

    HSESSION hs;

    int status = STATUS_OK;
    partition_info_list_t partInfoList;
    op_part_list_t* flist = get_partion_list();
    callbacks_struct_t cbs;
    memset(&cbs, 0, sizeof(cbs));

    if(connect(&hs) != STATUS_OK){
        goto exit;
    }

    status = flashtool_read_partition_table(hs, &partInfoList);
    if(status != S_DONE)
    {
        goto exit;
    }
    
    UpdateUploadPartitionInfo(flist, &partInfoList);
    cbs.cb_op_progress = cb_rb_operation_progress;
    cbs.cb_stage_message = cb_rb_stage_message;

    //conn->ConnectDA();

    LOGI("Begin executing readback without scatter command...");

    status = flashtool_upload(
                hs,
                &partInfoList,
                &cbs);

    LOGI("Readback result(%d)",status);

    if(status != S_DONE)
    {
        goto exit;
    }

    LOGI("Readback Succeeded.");

exit:
    delete flist;
    flashtool_destroy_session(hs);

    if(status != S_DONE)
    {
        THROW_BROM_EXCEPTION(status,0);
    }
}

void ReadbackWithoutScatterCommand::cb_rb_operation_progress(void* arg, enum transfer_phase phase,
                                                             unsigned int progress,
                                                             unsigned long long data_xferd,
                                                             unsigned long long data_total,
                                                             const struct cbs_additional_info * cbs_add_info)
{
    Q_UNUSED(arg);
    Q_UNUSED(phase);
    Q_UNUSED(data_xferd);
    Q_UNUSED(data_total);
    Q_UNUSED(cbs_add_info);

    LOGI("Readback percent: %d", progress);
}

void ReadbackWithoutScatterCommand::cb_rb_stage_message(void *usr_arg, const char *message)
{
    Q_UNUSED(usr_arg);

    std::cout<< "Stage: " << message << std::endl;
}

}
