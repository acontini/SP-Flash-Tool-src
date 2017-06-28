#ifndef MEMORYTESTSETTING_H
#define MEMORYTESTSETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"

#include <QString>
#include <QColor>

namespace APCore
{

typedef void (*MEMORY_TEST_UI_CALLBACK)(const QString&,QColor);

class MemoryTestSetting : public ISetting
{
public:
    MemoryTestSetting();
    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    //getters
    bool ram_test() { return ram_test_; }
    bool ram_data_bus_test() { return ram_data_bus_test_; }
    bool ram_addr_bus_test() { return ram_addr_bus_test_; }
    bool ram_dedicated_pattern_test() { return ram_dedicated_pattern_test_; }
    bool ram_inc_dec_pattern_test() { return ram_inc_dec_pattern_test_; }
    bool dram_flip_test() { return dram_flip_test_; }

    bool nand_flash_test() { return nand_flash_test_; }
    bool nand_dedicated_pattern_test() { return nand_dedicated_pattern_test_; }

    bool emmc_flash_test() { return emmc_flash_test_; }
    bool emmc_dedicated_pattern_test() { return emmc_dedicated_pattern_test_; }

    bool ufs_flash_test() { return ufs_flash_test_; }
    bool ufs_dedicated_pattern_test() { return ufs_dedicated_pattern_test_; }

    //setters
    void set_ram_test(bool enable) { ram_test_ = enable; }
    void set_ram_data_bus_test(bool enable) { ram_data_bus_test_ = enable; }
    void set_ram_addr_bus_test(bool enable) { ram_addr_bus_test_ = enable; }
    void set_ram_dedicated_pattern_test(bool enable) { ram_dedicated_pattern_test_ = enable; }
    void set_ram_inc_dec_pattern_test(bool enable) { ram_inc_dec_pattern_test_ = enable; }
    void set_dram_flip_test(bool enable) { dram_flip_test_ = enable; }

    void set_nand_flash_test(bool enable) { nand_flash_test_ = enable; }
    void set_nand_dedicated_pattern_test(bool enable) { nand_dedicated_pattern_test_ = enable; }

    void set_emmc_flash_test(bool enable) { emmc_flash_test_ = enable; }
    void set_emmc_dedicated_pattern_test(bool enable) { emmc_dedicated_pattern_test_ = enable; }

    void set_ufs_flash_test(bool enable) { ufs_flash_test_ = enable; }
    void set_ufs_dedicated_pattern_test(bool enable) { ufs_dedicated_pattern_test_ = enable; }

    void set_cb_gui(MEMORY_TEST_UI_CALLBACK cb) { cb_gui_ = cb; }
    void set_cb_memtest_init(CALLBACK_MEMORYTEST_PROGRESS_INIT cb){ cb_memtest_init = cb;}
    void set_cb_memtest_progress(CALLBACK_MEMORYTEST_PROGRESS cb){ cb_memtest_progress = cb;}
    void set_cb_memtest_nand_badblock(CALLBACK_MEMORYTEST_NAND_BAD_BLOCK_REPORT cb)
    {
        cb_memtest_nand_badblock = cb;
    }
    MEMORY_TEST_UI_CALLBACK cb_gui(){ return cb_gui_;}
    void set_dram_flip_test_paras(U64 addr, U64 len, U32 cnt);
    U64 start_address(){return dramtest_start_address;}
    U64 test_length(){return dramtest_length;}
    U32 test_cnt(){return dramtest_stress_cnt;}

private:
    bool ram_test_;
    bool ram_data_bus_test_;
    bool ram_addr_bus_test_;
    bool ram_dedicated_pattern_test_;
    bool ram_inc_dec_pattern_test_;
    bool dram_flip_test_;

    bool nand_flash_test_;
    bool nand_dedicated_pattern_test_;

    bool emmc_flash_test_;
    bool emmc_dedicated_pattern_test_;

    bool ufs_flash_test_;
    bool ufs_dedicated_pattern_test_;

    CALLBACK_MEMORYTEST_PROGRESS_INIT cb_memtest_init;
    CALLBACK_MEMORYTEST_PROGRESS cb_memtest_progress;
    CALLBACK_MEMORYTEST_NAND_BAD_BLOCK_REPORT cb_memtest_nand_badblock;

    MEMORY_TEST_UI_CALLBACK cb_gui_;

    U64 dramtest_start_address;
    U64 dramtest_length;
    U32 dramtest_stress_cnt;
};

}

#endif // MEMORYTESTSETTING_H
