#ifndef _EFUSE_COMMAND_H_
#define _EFUSE_COMMAND_H_

#include "ICommand.h"
#include "../BootRom/DOWNLOAD.H"
#include "../Setting/EfuseSetting.h"

namespace APCore
{
class EfuseCommand : public ICommand
{
public:
    EfuseCommand(APKey, EfuseSetting*);
    virtual ~EfuseCommand();

    virtual void exec(
            const _SHARED_PTR<Connection> &conn);

    void setreadonly(bool readonly){
        read_only_ = readonly;
    }

    void setStbLockEnable(bool enable){
        stb_lock_enable_ = enable;
    }

    void setStbKeyEnable(bool enable){
        stb_key_enable_ = enable;
    }

private:
    void Connect(Connection*);
    void TryConnectDA(Connection*);
    void WriteAll(Connection*);
    void ReadAll(Connection*);

    void DumpSetting();

    void DumpResult(
            const std::string &rb_file,
            const Efuse_Common_Arg *common,
            const Efuse_Secure_Arg *secure,
            const Efuse_Lock_Arg *lock,
            const Efuse_STB_Key_Arg *stb,
            const Efuse_Extra_Arg   *extra);

    void DumpCommonArg(
            const Efuse_Common_Arg *common,
            bool to_write = true);
    void DumpSecureArg(
            const Efuse_Secure_Arg *secure,
            bool to_write = true);
    void DumpLockArg(
            const Efuse_Lock_Arg *lock);
    void DumpSTBLockArg(
            const STB_Lock_PARAM* stb_lock);
    void DumpSTBArg(
            const Efuse_STB_Key_Arg *stb,
            bool to_write = true);
    void DumpExtraArg(
            const Efuse_Extra_Arg *extra,
            bool to_write = true);

    void SaveCommonArg(
            FILE *rb_file,
            const Efuse_Common_Arg *common);
    void SaveSecureArgR(
            FILE *rb_file,
            const Efuse_Secure_Arg *secure);
    void SaveLockArg(
            FILE *rb_file,
            const Efuse_Lock_Arg *lock);
    void SaveStbArg(
            FILE *rb_file,
            const Efuse_STB_Key_Arg *stb);

    void SaveSTBKeyLockArg(
            FILE *rb_file,
            const STB_Lock_PARAM* stb_lock);

    void SaveExtraArg(
            FILE *rb_file,
            const Efuse_Extra_Arg *extra);

    std::string Binary2Str(const _BUFFER&);

private:
    EfuseSetting *p_setting_;
    bool read_only_;
    bool stb_lock_enable_;
    bool stb_key_enable_;
};

}

#endif // _EFUSE_COMMAND_H_
