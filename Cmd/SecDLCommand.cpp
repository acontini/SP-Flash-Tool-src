#include "SecDLCommand.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/RBHandle.h"

#include "../Flow/storhelper.h"
#include "../Flow/dlproxy.h"
#include "../Flow/defaultrom.h"
#include "../Utility/constString.h"
#include "../Public/RomInfoFactory.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

namespace APCore
{
SecDLCommand::SecDLCommand(APKey key):
    ICommand(key),
    dl_setting_(NULL),
    format_setting_(NULL),
    key_(key)
{
}

SecDLCommand::~SecDLCommand()
{

}

const PART_INFO * SecDLCommand::get_user_data_partition(const QSharedPointer<Connection> &conn)
{
    APCore::CoreHandle *core_handle = __POOL_OF_HANDLE(key_);
    if(core_handle == NULL)
    {
        THROW_APP_EXCEPTION(FT_INVALIDE_SESSION_KEY, "");
    }

    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key_);

    CStorHelper storage(conn.data());

    CDLProxy proxy(conn, rb_handle, storage);

    const PART_INFO *part_info = proxy.read_pmt("USRDATA");

    if (part_info != NULL)
    {
        return part_info;
    }

    return NULL;
}

void SecDLCommand::DoDownload(const QSharedPointer<Connection> &conn)
{
    LOGD("downloading sec_ro image...");

    if(NULL != dl_setting_)
        dl_setting_->CreateCommand(conn->ap_key())->exec(conn);
}

void SecDLCommand::DoFormat(const QSharedPointer<Connection> &conn)
{
    LOGD("formating user data image...");

    if(NULL != format_setting_)
    {
        const PART_INFO *pmt = get_user_data_partition(conn);
        if(NULL != pmt)
        {
            format_setting_->set_begin_addr(pmt->begin_addr);
            format_setting_->set_length(pmt->image_length);
            format_setting_->set_part_id(pmt->part_id);

            LOG("formatting range: %08llx-%08llx",pmt->begin_addr,
                 pmt->image_length);

            format_setting_->CreateCommand(conn->ap_key())->exec(conn);
        }
    }
}

void SecDLCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(dl_setting_ != NULL)
        DoDownload(conn);

    if(format_setting_ != NULL)
        DoFormat(conn);
}
}
