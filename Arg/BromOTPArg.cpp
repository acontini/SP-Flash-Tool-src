#include "BromOTPArg.h"

#include "../Logger/Log.h"

int __stdcall BromOTPArg::cb_param_process_init(void *usr_arg)
{
    LOG("Parameter process initial...%s", usr_arg);
    return 0;
}

int __stdcall BromOTPArg::cb_param_process(unsigned char percent, void *usr_arg)
{
    static unsigned char lastReportedPercent = 0;
    if((lastReportedPercent != percent) && percent % 10 != 0)
    {
        LOG("%d%% of parameter data has been processed.", percent);
        LOGD("user_arg(%s)", usr_arg);
        lastReportedPercent = percent;
    }
    return 0;
}
int __stdcall BromOTPArg::cb_post_process_init(void *usr_arg)
{
    LOG("Post process initial...");
    LOGD("user_arg(%s)", usr_arg);
    return 0;
}

int __stdcall BromOTPArg::cb_post_process(unsigned char percent, void *usr_arg)
{
    static unsigned char lastReportedPercent = 0;
    if((lastReportedPercent != percent) && percent % 10 != 0)
    {
        LOG("%d%% of post data has been processed.", percent);
        LOGD("user_arg(%s)", usr_arg);
        lastReportedPercent = percent;
    }
    return 0;
}

BromOTPArg::BromOTPArg()
{
    this->FillDefaultValue();
}

void BromOTPArg::FillDefaultValue()
{
     this->ft_para_arg_.m_param.m_otp_enable = _FALSE;
     this->ft_para_arg_.m_cb_param_process_init = cb_param_process_init;
     this->ft_para_arg_.m_cb_param_process_init_arg = NULL;
     this->ft_para_arg_.m_cb_param_process = cb_param_process;
     this->ft_para_arg_.m_cb_param_process_arg = NULL;
     this->ft_para_arg_.m_cb_post_process_init = cb_post_process_init;
     this->ft_para_arg_.m_cb_post_process_init_arg = NULL;
     this->ft_para_arg_.m_cb_post_process = cb_post_process;
     this->ft_para_arg_.m_cb_post_process_arg = NULL;
}
