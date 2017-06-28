#include "DLPlugFmtAllCommand.h"

namespace APCore
{
DLPlugFmtAllCommand::DLPlugFmtAllCommand(APKey key):
    ICommand(key),
    dl_setting_(NULL),
    format_setting_(NULL)
{
}

DLPlugFmtAllCommand::~DLPlugFmtAllCommand()
{

}

void DLPlugFmtAllCommand::DoDownload(const QSharedPointer<Connection> &conn)
{
    if(NULL != dl_setting_)
        dl_setting_->CreateCommand(conn->ap_key())->exec(conn);
}

void DLPlugFmtAllCommand::DoFormat(const QSharedPointer<Connection> &conn)
{
    LOG("formating all image...");

    if(NULL != format_setting_)
        format_setting_->CreateCommand(conn->ap_key())->exec(conn);
}

void DLPlugFmtAllCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(format_setting_ != NULL)
        DoFormat(conn);

    if(dl_setting_ != NULL)
        DoDownload(conn);
}
}
