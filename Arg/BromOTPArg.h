#ifndef BROMOTPARG_H
#define BROMOTPARG_H

#include "../BootRom/flashtool.h"
#include <string>
#ifndef _WIN32
#include <string.h>
#endif

class BromOTPArg
{
public:
    BromOTPArg();
    explicit BromOTPArg(const FLASHTOOL_PARAMETER_ARG &ft_para_arg_);

    FLASHTOOL_PARAMETER_ARG GetBromParaArg()const
    {
        return this->ft_para_arg_;
    }

    //setters
    void set_para_process_init(const CALLBACK_PARAM_PROGRESS_INIT cb)
    {
         this->ft_para_arg_.m_cb_param_process_init = cb;
    }

    void set_para_process(const CALLBACK_PARAM_PROGRESS cb)
    {
         this->ft_para_arg_.m_cb_param_process = cb;
    }

    void set_post_process_init(const CALLBACK_POST_PROCESS_INIT cb)
    {
         this->ft_para_arg_.m_cb_post_process_init = cb;
    }

    void set_post_process(const CALLBACK_POST_PROCESS cb)
    {
         this->ft_para_arg_.m_cb_post_process = cb;
    }

    void set_otp_op(const OTP_OPERATION &otp_op)
    {
         this->ft_para_arg_.m_param.m_otp_op = otp_op;
    }

    void set_otp_enable(_BOOL enable)
    {
         this->ft_para_arg_.m_param.m_otp_enable = enable;
    }

    void set_otp_addr(const unsigned int addr)
    {
         this->ft_para_arg_.m_param.m_otp.m_addr = addr;
    }

    void set_otp_len(const unsigned int len)
    {
        this->ft_para_arg_.m_param.m_otp.m_len = len;
    }

    void set_otp_data(const unsigned char* data)
    {
        this->ft_para_arg_.m_param.m_otp.m_data = (unsigned char*)data;
    }

    void set_dl_handle(const DL_HANDLE_T handle)
    {
        this->ft_para_arg_.m_dl_handle = handle;
    }


    static int __stdcall cb_param_process_init(void *usr_arg);
    static int __stdcall cb_param_process(unsigned char percent, void *usr_arg);
    static int __stdcall cb_post_process_init(void *usr_arg);
    static int __stdcall cb_post_process(unsigned char percent, void *usr_arg);

private:
    void FillDefaultValue();
    FLASHTOOL_PARAMETER_ARG ft_para_arg_;
};

#endif // BROMOTPARG_H
