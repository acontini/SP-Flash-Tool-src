#ifndef DLPROXYIMPL_H
#define DLPROXYIMPL_H

#include <list>
#include <string>
#include "fw_defs.h"
#include "restoreworker.h"
#include "../Resource/Handle/RBHandle.h"
#include "../Setting/FormatSetting.h"
#include "../Setting/ReadbackSetting.h"
#include "../Setting/WriteMemorySetting.h"
#include "../Setting/DADownloadAllSetting.h"


#ifndef max
#define max(a,b) ((a)>=(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<=(b)?(a):(b))
#endif

#define _InRange(x, a, l)   \
    ((x)>=(a) && (x)<=(a)+(l))

#define _OverLap(r, a, l)   \
    (_InRange((r).addr, (a), (l)) ||    \
     _InRange((a), (r).addr, (r).leng) )

#define _EndAddr(r) ((r).addr+(r).leng)


typedef struct __ADDRESS_RANGE
{
    U64 addr;
    U64 leng;
    U32 part;
} ADDRESS_RANGE;
/*
typedef struct __RESTORE_RANGE
{
    U64 addr;
    U64 leng;
    U64 dest;
} RESTORE_RANGE;
*/

typedef QSharedPointer<CRestoreWorker> RESTORE_RANGE;

class CDLProxyImpl
{
public:
    friend class CDLProxy;

    CDLProxyImpl(
            const QSharedPointer<APCore::Connection> &conn,
            APCore::RBHandle *rb_handle,
            const CStorHelper &stor);

    ~CDLProxyImpl();

public:
    const PART_INFO *read_pmt(const char *name);

    ERROR_T DoRead(APCore::ReadbackSetting*);
    ERROR_T DoRestore(APCore::WriteMemorySetting*);
    ERROR_T DoFormat(APCore::FormatSetting*);
    ERROR_T DoDownload(APCore::DADownloadAllSetting*);
    //ERROR_T DoVerify(APCore::ReadbackSetting*); // ???

    bool ValidateStorage(U64 align, U64 total) const;
    void set_backup_folder_exists_before_fw_grade(bool exists)
    {
        backup_folder_exists_before_fw_grade = exists;
    }

    bool BackupFolderExist();
    bool BackupFolderExistNoCreate();

private:
    void DoCommand(ISetting *setting)
    {
        setting->CreateCommand(conn_->ap_key())->exec(conn_);
    }

    void ClearTempFolder() const;

    bool ValidateRestore(U64 align, U64 total) const;
    bool ValidateFormat(U64 align, U64 total) const;

    ERROR_T SaveChecksum() const;

    const PART_INFO *FindPartition(const char*) const;
    bool DoReadPmt(void);

    static bool ValidateAddress(U64 addr, U64 align, U64 total);

    ProgramMode GetProgramModeByPartName(std::string part_name);

    ERROR_T DoRestoreWhenPMTNoChangeBakfolderExists(APCore::WriteMemorySetting*);

    ERROR_T DoRestoreWhenPMTChanged(APCore::WriteMemorySetting*);

private:
    //std::list<ADDRESS_RANGE> read_range_set_;
    std::list<RESTORE_RANGE> restore_range_set_;
    std::list<ADDRESS_RANGE> format_range_set_;

    QSharedPointer<APCore::Connection> conn_;

    /*static*/ PART_INFO *part_info_all_;
    /*static*/ unsigned int part_count_;

    APCore::RBHandle *rb_handle_;

    unsigned int rom_to_dl_;
    //unsigned int update_pmt_;

    bool dl_verify_;

    const CStorHelper &stor_;

    bool does_read_;

    //this var to process specail case that PMT updated but fw upgrade fail, try again
    bool backup_folder_exists_before_fw_grade;
};


#endif // DLPROXYIMPL_H
