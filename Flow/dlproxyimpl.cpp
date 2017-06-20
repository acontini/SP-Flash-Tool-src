
#include "dlproxyimpl.h"
#include "../Utility/FileUtils.h"
#include "../BootRom/host.h"
#include "../Host/Inc/RuntimeMemory.h"

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

#ifdef _WIN32
#define _DeleteFile DeleteFileA
#else
#define _DeleteFile unlink
#endif


CDLProxyImpl::CDLProxyImpl(
        const QSharedPointer<APCore::Connection> &conn,
        APCore::RBHandle *rb_handle,
        const CStorHelper &stor)
    : conn_(conn),
      part_info_all_(NULL),
      part_count_(0),
      rb_handle_(rb_handle),
      rom_to_dl_(0),
      dl_verify_(false),
      stor_(stor),
      does_read_(true),
      backup_folder_exists_before_fw_grade(false)
{
}

CDLProxyImpl::~CDLProxyImpl()
{
    if (part_info_all_ != NULL)
    {
        delete [] part_info_all_;
    }
}

bool CDLProxyImpl::BackupFolderExist()
{
    const std::string backup_dir=
            CRestoreWorker::BasePath(
                conn_->da_report().m_random_id,
                sizeof(conn_->da_report().m_random_id));
    return FileUtils::IsDirectoryExist(backup_dir);
}

bool CDLProxyImpl::BackupFolderExistNoCreate()
{
    const std::string backup_dir=
            CRestoreWorker::BasePathNoCreateIfNotExist(
                conn_->da_report().m_random_id,
                sizeof(conn_->da_report().m_random_id));
    return FileUtils::IsDirectoryExist(backup_dir);
}

void CDLProxyImpl::ClearTempFolder() const
{
    const std::string backup_dir=
            CRestoreWorker::BasePath(
                conn_->da_report().m_random_id,
                sizeof(conn_->da_report().m_random_id));

    LOGD("Delete backup folder:%s", backup_dir.c_str());

    FileUtils::QDeleteDirectory(backup_dir);
}

ERROR_T CDLProxyImpl::DoRead(APCore::ReadbackSetting *setting)
{
    unsigned int i = 0;

    assert(rb_handle_ != NULL);

    rb_handle_->ClearAll();

    for(std::list<RESTORE_RANGE>::const_iterator it = restore_range_set_.begin();it != restore_range_set_.end();++it)
    {
        (*it)->SetChipId(
                    conn_->da_report().m_random_id,
                    sizeof(conn_->da_report().m_random_id));

        if((*it)->inpos())
            continue;

        if ((*it)->Verify(&stor_))
        {
            LOGD("Backup files already exist!");
        }
        else
        {
            LOGD("reading: %08llx-%08llx, %08llx, %s",
                 (*it)->addr(), (*it)->end(),
                 (*it)->leng(), (*it)->Path().c_str());

            ReadbackItem item(
                        i++,
                        true,
                        (*it)->addr(),
                        (*it)->leng(),
                        (*it)->Path(),
                        (*it)->flag(),
                        (*it)->part(),
                        NUTL_ADDR_LOGICAL);

            rb_handle_->AppendItem(item);
        }
    }

    if (rb_handle_->GetCount() > 0)
    {
        DoCommand(setting);
        rb_handle_->ClearAll();

        ERROR_T ret = SaveChecksum();

        if(ret != ERROR_OK)
        {
            //ClearTempFolder();
            return ret;
        }
    }
    else
    {
        LOGD("nothing to read");
    }

    return ERROR_OK;
}

ProgramMode CDLProxyImpl::GetProgramModeByPartName(std::string part_name)
{
    for(std::list<RESTORE_RANGE>::const_iterator it = restore_range_set_.begin();it != restore_range_set_.end();++it)
    {
        if((*it)->inpos())
            continue;

        if((*it)->part_name() == part_name)
        {
            return (*it)->flag() == NUTL_READ_PAGE_SPARE_WITH_ECCDECODE?
                        ProgramMode_PageSpare:
                        ProgramMode_PageOnly;
        }
    }
}

ERROR_T CDLProxyImpl::DoRestoreWhenPMTNoChangeBakfolderExists(APCore::WriteMemorySetting* setting)
{
    APCore::WriteMemorySetting wm_setting(*setting);
    wm_setting.set_container_length(0); // dummy
    wm_setting.set_input_mode(InputMode_FromFile);
    wm_setting.set_addressing_mode(AddressingMode_LogicalAddress);

    LOGD("Process Special case: first time fw upgrade fail, but PMT update and backup folder exists!!!");

    const std::string backup_dir=
            CRestoreWorker::BasePath(
                conn_->da_report().m_random_id,
                sizeof(conn_->da_report().m_random_id));
    QDir* pBackupFolder = new QDir(QString::fromLocal8Bit(backup_dir.c_str()));
    QStringList chkFilter = QStringList(QString("*.chk"));
    QStringList rawFilter = QStringList(QString("*.raw"));
    pBackupFolder->setNameFilters(chkFilter);
    QList<QFileInfo> *chkFileInfo = new QList<QFileInfo>(pBackupFolder->entryInfoList(chkFilter));
    pBackupFolder->setNameFilters(rawFilter);
    QList<QFileInfo> *rawFileInfo = new QList<QFileInfo>(pBackupFolder->entryInfoList(rawFilter));
    int chkFileCount = chkFileInfo->size();
    int rawFileCount = rawFileInfo->size();
    if(chkFileCount != rawFileCount)
        return ERROR_CHK_RAW_FILE_COUNT_DIFF;

    QList<QFileInfo>::const_iterator chkfileInfoIt = chkFileInfo->begin();
    QList<QFileInfo>::const_iterator rawfileInfoIt  = rawFileInfo->begin();
    while(rawfileInfoIt != rawFileInfo->end())
    {
        bool haveChkFile = false;
        while(chkfileInfoIt != chkFileInfo->end())
        {
            if(chkfileInfoIt->baseName() == rawfileInfoIt->baseName())
            {
                haveChkFile = true;
                break;
            }
            ++chkfileInfoIt;
        }
        if(!haveChkFile)
            return ERROR_CHK_FILE_NOT_EXIST;

        ++rawfileInfoIt;
    }

    rawfileInfoIt  = rawFileInfo->begin();
    while(rawfileInfoIt != rawFileInfo->end())
    {
        std::string part_name = rawfileInfoIt->baseName().toStdString();
        const PART_INFO *pmt = read_pmt(part_name.c_str());

        wm_setting.set_address(pmt->begin_addr);
        wm_setting.set_input_length(pmt->image_length);
        wm_setting.set_input(rawfileInfoIt->filePath().toStdString());
        wm_setting.set_program_mode(GetProgramModeByPartName(part_name));
        wm_setting.set_part_id(pmt->part_id);

        LOGD("specail restoring: %s, %08llx-%08llx ",
             part_name.c_str(), pmt->begin_addr, pmt->begin_addr+pmt->image_length);

        DoCommand(&wm_setting);
        ++ rawfileInfoIt;
    }

    ClearTempFolder();

    return ERROR_OK;
}

ERROR_T CDLProxyImpl::DoRestoreWhenPMTChanged(APCore::WriteMemorySetting *setting)
{
    APCore::WriteMemorySetting wm_setting(*setting);
    wm_setting.set_container_length(0); // dummy
    wm_setting.set_input_mode(InputMode_FromFile);
    wm_setting.set_addressing_mode(AddressingMode_LogicalAddress);

    for(std::list<RESTORE_RANGE>::const_iterator it = restore_range_set_.begin();it != restore_range_set_.end();++it)
    {
        if((*it)->inpos())
            continue;

        if (!(*it)->Verify(&stor_))
        {
            LOGW("check sum failed before restore: %s, %08llx-%08llx --> %08llx",
                 (*it)->part_name().c_str(), (*it)->addr(), (*it)->end());

            (*it)->Clean();

            return ERROR_CHKSUM_FAIL; //???
        }

        LOGD("restoring: %s, %08llx-%08llx --> %08llx",
             (*it)->part_name().c_str(), (*it)->addr(), (*it)->end(), (*it)->dest());

        wm_setting.set_address((*it)->dest());
        wm_setting.set_input_length((*it)->leng());
        wm_setting.set_input((*it)->Path());

        if((*it)->flag() == NUTL_READ_PAGE_SPARE_WITH_ECCDECODE)
            wm_setting.set_program_mode(ProgramMode_PageSpare);
        else
            wm_setting.set_program_mode(ProgramMode_PageOnly);

        wm_setting.set_part_id((*it)->part());

        DoCommand(&wm_setting);
    }

    ClearTempFolder();

    return ERROR_OK;
}

ERROR_T CDLProxyImpl::DoRestore(APCore::WriteMemorySetting *setting)
{
    LOGD("backup_folder_exists_before_fw_grade: %d", backup_folder_exists_before_fw_grade);
    if(backup_folder_exists_before_fw_grade)
        return DoRestoreWhenPMTNoChangeBakfolderExists(setting);
    else
        return DoRestoreWhenPMTChanged(setting);
}

ERROR_T CDLProxyImpl::DoFormat(APCore::FormatSetting *setting)
{
    std::list<ADDRESS_RANGE>::const_iterator it =
            format_range_set_.begin();

    setting->set_physical_fmt(false);

    while (it != format_range_set_.end())
    {
        LOGD("formatting: %08llx-%08llx",
             it->addr, _EndAddr(*it));

        U32 part_id = it->part;
        setting->set_begin_addr(it->addr);
        setting->set_length(it->leng);
        setting->set_part_id(part_id);
        setting->update_format_type_by_region_id(part_id);

        DoCommand(setting);

        ++ it;
    }

    return ERROR_OK;
}

ERROR_T CDLProxyImpl::DoDownload(APCore::DADownloadAllSetting *setting)
{
    if (rom_to_dl_ > 0)
    {
        //dl_handle->EnableDAChksum(dl_verify_);

        // nothing extra to set

        LOGD("downloading...");

        DoCommand(setting);
    }

    return ERROR_OK;
}

ERROR_T CDLProxyImpl::SaveChecksum() const
{
    for(std::list<RESTORE_RANGE>::const_iterator it = restore_range_set_.begin();it != restore_range_set_.end();++it)
    {
        if((*it)->inpos())
            continue;

        if((*it)->Stamp(&stor_))
        {
            LOGD("checksum stamp ok: %08llx-%08llx",
                 (*it)->addr(), (*it)->end());
        }
        else
        {
            LOGD("checksum stamp failed: %08llx-%08llx",
                 (*it)->addr(), (*it)->end());

            (*it)->Clean();

            return ERROR_CHKSUM_FAIL;
        }
    }

    return ERROR_OK;
}

// work hard to find a best match
const PART_INFO *CDLProxyImpl::FindPartition(
        const char *name) const
{
    assert(part_info_all_ != NULL);
    assert(part_count_ > 0);

    const char *name0 = NULL;
    const PART_INFO *p = NULL;

    // to find a perfect match or the 1st similar one
    for (unsigned int i=0; i<part_count_; ++i)
    {
        name0 = part_info_all_[i].name;

        if (strcmp(name, name0) == 0)
        {
            p = part_info_all_+i;
            break;
        }
        if (p != NULL)
        {
            continue;
        }
        if (strstr(name0, name) != 0 ||
            strstr(name, name0) != 0)
        {
            p = part_info_all_+i;
        }
    }

    if (p != NULL)
    {
        LOGD("PMT for %s: %s [0x%08llx-0x%08llx]",
             name, p->name, p->begin_addr,
             p->begin_addr+p->image_length);
    }
    else
    {
        LOGD("PMT for %s not found~", name);
    }

    return p;
}

bool CDLProxyImpl::DoReadPmt(void)
{
    assert(part_info_all_ == NULL);

    FLASHTOOL_API_HANDLE_T ft_handle =
            conn_->FTHandle();

    int ret = FlashTool_ReadPartitionCount(
                ft_handle, &part_count_);

    // HOPE: for blank phones,
    // ret == S_DONE && part_count_ == 0

    if( S_DONE != ret )
    {
        LOGD("PMT is not exist, read only once!");
        does_read_ = false;

        return false;
    }

    part_info_all_ = new PART_INFO[part_count_+1];

    if (part_count_ > 0)
    {
        ret = FlashTool_ReadPartitionInfo(
                    ft_handle,
                    part_info_all_,
                    part_count_);

        if( S_DONE != ret )
        {
            delete [] part_info_all_;
            part_info_all_ = NULL;
            return false;
        }
    }

    part_info_all_[part_count_].image_length = 0;
    part_info_all_[part_count_].begin_addr = 0;
    part_info_all_[part_count_].name[0] = 0;

    return true;
}

// return NULL: error
// size=0: no such partition
const PART_INFO *CDLProxyImpl::read_pmt(const char *name)
{
    assert(NULL != conn_);

    if(!does_read_)
    {
       return NULL;
    }

    if (part_info_all_ == NULL)
    {
        if (!DoReadPmt())
        {
            return NULL;
        }
    }

    return FindPartition(name);
}

bool CDLProxyImpl::ValidateStorage(U64 align, U64 total) const
{
    return ValidateRestore(align, total) &&
            ValidateFormat(align, total);
}

bool CDLProxyImpl::ValidateRestore(U64 align, U64 total) const
{
    for(std::list<RESTORE_RANGE>::const_iterator it = restore_range_set_.begin();it != restore_range_set_.end();++it)
    {
        if((*it)->inpos())
            continue;

        if (!ValidateAddress((*it)->addr(), align, total) ||
            !ValidateAddress((*it)->end(),  align, total) ||
            !ValidateAddress((*it)->dest(), align, total) )
        {
            LOGD("illegal restore address: %08llx-%08llx --> %08llx",
                 (*it)->addr(), (*it)->end(), (*it)->dest());
            return false;
        }
    }
    return true;
}

bool CDLProxyImpl::ValidateFormat(U64 align, U64 total) const
{
    std::list<ADDRESS_RANGE>::const_iterator it =
            format_range_set_.begin();

    while (it != format_range_set_.end())
    {
        if (!ValidateAddress(it->addr, align, total) ||
            !ValidateAddress(_EndAddr(*it), align, total) )
        {
            LOGD("illegal format address: %08llx-%08llx",
                 it->addr, _EndAddr(*it));
            return false;
        }
        ++ it;
    }
    return true;
}

bool CDLProxyImpl::ValidateAddress(
    U64 addr, U64 align, U64 total)
{
    return ((addr <= total) &&
            ((addr % align) == 0));
}
