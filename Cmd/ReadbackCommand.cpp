#include "ReadbackCommand.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Utility/FileUtils.h"

namespace APCore
{

ReadbackCommand::ReadbackCommand(APKey key)
    :ICommand(key), readback_arg_()
{
}

ReadbackCommand::~ReadbackCommand()
{

}

void ReadbackCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    LOGI("executing Readback command...");

    FlashTool_Readback_Result rb_result;
    memset(&rb_result, 0, sizeof(FlashTool_Readback_Result));

    int ret = FlashTool_Readback(
                conn->FTHandle(),
                readback_arg_.raw_arg(),
                &rb_result);
    LOGD("Readback result(%d)",ret);
    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    LOGI("Readback Succeeded.");
}

}
