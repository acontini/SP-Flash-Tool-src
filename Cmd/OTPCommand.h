#ifndef OTPCOMMAND_H
#define OTPCOMMAND_H

#include "ICommand.h"
#include "../Conn/Connection.h"
#include "../Arg/BromOTPArg.h"

namespace APCore
{
class OTPCommand: public ICommand
{
public:
    OTPCommand(APKey key);
    virtual ~OTPCommand();

    void set_otp_file(const std::string &path)
    {
         this->path_ = path;
    }

    virtual void exec(const QSharedPointer<APCore::Connection> &conn);

private:
    friend class OTPSetting;
    BromOTPArg otp_arg_;
    std::string path_;
};
}

#endif // OTPCOMMAND_H
