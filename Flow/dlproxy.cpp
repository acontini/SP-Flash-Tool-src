#include "dlproxyimpl.h"
#include "dlproxy.h"
#include "../Host/Inc/RuntimeMemory.h"

CDLProxy::CDLProxy(
        const QSharedPointer<APCore::Connection> &conn,
        APCore::RBHandle *rb_handle,
        const CStorHelper &stor)
    :my_impl_(new CDLProxyImpl(conn, rb_handle, stor))
{
}

CDLProxy::~CDLProxy()
{
    delete my_impl_;
}

ERROR_T CDLProxy::read(U64 /*address*/,
                       U64 /*length*/)
{
    // merge & save read address range

    // it seems we can ignore the "read"
    // we can deduce what to read from the "restore" info

    return ERROR_OK;
}

ERROR_T CDLProxy::format(U64 address,
                         U64 length,
                         U32 part)
{
    LOGD("format: %08llx-%08llx-%d",
         address, address+length, part);

    if (length == 0)
    {
        return ERROR_OK;
    }

    std::list<ADDRESS_RANGE>::iterator it =
            my_impl_->format_range_set_.begin();

    while (it != my_impl_->format_range_set_.end())
    {
        if(it->part == part)
        {
            if(address > _EndAddr(*it))
            {
                break;  // sorted descendingly
            }
            else if (_OverLap(*it, address, length))
            {
                U64 end_addr = address + length;

                end_addr = max(end_addr, _EndAddr(*it));
                address = min(address, it->addr);
                length = end_addr - address;

                my_impl_->format_range_set_.erase(it);

                LOGD("format: merging...");

                // merge ranges & try again
                return format(address, length, part);
            }
        }

        ++ it;
    }

    ADDRESS_RANGE range =
    {
        address, length, part
    };

    my_impl_->format_range_set_.insert(it, range);

    return ERROR_OK;
}

void CDLProxy::Add_Rom_2format(const PART_INFO& rom)
{
    ADDRESS_RANGE range =
    {
        rom.begin_addr, rom.image_length, rom.part_id
    };

    my_impl_->format_range_set_.push_back(range);
}

ERROR_T CDLProxy::download(const ROM_INFO &rom)
{
    LOGD("download: %s", rom.name);

    ++ my_impl_->rom_to_dl_;
    return ERROR_OK;
}

ERROR_T CDLProxy::restore(CRestoreWorker *resto)
{
    // for checksum concerns,
    // restore ranges are no longer merged

    LOGD("restore: %s, %08llx-%08llx --> %08llx, %d, %d",
         resto->part_name().c_str(), resto->addr(), resto->end(), resto->dest(), resto->flag(), resto->part());

    RESTORE_RANGE range(resto);
    my_impl_->restore_range_set_.push_back(range);

    return ERROR_OK;
}

ERROR_T CDLProxy::verify(U64 /*address*/,
                         U64 /*length*/)
{
    my_impl_->dl_verify_ = true;
    return ERROR_OK;
}

ERROR_T CDLProxy::commit(
    APCore::ReadbackSetting *       read_setting,
    APCore::FormatSetting *         format_setting,
    APCore::DADownloadAllSetting *  dl_setting,
    APCore::WriteMemorySetting *    write_setting)
{
    ERROR_T err;

    //add this flag to seperate backupFoler is create in this fw or previous fw
    my_impl_->set_backup_folder_exists_before_fw_grade(my_impl_->BackupFolderExistNoCreate());

    if (read_setting != NULL)
    {
        if ((err=my_impl_->DoRead(read_setting)) != ERROR_OK)
            return err;
    }

    if (format_setting != NULL)
    {
        if ((err=my_impl_->DoFormat(format_setting)) != ERROR_OK)
            return err;
    }

    if (dl_setting != NULL)
    {
        if ((err=my_impl_->DoDownload(dl_setting)) != ERROR_OK)
            return err;
    }

    if (write_setting != NULL)
    {
        if ((err=my_impl_->DoRestore(write_setting)) != ERROR_OK)
            return err;
    }

    return ERROR_OK;
}

const PART_INFO *CDLProxy::read_pmt(const char *name)
{
    return my_impl_->read_pmt(name);
}

ERROR_T CDLProxy::validate_storage(U64 align, U64 total) const
{
    if (my_impl_->ValidateStorage(align, total))
    {
        return ERROR_OK;
    }
    else
    {
        return ERROR_ILLEGAL_ADDRESS;
    }
}
