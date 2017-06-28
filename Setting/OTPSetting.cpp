#include "OTPSetting.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{
OTPSetting::OTPSetting():
    operation(OTP_WRITE),
    addr(0),
    len(0),
    storage_(HW_STORAGE_NAND)
{}

OTPSetting::~OTPSetting()
{
}

QSharedPointer<APCore::ICommand> OTPSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::OTPCommand> cmd(new APCore::OTPCommand(key));
    cmd->otp_arg_.set_otp_enable(_TRUE);
    cmd->otp_arg_.set_otp_op(operation);
    cmd->otp_arg_.set_otp_addr(addr);
    cmd->otp_arg_.set_otp_len(len);
    cmd->otp_arg_.set_otp_data(data.data());
    cmd->set_otp_file(this->file);

    if(NULL != cb_param_process_init)
        cmd->otp_arg_.set_para_process_init(cb_param_process_init);

    if(NULL != cb_param_process)
        cmd->otp_arg_.set_para_process(cb_param_process);

    if(NULL != cb_post_process_init)
        cmd->otp_arg_.set_post_process_init(cb_post_process_init);

    if(NULL != cb_post_process)
        cmd->otp_arg_.set_post_process(cb_post_process);

    return cmd;
}
void OTPSetting::LoadXML(const XML::Node &node)
{
    LOG("The node name is %s.", node.GetName().c_str());
}
 void OTPSetting::SaveXML(XML::Node &node) const
{
    LOG("The node name is %s.", node.GetName().c_str());
}

}

