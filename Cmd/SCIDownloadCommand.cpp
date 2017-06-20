#include "SCIDownloadCommand.h"

namespace APCore
{

SCIDownloadCommand::SCIDownloadCommand(APKey key):
    ICommand(key),
    fmt_setting_(NULL),
    dl_setting_(NULL)
{
}

SCIDownloadCommand::~SCIDownloadCommand()
{

}

void SCIDownloadCommand::DoDownload(const QSharedPointer<Connection> &conn)
{
    LOG("begin downloading SCI image...");

    if(NULL != dl_setting_)
        dl_setting_->CreateCommand(conn->ap_key())->exec(conn);
}

void SCIDownloadCommand::DoFormat(const QSharedPointer<Connection> &conn)
{
     LOG("begin formating SCI image...");

     if(NULL != fmt_setting_)
         fmt_setting_->CreateCommand(conn->ap_key())->exec(conn);
}

void SCIDownloadCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(NULL != fmt_setting_)
        DoFormat(conn);

    if(NULL != dl_setting_)
        DoDownload(conn);
}


}
