#include "DLWithoutScatterCommand.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../BootRom/flashtoolex_api.h"
#include "../BootRom/type_define.h"
#include "../BootRom/mtk_status.h"

namespace APCore
{

DLWithoutScatterCommand::DLWithoutScatterCommand(APKey key):
    ICommand(key),
    cb_op_progress_(NULL),
    cb_stage_message_(NULL)
{
}

DLWithoutScatterCommand::~DLWithoutScatterCommand()
{

}

op_part_list_t* DLWithoutScatterCommand::get_partion_list()
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

void DLWithoutScatterCommand::exec(const QSharedPointer<Connection> &conn){
  //  conn->ConnectDA();
    Q_UNUSED(conn);

    HSESSION hs;

    int status = STATUS_OK;

    op_part_list_t* flist = get_partion_list();

    callbacks_struct_t cbs;
    memset(&cbs, 0, sizeof(cbs));

    cbs.cb_op_progress = cb_op_progress_ == NULL ? cb_dl_operation_progress : cb_op_progress_;

    cbs.cb_stage_message = cb_stage_message_ == NULL ? cb_dl_stage_message: cb_stage_message_;

    if(connect(&hs) != STATUS_OK){
        goto exit;
    }

#if 0
    status_t status = STATUS_OK;

    char* filter[3] =
    {
        "USB\\VID_0E8D&PID_0003",
        "USB\\VID_0E8D&PID_2000",
        "USB\\VID_0E8D&PID_2001"
    };

    char port_name[256] = {0};
    std::cout << "Wait for device. Plug in USB." <<std::endl;
    status = flashtool_waitfor_com(filter, 3, port_name);
    if(status != STATUS_OK)
    {
        goto exit;
    }

    HSESSION hs = flashtool_create_session();
    if(hs == INVALID_HSESSION_VALUE)
    {
        goto exit;
    }

    std::cout << "Connect port: " <<port_name <<std::endl;
    status = flashtool_connect_brom(hs, port_name, NULL, NULL, NULL);
    if(status != STATUS_OK)
    {
        goto exit;
    }

    connect_params_struct_t conn_da_params;
    conn_da_params.checksum_level = 0;
    conn_da_params.battery_setting = 0;
    status = flashtool_connect_da(hs, (char*)da_.c_str(), NULL, &conn_da_params, NULL);
    if(status != STATUS_OK)
    {
        goto exit;
    }

    status = flashtool_switch_com(hs, filter, 3);

    if(status != STATUS_OK)
    {
        goto exit;
    }
#endif

    LOGI("Begin executing download without scatter command...");

    status = flashtool_download(
                hs,
                NULL,
                flist,
                part_list_.size(),
                &cbs);

    LOGI("Download result(%d)",status);

    if(status != STATUS_OK)
    {
        goto exit;
    }

    LOGI("Download Succeeded.");

exit:
    delete flist;

    flashtool_destroy_session(hs);

    if(status != STATUS_OK)
    {
       THROW_BROM_EXCEPTION(status,0);
    }
}

void DLWithoutScatterCommand::cb_dl_operation_progress(void* arg, enum transfer_phase phase,
                                                       unsigned int progress,
                                                       unsigned long long data_xferd,
                                                       unsigned long long data_total,
                                                       const struct cbs_additional_info * cbs_add_info)
{
    Q_UNUSED(arg);
    Q_UNUSED(data_xferd);
    Q_UNUSED(data_total);
    Q_UNUSED(cbs_add_info);
   // unsigned long long  total_bytes = static_cast<unsigned long long>(static_cast<float>(data_xferd)*progress/100);
    if(phase == TPHASE_LOADER)
    {
        if(progress == 0)
        {
            LOGI("Download Boot Loader 0%");
        }
        else
        {
            LOGI("Download Boot Loader %d", progress);
        }
    }
    else
    {
        if(progress == 0)
        {
            LOGI("Download Flash 0%");
        }
        else
        {
            LOGI("Download Flash %d", progress);
        }
    }
}

void DLWithoutScatterCommand::cb_dl_stage_message(void *usr_arg, const char *message)
{
    Q_UNUSED(usr_arg);

    std::cout<< "Stage: " << message << std::endl;
}

}

