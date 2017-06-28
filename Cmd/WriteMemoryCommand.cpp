#include "WriteMemoryCommand.h"

#include "../Err/Exception.h"
#include "../Logger/Log.h"
#include "../Utility/FileUtils.h"
#include "../UI/src/ICallback.h"

namespace APCore
{
WriteMemoryCommand::WriteMemoryCommand(APKey key):
   ICommand(key), wm_arg_(),
   cb_write_memory_init(NULL)
{}

WriteMemoryCommand::~WriteMemoryCommand()
{}

int __stdcall WriteMemoryCommand::cb_write_memory_init_console()
{
    LOGI("Write Memory Initial...");
    return 0;
}

void WriteMemoryCommand::exec(
        const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    LOG("executing Write Memory Command...");
    int ret = 0;

    if(cb_write_memory_init != NULL)
    {
          cb_write_memory_init();
    }
    else
    {
        this->cb_write_memory_init_console();
    }

    ret = FlashTool_WriteFlashMemory(conn->FTHandle(), wm_arg_.raw_arg());
    if( S_DONE != ret )
    {
         LOG("ERROR: FlashTool_WriteFlashMemory() failed!");
         THROW_BROM_EXCEPTION(ret, 0);
    }
}

}


