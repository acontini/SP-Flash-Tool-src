#include "BromMemtestArg.h"
#include "../Logger/Log.h"
#include <iostream>


int __stdcall BromMemtestArg::cb_memtest_init(void *usr_arg)
{
    LOG("memory test inited...");
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

int __stdcall BromMemtestArg::cb_memtest_progress(unsigned int cur_progress,U64 finished_bytes, U64 total_bytes,
                                                  void *usr_arg)
{
    Q_UNUSED(finished_bytes);
    Q_UNUSED(total_bytes);
    if(usr_arg != NULL)
    {
        FlashTool_MemoryTest_Arg *mt_arg = static_cast<FlashTool_MemoryTest_Arg*>(usr_arg);

        if(mt_arg->m_test_method == HW_MEM_DATA_BUS_TEST)
        {
            LOG("[D%d]", cur_progress);
        }
        else if(mt_arg->m_test_method == HW_MEM_ADDR_BUS_TEST)
        {
            LOG("[A%d]", cur_progress);
        }
        else if(mt_arg->m_test_method == HW_MEM_PATTERN_TEST)
        {
            LOG("(0x%08X),", cur_progress);
        }
    }
    usr_arg = 0;
    return 0;
}

int __stdcall BromMemtestArg::cb_memtest_nand_bad_block_report(U64 bad_block_addr,
                                                      void *usr_arg)
{
    LOG("Bad Block addr: 0x%016I64X", bad_block_addr);
    usr_arg = 0;
    Q_UNUSED(usr_arg);
    return 0;
}

BromMemtestArg::BromMemtestArg()
{
    FillDefaultVaule();
}

void BromMemtestArg::FillDefaultVaule()
{
    memset(&flashtool_memtest_arg_, 0, sizeof(FlashTool_MemoryTest_Arg));

    flashtool_memtest_arg_.m_cb_memorytest_progress_init = BromMemtestArg::cb_memtest_init;
    flashtool_memtest_arg_.m_cb_memorytest_progress_init_arg = NULL;
    flashtool_memtest_arg_.m_cb_memorytest_progress = BromMemtestArg::cb_memtest_progress;
    flashtool_memtest_arg_.m_cb_memorytest_progress_arg = &flashtool_memtest_arg_;
    flashtool_memtest_arg_.m_cb_memorytest_nand_bad_block_report =
            BromMemtestArg::cb_memtest_nand_bad_block_report;
    flashtool_memtest_arg_.m_cb_memorytest_nand_bad_block_report_arg = NULL;

    flashtool_memtest_arg_.m_p_stopflag = NULL;

}
