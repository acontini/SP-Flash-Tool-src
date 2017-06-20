#include "BromAdapterArg.h"
#include "../Logger/Log.h"
#include <iostream>

BromAdapterArg::BromAdapterArg()
{
    this->FillDefaultValue();
}

BromAdapterArg::BromAdapterArg(const FlashTool_BROM_DL_Arg& dl_arg)
{
    memset(&brom_dl_arg_, 0, sizeof(FlashTool_BROM_DL_Arg));

    this->brom_dl_arg_.begin_addr = dl_arg.begin_addr;
    this->brom_dl_arg_.dl_data_buffer = dl_arg.dl_data_buffer;
    this->brom_dl_arg_.dl_data_len = dl_arg.dl_data_len;
    this->brom_dl_arg_.is_jump = dl_arg.is_jump;
    this->brom_dl_arg_.jump_addr = dl_arg.jump_addr;
    this->brom_dl_arg_.m_cb_init = dl_arg.m_cb_init;
    this->brom_dl_arg_.m_cb_init_arg = dl_arg.m_cb_init_arg;
    this->brom_dl_arg_.m_cb_progress = dl_arg.m_cb_progress;
    this->brom_dl_arg_.m_cb_progress_arg = dl_arg.m_cb_progress_arg;
}

void BromAdapterArg::FillDefaultValue()
{
    memset(&brom_dl_arg_, 0, sizeof(FlashTool_BROM_DL_Arg));
    this->brom_dl_arg_.begin_addr = 0;
    this->brom_dl_arg_.dl_data_buffer = NULL;
    this->brom_dl_arg_.dl_data_len = 0;
    this->brom_dl_arg_.is_jump = false;
    this->brom_dl_arg_.jump_addr = 0;
    this->brom_dl_arg_.m_cb_init = BromDownload_Progress_Init;
    this->brom_dl_arg_.m_cb_init_arg = NULL;
    this->brom_dl_arg_.m_cb_progress = BromDownload_Progress;
    this->brom_dl_arg_.m_cb_progress_arg = NULL;
}

int BromAdapterArg::BromDownload_Progress_Init(void *usr_arg)
{
    LOGI("Begin Brom download");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int BromAdapterArg::BromDownload_Progress(unsigned char finished_percentage, unsigned int sent_bytes,
                                          unsigned int total_bytes, void *usr_arg)
{
    static unsigned char lastReportedProgress = 0;

    if ((finished_percentage != lastReportedProgress) &&
        (finished_percentage % 10 == 0)) {
        LOGI("%d%% of data has been download,(%d / %d)",
            (int) finished_percentage, sent_bytes, total_bytes);
        lastReportedProgress = finished_percentage;
    }

    usr_arg = 0;
    Q_UNUSED(usr_arg);

    return 0;
}
