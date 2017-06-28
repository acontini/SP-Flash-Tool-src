#include "CertDLCommand.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Handle/ScretHandle.h"
#include "../Conn/Connection.h"

#include "../Logger/Log.h"
#include "../Err/Exception.h"

namespace APCore
{

CertDLCommand::CertDLCommand(APKey key)
    :ICommand(key)
{
}

CertDLCommand::~CertDLCommand()
{

}

void CertDLCommand::exec(
        const QSharedPointer<Connection> &conn)
{
    SCERT_ARG_S cdl_arg;
    BOOT_RESULT cdl_res;
    int stop = 0;

    ArgFlashToolDLCert(cdl_arg, &cdl_res);

    conn->ConnectBROM();

    int ret = Brom_SendSCERTWithHandle(
                0,  // WHY do U need BROM_HANDLE when U have FT_HANDLE???
                conn->FTHandle(),
                &cdl_arg, &cdl_res, &stop);

    if(ret != S_DONE)
    {
        THROW_BROM_EXCEPTION(ret, 0);
    }
}

void CertDLCommand::ArgFlashToolDLCert(
        SCERT_ARG_S &cdl_arg,
        void *m_cb_in_brom_state_arg)
{
    memset(&cdl_arg, 0, sizeof(cdl_arg));
    cdl_arg.m_bbchip_type = AUTO_DETECT_BBCHIP;
    cdl_arg.m_ext_clock = AUTO_DETECT_EXT_CLOCK;
    cdl_arg.m_ms_boot_timeout = BOOT_INFINITE;
    cdl_arg.m_max_start_cmd_retry_count =
            DEFAULT_BROM_START_CMD_RETRY_COUNT;

    // whether need to register two callback function
    cdl_arg.m_cb_com_init_stage = cb_com_init_stage;
    cdl_arg.m_cb_com_init_stage_arg = this;

    cdl_arg.m_cb_in_brom_stage = cb_in_brom_stage;
    cdl_arg.m_cb_in_brom_stage_arg = m_cb_in_brom_state_arg;

    cdl_arg.m_scert_handle = GET_CERT_HANDLE_T(key_);
}

int CertDLCommand::cb_in_brom_stage(BROM_HANDLE_T brom_handle,
                                    HANDLE hCom, void *usr_arg)
{
    unsigned short value16;
    BBCHIP_TYPE *p_bbchip = (BBCHIP_TYPE*)usr_arg;

    if(NULL != p_bbchip && MT6218B == *p_bbchip)
    {
        // configure GPIO17 mode to GPIO, GPIO_MODES_CLR
        value16 = 0x000C;
        Brom_Write16Cmd(brom_handle, hCom,
                        (0x80120000 + 0x0120 + 0x08), &value16, 1);

        //configure GPIO17 direction as output GPIO_DIR2_SET
        value16 = 0x0002;
        Brom_Write16Cmd(brom_handle, hCom, (0x80120000 + 0x0010 + 0x04),
                        &value16, 1);

        // pull down GPIO17 to disable vibrator
        value16 = 0x0002;
        Brom_Write16Cmd(brom_handle, hCom, (0x80120000+0x00a0+0x08)/*GPIO_DOUT2_CLR*/,
                        &value16, 1);

    }

    return 0;
}

int CertDLCommand::cb_com_init_stage(HANDLE hCom, void *usr_arg)
{
    Q_UNUSED(usr_arg);
    Q_UNUSED(hCom);

  /*  OxQueryDeviceInfo deviceInfo;
    DWORD bytesReturned;
   // BOOL ok;

    deviceInfo.dwRequestedType = OX_FOR_SERIAL_DRIVER;

    // ok =
    DeviceIoControl( hCom,
                     IOCTL_OXSER_QUERY_INFO,
                     &deviceInfo, sizeof(deviceInfo),
                     &deviceInfo, sizeof(deviceInfo),
                     &bytesReturned,
                     NULL);

    if ( ok )
    {
        if ( bytesReturned >= sizeof(deviceInfo) && deviceInfo.dwMagicNumber == OXMAGICNUMBER )
        {
            // if Oxford Card, then call PowerOnOxford()
            if ( deviceInfo.ChipId == OXCF950_CHIP_ID )
            {
                bool bPoweronOk = PowerOnOxford( hCOM, usr_arg );
                if ( !bPoweronOk )
                {
                    t->asMsg.printf("Failed to power on Oxford");
                    t->SyncShowMsg();
                    return 1;
                };

                return 0;
            }
        }
    }
  */
    return 0;
}

}
