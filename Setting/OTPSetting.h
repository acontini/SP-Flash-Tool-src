#ifndef OTPSETTING_H
#define OTPSETTING_H

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"
#include "../Cmd/OTPCommand.h"
#include "../XMLParser/XMLNode.h"

namespace APCore
{
class OTPSetting: public ISetting
{
public:
    OTPSetting();
    ~OTPSetting();

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    //setters
    void set_otp_operation(const OTP_OPERATION &otp_oper)
    {
        this->operation = otp_oper;
    }

    void set_otp_address(const unsigned int otp_addr)
    {
        this->addr = otp_addr;
    }

    void set_otp_length(const unsigned int otp_len)
    {
        this->len = otp_len;
    }

    void set_otp_file(const std::string &otp_file)
    {
        this->file = otp_file;
    }

    void set_para_process_init(const CALLBACK_PARAM_PROGRESS_INIT cb)
    {
        this->cb_param_process_init = cb;
    }

    void set_para_process(const CALLBACK_PARAM_PROGRESS cb)
    {
        this->cb_param_process = cb;
    }

    void set_post_process_init(const CALLBACK_POST_PROCESS_INIT cb)
    {
        this->cb_post_process_init = cb;
    }

    void set_post_process(const CALLBACK_POST_PROCESS cb)
    {
        this->cb_post_process = cb;
    }

    void set_otp_data(const QSharedPointer<U8> data)
    {
        this->data = data;
    }

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key);

private:

    CALLBACK_PARAM_PROGRESS_INIT cb_param_process_init;
    CALLBACK_PARAM_PROGRESS cb_param_process;
    CALLBACK_POST_PROCESS_INIT cb_post_process_init;
    CALLBACK_POST_PROCESS cb_post_process;

    OTP_OPERATION operation;
    unsigned int addr;
    unsigned int len;
    std::string file;
    QSharedPointer<U8> data;
    HW_StorageType_E storage_;
};
}

#endif // OTPSETTING_H
