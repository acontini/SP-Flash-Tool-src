#include "BromReadbackArg.h"
#include "../Logger/Log.h"


int __stdcall BromReadbackArg::cb_readback_init(HW_StorageType_E storage_type,
                                      U64 rb_addr,
                                      U64 rb_len,
                                      const char *rb_file,
                                      void *usr_arg)
{
    LOGI("Readback inited...");
    LOGI("storage(%d), addr(0x%llx), length(0x%llx), file_path(%s)",
        storage_type, rb_addr, rb_len, rb_file);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int __stdcall BromReadbackArg::cb_readback_progress(unsigned char finished_percent,
                                          U64 finished_bytes,
                                          U64 total_bytes,
                                          void *usr_arg)
{
    static unsigned char lastReportedProgress = 0;

    if ((finished_percent != lastReportedProgress) &&
        (finished_percent % 10 == 0)) {
        LOGI("%d%% of data readback to PC,(%llu / %llu)",
            (int) finished_percent, finished_bytes, total_bytes);
        lastReportedProgress = finished_percent;
    }
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

BromReadbackArg::BromReadbackArg()
{
    FillDefaultVaule();
}

void BromReadbackArg::FillDefaultVaule()
{
    memset(&rb_arg,0,sizeof(FlashTool_Readback_Arg));

    rb_arg.m_readback_accuracy = ACCURACY_AUTO;
    rb_arg.m_cb_readback_flash_init = BromReadbackArg::cb_readback_init;
    rb_arg.m_cb_readback_flash_init_arg = NULL;
    rb_arg.m_cb_readback_flash = BromReadbackArg::cb_readback_progress;
    rb_arg.m_cb_readback_flash_arg = NULL;

}
