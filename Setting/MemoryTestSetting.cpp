#include "MemoryTestSetting.h"
#include "../Cmd/MemoryTestCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

MemoryTestSetting::MemoryTestSetting()
    : ram_test_(false),
      ram_data_bus_test_(false),
      ram_addr_bus_test_(false),
      ram_dedicated_pattern_test_(false),
      ram_inc_dec_pattern_test_(false),
      dram_flip_test_(false),
      nand_flash_test_(false),
      nand_dedicated_pattern_test_(false),
      emmc_flash_test_(false),
      emmc_dedicated_pattern_test_(false),
      ufs_flash_test_(false),
      ufs_dedicated_pattern_test_(false),
      cb_memtest_init(NULL),
      cb_memtest_progress(NULL),
      cb_memtest_nand_badblock(NULL),
      cb_gui_(NULL),
      dramtest_start_address(0),
      dramtest_length(0),
      dramtest_stress_cnt(1)
{
}

QSharedPointer<APCore::ICommand> MemoryTestSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::MemoryTestCommand> cmd(new APCore::MemoryTestCommand(key, this));

    if(cb_memtest_init != NULL)
    {
        cmd->memtest_arg().raw_arg()->m_cb_memorytest_progress_init = cb_memtest_init;
    }
    if(cb_memtest_progress != NULL)
    {
        cmd->memtest_arg().raw_arg()->m_cb_memorytest_progress =
                cb_memtest_progress;
        cmd->memtest_arg().raw_arg()->m_cb_memorytest_progress_arg =
                cmd->memtest_arg().raw_arg();
    }
    if(cb_memtest_nand_badblock != NULL)
    {
        cmd->memtest_arg().raw_arg()->m_cb_memorytest_nand_bad_block_report =
                cb_memtest_nand_badblock;
    }

    //this flag MUST be NULL, otherwise mem test can't be stopped.
    cmd->memtest_arg().raw_arg()->m_p_stopflag = NULL;

    LOG("memtest stop flag,(0x%08X)",cmd->memtest_arg().raw_arg()->m_p_stopflag);
    return cmd;
}

void MemoryTestSetting::LoadXML(const XML::Node &node)
{
    //TODO
    Q_ASSERT("MemoryTestSetting::LoadXML was not implemented.");

    LOG("The node name is %s.", node.GetName().c_str());
}

void MemoryTestSetting::SaveXML(XML::Node &node) const
{
    //TODO
    Q_ASSERT("MemoryTestSetting::SaveXML was not implemented.");

    LOG("The node name is %s.", node.GetName().c_str());
}

void MemoryTestSetting::set_dram_flip_test_paras(U64 addr, U64 len, U32 cnt)
{
    dramtest_start_address = addr;
    dramtest_length = len;
    dramtest_stress_cnt = cnt;
}

}
