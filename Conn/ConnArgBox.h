/*
 * Connection.h
 *
 *  Created on: Aug 27, 2011
 *      Author: MTK81019
 */

#ifndef _CONN_ARG_BOX_H_
#define _CONN_ARG_BOX_H_

#include "Connection.h"

namespace APCore
{

class ConnArgBox
{
public:
    ConnArgBox(
            APKey key,
            int *stop_flag,
            int timeout,
            HW_StorageType_E stor,
            const FlashTool_Connect_Arg &connArg);

public:
    const FlashTool_Connect_Arg *FTConnArg();

    FLASHTOOL_API_HANDLE_T FTHandle() const
    {
        return ft_handle_;
    }

    void SetFTHandle(FLASHTOOL_API_HANDLE_T handle)
    {
        ft_handle_ = handle;
    }

    APKey ap_key() const
    {
        return key_;
    }

    int *StopFlag() const
    {
        return stop_flag_;
    }

    int timeout() const
    {
        return timeout_;
    }

    void SetPowerType(int force_chg)
    {
        conn_arg_.m_force_charge = force_chg;
    }

    void SetConnDAEndStage(CONN_DA_END_STAGE stage)
    {
        conn_arg_.m_conn_da_end_stage = stage;
    }

    void Set1stDAEnableDRAM(_BOOL enable)
    {
        conn_arg_.m_1st_da_enable_dram = enable;
    }

private:
    APKey key_;
    int *stop_flag_;
    int timeout_;
    HW_StorageType_E stor_;
    FlashTool_Connect_Arg conn_arg_;
    FLASHTOOL_API_HANDLE_T ft_handle_;
};

} /* namespace APCore */
#endif /* _CONN_ARG_BOX_H_ */
