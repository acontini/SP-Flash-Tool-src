#ifndef WRITEMEMORYCOMMAND_H
#define WRITEMEMORYCOMMAND_H

#include "ICommand.h"
#include "../Conn/Connection.h"
#include "../Arg/WriteMemoryArg.h"

namespace APCore
{
class WriteMemoryCommand:public ICommand
{
public:
     typedef void (__stdcall * void_callback)();

    WriteMemoryCommand(APKey key);
    ~WriteMemoryCommand();

    virtual void exec(const QSharedPointer<Connection> &conn);
    void set_write_memory_init(void_callback cb)
    {
          this->cb_write_memory_init = cb;
    }

    static int __stdcall cb_write_memory_init_console();
private:
    friend class WriteMemorySetting;
    WriteMemoryArg wm_arg_;
    void_callback cb_write_memory_init;
};
}

#endif // WRITEMEMORYCOMMAND_H
