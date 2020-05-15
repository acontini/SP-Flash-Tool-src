#ifndef MEMORYTESTCOMMAND_H
#define MEMORYTESTCOMMAND_H

#include "ICommand.h"
#include "../Setting/MemoryTestSetting.h"
#include "../Arg/BromMemtestArg.h"
#include <QColor>
#include <QString>

namespace APCore
{


class MemoryTestCommand : public ICommand
{
public:
    static int __stdcall cb_memorytest_progress(unsigned int progress, U64 finished_bytes, U64 total_bytes, void *usr_arg);
    static MemoryTestCommand *instance;

    MemoryTestCommand(APKey key, const MemoryTestSetting *setting);
    virtual ~MemoryTestCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);

    BromMemtestArg& memtest_arg() { return memtest_arg_; }

private:
    friend class MemoryTestSetting;
    MemoryTestSetting memtest_setting;
    BromMemtestArg memtest_arg_;

    void UpdateUI(const QString& text, QColor color = Qt::black);

    void UpdateMemoryTestDetection(const DA_REPORT_T &da_report);

    void EnableDRAM(DA_REPORT_T &da_report,
                    FLASHTOOL_API_HANDLE_T ft_handle,
                    DL_HANDLE_T dl_handle);

    void RAMTest(FlashTool_MemoryTest_Arg *mt_arg,
                 FlashTool_MemoryTest_Result *mt_result,
                 const DA_REPORT_T &da_report,
                 FLASHTOOL_API_HANDLE_T ft_handle);

    int RAMDataBusTest(FlashTool_MemoryTest_Arg *mt_arg,
                        FlashTool_MemoryTest_Result *mt_result,
                        const DA_REPORT_T &da_report,
                        FLASHTOOL_API_HANDLE_T ft_handle);

    int RAMAddrBusTest(FlashTool_MemoryTest_Arg *mt_arg,
                        FlashTool_MemoryTest_Result *mt_result,
                        const DA_REPORT_T &da_report,
                        FLASHTOOL_API_HANDLE_T ft_handle);

    int RAMPatternTest(FlashTool_MemoryTest_Arg *mt_arg,
                        FlashTool_MemoryTest_Result *mt_result,
                        unsigned int pattern_set[],
                        int pattern_size,
                        const DA_REPORT_T &da_report,
                        FLASHTOOL_API_HANDLE_T ft_handle);

    int RAMOnePatternTest(FlashTool_MemoryTest_Arg *mt_arg,
                           FlashTool_MemoryTest_Result *mt_result,
                           unsigned int pattern,
                           const DA_REPORT_T &da_report,
                           FLASHTOOL_API_HANDLE_T ft_handle);

    int RAMIncDecTest(FlashTool_MemoryTest_Arg *mt_arg,
                       FlashTool_MemoryTest_Result *mt_result,
                       const DA_REPORT_T &da_report,
                       FLASHTOOL_API_HANDLE_T ft_handle);

    int RAMIncDecTestIO(FlashTool_MemoryTest_Arg *mt_arg,
                         FlashTool_MemoryTest_Result *mt_result,
                         HW_MemoryIO_E io,
                         const DA_REPORT_T &da_report,
                         FLASHTOOL_API_HANDLE_T ft_handle);

    int DRAMFlipTest(FlashTool_MemoryTest_Arg *mt_arg,
                      FlashTool_MemoryTest_Result *mt_result,
                      const DA_REPORT_T &da_report,
                      FLASHTOOL_API_HANDLE_T ft_handle);

    void NANDTest(FlashTool_MemoryTest_Arg *mt_arg,
                  FlashTool_MemoryTest_Result *mt_result,
                  const DA_REPORT_T &da_report,
                  FLASHTOOL_API_HANDLE_T ft_handle);

    int NANDPatternTest(FlashTool_MemoryTest_Arg *mt_arg,
                         FlashTool_MemoryTest_Result *mt_result,
                         const DA_REPORT_T &da_report,
                         FLASHTOOL_API_HANDLE_T ft_handle);

    void EMMCTest(FlashTool_MemoryTest_Arg *mt_arg,
                  FlashTool_MemoryTest_Result *mt_result,
                  const DA_REPORT_T &da_report,
                  FLASHTOOL_API_HANDLE_T ft_handle);

    int EMMCPatternTest(FlashTool_MemoryTest_Arg *mt_arg,
                         FlashTool_MemoryTest_Result *mt_result,
                         const DA_REPORT_T &da_report,
                         FLASHTOOL_API_HANDLE_T ft_handle);

    void UFSTest(FlashTool_MemoryTest_Arg *mt_arg,
                  FlashTool_MemoryTest_Result *mt_result,
                  const DA_REPORT_T &da_report,
                  FLASHTOOL_API_HANDLE_T ft_handle);

    int UFSPatternTest(FlashTool_MemoryTest_Arg *mt_arg,
                         FlashTool_MemoryTest_Result *mt_result,
                         const DA_REPORT_T &da_report,
                         FLASHTOOL_API_HANDLE_T ft_handle);
};

}

#endif // MEMORYTESTCOMMAND_H
