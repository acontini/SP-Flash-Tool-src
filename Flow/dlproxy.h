#ifndef DLPROXY_H
#define DLPROXY_H

#include "fw_defs.h"
#include <QSharedPointer>

namespace APCore
{
    class RBHandle;
    class Connection;
    class FormatSetting;
    class ReadbackSetting;
    class WriteMemorySetting;
    class DADownloadAllSetting;
}

class CStorHelper;
class CDLProxyImpl;
class CRestoreWorker;
class CSpecailRestoreWorker;

class CDLProxy
{
public:
    CDLProxy(
            const QSharedPointer<APCore::Connection> &conn,
            APCore::RBHandle *rb_handle,
            const CStorHelper &stor);

    ~CDLProxy();

public:
    ERROR_T read(U64 address,
                 U64 length);

    ERROR_T format(U64 address,
                   U64 length,
                   U32 part);

    //for no reserved ROM that target have but scatter not have, it should be format
    void Add_Rom_2format(const PART_INFO&);

    ERROR_T download(const ROM_INFO&);

    ERROR_T restore(CRestoreWorker*);

    ERROR_T verify(U64 address,
                   U64 length);

    ERROR_T commit(APCore::ReadbackSetting *read_setting,
                   APCore::FormatSetting *format_setting,
                   APCore::DADownloadAllSetting *dl_setting,
                   APCore::WriteMemorySetting *write_setting);

    const PART_INFO *read_pmt(const char*);

    ERROR_T validate_storage(U64 align, U64 total) const;

private:
    CDLProxyImpl *my_impl_;
};

#endif // DLPROXY_H
