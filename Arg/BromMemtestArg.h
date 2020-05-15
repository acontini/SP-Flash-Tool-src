#ifndef BROMMEMTESTARG_H
#define BROMMEMTESTARG_H

#include "../BootRom/flashtool_api.h"

class BromMemtestArg
{
public:
    BromMemtestArg();

    FlashTool_MemoryTest_Arg* raw_arg()
    {
        return &flashtool_memtest_arg_;
    }

    static int __stdcall cb_memtest_init(void *usr_arg);
    static int __stdcall cb_memtest_progress(unsigned int cur_progress, U64 finished_bytes, U64 total_bytes, void *usr_arg);
    static int __stdcall cb_memtest_nand_bad_block_report(U64 bad_block_addr,
                                                          void *usr_arg);
private:
    void FillDefaultVaule();

    FlashTool_MemoryTest_Arg flashtool_memtest_arg_;
};

#endif // BROMMEMTESTARG_H
