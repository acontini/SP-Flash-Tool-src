#include"OTPCommand.h"

#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Utility/FileUtils.h"
#include "../Err/FlashToolErrorCodeDef.h"

namespace APCore
{

OTPCommand::OTPCommand(APKey key)
    :ICommand(key)
{
}

OTPCommand::~OTPCommand()
{

}

void OTPCommand::exec(const QSharedPointer<APCore::Connection> &conn)
{
    conn->ConnectDA();

    LOG("executing OTP command...");
    int ret = 0;

    OTP_T otp_t = otp_arg_.GetBromParaArg().m_param.m_otp;

    otp_arg_.set_dl_handle(GET_DL_HANDLE_T(key_));

    switch(otp_arg_.GetBromParaArg().m_param.m_otp_op)
    {
    case OTP_READ:
        ret = FlashTool_OTP_Read(conn->FTHandle(), &otp_t);
        LOG("OTP Read result(%d)",ret);
        break;

    case OTP_WRITE:
        ret = FlashTool_OTP_Write(conn->FTHandle(), &otp_t);
        LOGD("data is: %s", otp_arg_.GetBromParaArg().m_param.m_otp.m_data);
        LOG("OTP Write result(%d)",ret);
        break;

    case OTP_WRITE_AND_LOCK:
        ret = FlashTool_OTP_Write(conn->FTHandle(), &otp_t);
        ret = FlashTool_OTP_Lock(conn->FTHandle());
        LOG("OTP Write and Lock result(%d)",ret);
        break;

    case OTP_LOCK:
        ret = FlashTool_OTP_Lock(conn->FTHandle());
        LOG("OTP Lock result(%d)",ret);
        break;

    default:
        break;
    }

    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret,0);
    }

    if(otp_arg_.GetBromParaArg().m_param.m_otp_op == OTP_READ)
    {
        if(FileUtils::SaveFile(this->path_.c_str(),
             otp_arg_.GetBromParaArg().m_param.m_otp.m_data, otp_arg_.GetBromParaArg().m_param.m_otp.m_len))
        {
             LOGD("Save OTP data to file failed.");
             THROW_APP_EXCEPTION(FT_SAVE_FILE_ERROR, "");
        }
    }

}

}
