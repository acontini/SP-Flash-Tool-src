#include "../Resource/ResourceManager.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/DAHandle.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../Conn/Connection.h"
#include "../Err/Exception.h"
#include "../Logger/Log.h"
#include "EfuseCommand.h"
#include "../Utility/IniItem.h"

#define sbc_pub_key_hash \
    sbc_pub_key_u.r_sbc_pub_key_hash

#define _sbc_key_E  \
    sbc_pub_key_u.w_sbc_pub_key.key_e

#define _sbc_key_N  \
    sbc_pub_key_u.w_sbc_pub_key.key_n

#define sbc_pub_key_hash1 \
    sbc_pub_key1_u.r_sbc_pub_key_hash1

#define _sbc_key1_E  \
    sbc_pub_key1_u.w_sbc_pub_key1.key_e

#define _sbc_key1_N  \
    sbc_pub_key1_u.w_sbc_pub_key1.key_n


#define EfuseOpt2Text(opt)  \
    ((opt)==EFUSE_ENABLE ? "on" : "off")
  
#define DumpEfuseOpt(p, sec) \
    LOGI(#sec " = %s", EfuseOpt2Text(p->sec))

#define DumpEfuseOptEx(p, sec, key) \
    LOGI("%s = %s", key.c_str(), EfuseOpt2Text(p.sec))

#define DumpInteger(p, sec) \
    LOGI(#sec " = %d", p->sec)

#define DumpIntegerEx(p, sec, key) \
    LOGI("%s = %d", key.c_str(), p.sec)

#define DumpShortHex(p, sec) \
    LOGI(#sec " = %04x", p->sec)

#define DumpIntHex(p, sec) \
    LOGI(#sec " = %08x", p->sec)

#define DumpIntHexEx(p, sec, key) \
    LOGI("%s = %08x", key.c_str(), p.sec)

#define DumpText(p, sec) \
    LOGI(#sec " = %s", (p->sec).buf)

#define DumpTextEx(p, sec, key) \
    LOGI("%s = %s", key.c_str(), (p.sec).buf)

#define DumpBinary(p, sec) \
    LOGI(#sec " = %s", Binary2Str(p->sec).c_str())

#define DumpBinaryEx(p, sec, key) \
    LOGI("%s = %s", key.c_str(), Binary2Str(p.sec).c_str())

#define DumpEfuseSTBOpt(p, sec) \
    LOGI(#sec " = %s", EfuseOpt2Text(p.sec))

#define DumpSTBKeyName(p, sec) \
     LOGI(#sec " = %s", p.sec)

#define DumpSTBText(p, sec) \
    LOGI(#sec " = %s", (p.sec).buf)

#define DumpSTBBinary(p, sec) \
    LOGI(#sec " = %s", Binary2Str(p.sec).c_str())

#define SaveEfuseOpt(f, p, sec) \
    fprintf(f, #sec " = %s\r\n", EfuseOpt2Text(p->sec))

#define SaveEfuseOptEx(f, p, sec, key) \
    fprintf(f, "%s =%s\r\n", key.c_str(), EfuseOpt2Text(p.sec))

#define SaveInteger(f, p, sec) \
    fprintf(f, #sec " = %d\r\n", p->sec)

#define SaveIntegerEx(f, p, sec, key) \
    fprintf(f, "%s = %d\r\n", key.c_str(), p.sec)

#define SaveShortHex(f, p, sec) \
    fprintf(f, #sec " = %04x\r\n", p->sec)

#define SaveIntHex(f, p, sec)   \
    fprintf(f, #sec " = %08x\r\n", p->sec)

#define SaveIntHexEx(f, p, sec, key)   \
    fprintf(f,  "%s = %08x\r\n", key.c_str(), p->sec)

#define SaveBinary(f, p, sec) \
    fprintf(f, #sec " = %s\r\n", Binary2Str(p->sec).c_str())

#define SaveBinaryEx(f, p, sec, key) \
    fprintf(f, "%s = %s\r\n", key.c_str(), Binary2Str(p.sec).c_str())

#define SaveSTBKeyName(f, p, sec) \
    fprintf(f, #sec " = %s\r\n", p.sec)

#define SaveSTBBinary(f, p, sec) \
    fprintf(f, #sec " = %s\r\n", Binary2Str(p.sec).c_str())

using std::string;

namespace APCore
{

// class EfuseCommand
EfuseCommand::EfuseCommand(
        APKey key,
        EfuseSetting *p)
    : ICommand(key),
      p_setting_(p),
      read_only_(false),
      stb_lock_enable_(false),
      stb_key_enable_(false)
{
}

EfuseCommand::~EfuseCommand()
{
}

void EfuseCommand::exec(
        const QSharedPointer<Connection> &conn)
{
    Q_ASSERT(p_setting_ != NULL);

    Connect(conn.data());

    if(!read_only_)
    WriteAll(conn.data());

    ReadAll(conn.data());
}

void EfuseCommand::Connect(Connection *conn)
{
    conn->ConnectBROM();

    IniItem item("option.ini", "BLOWN", "brom");
    bool brom_blown = item.GetBooleanValue();

    switch (conn->boot_result().m_bbchip_type)
    {
    case MT6573:
    case MT6575:
    case MT6577:
    case MT6589:
        break;
    case MT8135:
        TryConnectDA(conn);
        break;

	default:
        if(!brom_blown){
            conn->ConnectDA();
        }

        break;
    }
}

void EfuseCommand::TryConnectDA(Connection *conn)
{
    if (GET_DA_HANDLE_T(conn->ap_key()) != NULL &&
        GET_DL_HANDLE_T(conn->ap_key()) != NULL)
    {
        conn->ConnectDA();
    }
}

void EfuseCommand::WriteAll(Connection *conn)
{
    DumpSetting();

    LOGI("Writing Efuse registers...");

    int ret = FlashTool_WriteEfuseAll_Ex(
                conn->FTHandle(),
                p_setting_->CommonArg(),
                p_setting_->SecureArg(),
                p_setting_->LockArg(),
                p_setting_->StbKeyArg(),
                p_setting_->extraArg());

    if (S_DONE != ret)
    {
        LOGI("Write Efuse registers failed: %s",
             StatusToString(ret));
        THROW_APP_EXCEPTION(ret, "");
    }
    else
    {
        LOGI("Efuse registers written successfully");
    }
}

void EfuseCommand::ReadAll(Connection *conn)
{
    string rb_file = p_setting_->ReadbackFile();

    if (!rb_file.empty())
    {
        Efuse_Common_Arg common_arg;// = { 0 };
        memset(&common_arg, 0, sizeof(common_arg));
        Efuse_Secure_Arg secure_arg;// = { 0 };
        memset(&secure_arg, 0, sizeof(secure_arg));
        Efuse_Lock_Arg   lock_arg;//   = { 0 };
        memset(&lock_arg, 0, sizeof(lock_arg));
        Efuse_STB_Key_Arg stb_arg;// = { 0 };
        memset(&stb_arg, 0, sizeof(stb_arg));
        Efuse_Extra_Arg   extra_arg;// = { 0 };
        memset(&extra_arg, 0, sizeof(extra_arg));

        char spare_buf[64] = { 0 };
        char ackey_buf[16] = { 0 };
        char sbcpk_buf[32] = { 0 };
        char sbcpk1_buf[32] = { 0 };

        char stbkey_bufs[EFUSE_STK_KEY_NUMBER][32] = {{0}};
        char stbkey_names[EFUSE_STK_KEY_NUMBER][64] = {{0}};

        common_arg.spare.buf = spare_buf;
        common_arg.spare.buf_len = sizeof(spare_buf);

        secure_arg.ac_key.buf = ackey_buf;
        secure_arg.ac_key.buf_len = sizeof(ackey_buf);

        secure_arg.sbc_pub_key_hash.buf = sbcpk_buf;
        secure_arg.sbc_pub_key_hash.buf_len = sizeof(sbcpk_buf);

        secure_arg.sbc_pub_key_hash1.buf = sbcpk1_buf;
        secure_arg.sbc_pub_key_hash1.buf_len = sizeof(sbcpk1_buf);

        char extra_bufs[END_KEY][64] = {{0}};

        for(int i = 0; i < EFUSE_STK_KEY_NUMBER; i++)
        {
            stb_arg.stb_blow_keys[i].key_name = stbkey_names[i];
            stb_arg.stb_blow_keys[i].stb_key.buf = stbkey_bufs[i];
            stb_arg.stb_blow_keys[i].stb_key.buf_len = sizeof(stbkey_bufs[i]);
        }

        for(int i = 0; i < END_KEY; i++)
        {
            extra_arg.items[i].data.key_pair.key.buf = extra_bufs[i];
        }

        LOGI("Reading Efuse registers...");

        int ret = FlashTool_ReadEfuseAll_Ex(
                    conn->FTHandle(),
                    &common_arg,
                    &secure_arg,
                    &lock_arg,
                    &stb_arg,
                    &extra_arg);

        if (S_DONE != ret)
        {
            LOGI("Read Efuse registers failed: %s",
                 StatusToString(ret));
            THROW_APP_EXCEPTION(ret, "");
        }
        else
        {
            LOGI("Efuse registers read successfully");

            DumpResult(rb_file,
                       &common_arg,
                       &secure_arg,
                       &lock_arg,
                       &stb_arg,
                       &extra_arg);
        }

        LOGI("Efuse Dump done");
    }
}

string EfuseCommand::Binary2Str(
        const _BUFFER &ref)
{
    string str;
    char tmp[16];

    for (U32 i=0; i<ref.buf_len; i+=sizeof(U32))
    {
        sprintf(tmp, "%08X ", *(U32*)(ref.buf+i));
        str += tmp;
    }

    return str;
}

void EfuseCommand::DumpCommonArg(
        const Efuse_Common_Arg *common,
        bool to_write /*= true*/)
{
    LOGI("========== Efuse Common Setting ==========");

    DumpEfuseOpt(common, emmc_boot_dis);
    DumpEfuseOpt(common, nand_boot_dis);
    DumpEfuseOpt(common, nand_boot_speedup_dis);
    DumpEfuseOpt(common, pid_vid_custom_en);
    DumpEfuseOpt(common, ufs_boot_dis);
    DumpEfuseOpt(common, sbc_pub_hash_dis);
    DumpEfuseOpt(common, sbc_pub_hash1_dis);

    if (to_write)
    {
        DumpEfuseOpt(common, spare_blow);
    }

    if (common->spare.buf_len > 0)
    {
        if (to_write)
            DumpText(common, spare);
        else
            DumpBinary(common, spare);
    }

    DumpInteger(common, usbdl_type);

    if (to_write)
    {
        DumpEfuseOpt(common, usb_id_blow);
    }

    if (common->usb_id_blow == EFUSE_ENABLE || !to_write)
    {
        DumpShortHex(common, usb_pid);
        DumpShortHex(common, usb_vid);
    }
}

void EfuseCommand::DumpSecureArg(
        const Efuse_Secure_Arg *secure,
        bool to_write /*= true*/)
{
    LOGI("========== Efuse Secure Setting ==========");

    DumpEfuseOpt(secure, acc_en);
    DumpEfuseOpt(secure, sbc_en);
    DumpEfuseOpt(secure, daa_en);
    DumpEfuseOpt(secure, sla_en);
    DumpEfuseOpt(secure, ack_en);
    DumpEfuseOpt(secure, jtag_dis);
    DumpEfuseOpt(secure, jtag_sw_con);
    DumpEfuseOpt(secure, dbgport_lock_dis);
    DumpEfuseOpt(secure, md1_sbc_en);
    DumpEfuseOpt(secure, c2k_sbc_en);
    DumpEfuseOpt(secure, pl_ar_en);
    DumpEfuseOpt(secure, pk_cus_en);

    if (to_write)
    {
        DumpEfuseOpt(secure, ac_key_blow);
    }

    if(secure->ac_key.buf_len > 0)
    {
        if (to_write)
            DumpText(secure, ac_key);
        else
            DumpBinary(secure, ac_key);
    }

    if (to_write)
    {
        DumpEfuseOpt(secure, sbc_pubk_blow);

        if(secure->_sbc_key_E.buf_len > 0)
        {
            LOGI("sbc_public_key_e = %s",
                 secure->_sbc_key_E.buf);
        }
        if(secure->_sbc_key_N.buf_len > 0)
        {
            LOGI("sbc_public_key_n = %s",
                 secure->_sbc_key_N.buf);
        }
    }
    else if (secure->sbc_pub_key_hash.buf_len > 0)
    {
        DumpBinary(secure, sbc_pub_key_hash);
    }

     if (to_write)
    {
        DumpEfuseOpt(secure, sbc_pubk1_blow);

        if(secure->_sbc_key1_E.buf_len > 0)
        {
            LOGI("sbc_public_key1_e = %s",
                 secure->_sbc_key1_E.buf);
        }
        if(secure->_sbc_key1_N.buf_len > 0)
        {
            LOGI("sbc_public_key1_n = %s",
                 secure->_sbc_key1_N.buf);
        }
    }
    else if (secure->sbc_pub_key_hash1.buf_len > 0)
    {
        DumpBinary(secure, sbc_pub_key_hash1);
    }
}

void EfuseCommand::DumpLockArg(
        const Efuse_Lock_Arg *lock)
{
    LOGI("========== Efuse Lock Setting ==========");

    DumpEfuseOpt(lock, common_ctrl_lock);
    DumpEfuseOpt(lock, usb_id_lock);
    DumpEfuseOpt(lock, spare_lock);
    DumpEfuseOpt(lock, sec_ctrl_lock);
    DumpEfuseOpt(lock, ackey_lock);
    DumpEfuseOpt(lock, sbc_pubk_hash_lock);
    DumpEfuseOpt(lock, sec_msc_lock);
    DumpEfuseOpt(lock, custk_lock);
    DumpEfuseOpt(lock, sbc_pubk_hash1_lock);
}

void EfuseCommand::DumpSTBLockArg(const STB_Lock_PARAM *stb_lock)
{
    LOGI("========== Efuse STB Lock Setting ==========");

    DumpEfuseOpt(stb_lock, stb_key_g7_lock);
    DumpEfuseOpt(stb_lock, stb_key_g6_lock);
    DumpEfuseOpt(stb_lock, stb_key_g5_lock);
    DumpEfuseOpt(stb_lock, stb_key_g4_lock);
    DumpEfuseOpt(stb_lock, stb_key_g3_lock);
    DumpEfuseOpt(stb_lock, stb_key_g2_lock);
    DumpEfuseOpt(stb_lock, stb_key_g1_lock);
    DumpEfuseOpt(stb_lock, stb_key_g0_lock);
    DumpEfuseOpt(stb_lock, stb_key_operatorid_lock);
    DumpEfuseOpt(stb_lock, stb_key_chipid_lock);
    DumpEfuseOpt(stb_lock,  stb_key_sn_lock);
}

void EfuseCommand::DumpSTBArg(const Efuse_STB_Key_Arg *stb,
                              bool to_write)
{
    if(stb_lock_enable_)
        DumpSTBLockArg(&stb->stb_lock);

    if(!stb_key_enable_)
        return;

    LOGI("========== Efuse STB Key Setting ==========");

    if (to_write)
    {
        DumpEfuseOpt(stb, stb_key_chipid_blow);
    }

    if (stb->stb_key_chipid_blow == EFUSE_ENABLE || !to_write)
    {
        DumpShortHex(stb, stb_key_chipID);
    }

    if(to_write)
    {
        DumpEfuseOpt(stb, stb_key_operatorid_blow);
    }

    if(stb->stb_key_operatorid_blow == EFUSE_ENABLE || !to_write)
    {
        DumpShortHex(stb, stb_key_operatorid);
    }

    for(int i = 0; i < 16; i++)
    {
        if (to_write)
        {
            DumpEfuseSTBOpt(stb->stb_blow_keys[i], key_blow);
        }

        if(stb->stb_blow_keys[i].stb_key.buf_len > 0 &&
                stb->stb_blow_keys[i].key_name != NULL &&
                strlen(stb->stb_blow_keys[i].key_name) > 0)
        {
            DumpSTBKeyName(stb->stb_blow_keys[i], key_name);

            if (to_write)
                DumpSTBText(stb->stb_blow_keys[i], stb_key);
            else
                DumpSTBBinary(stb->stb_blow_keys[i], stb_key);
        }
    }
}

#define KeyToString(key) EfuseExtraKeyToString(key)

void EfuseCommand::DumpExtraArg(const Efuse_Extra_Arg *extra,
                                bool to_write)
{
    LOGI("========== Efuse Extra Setting ==========");

    for(int i = 0; i < END_KEY; i++)
    {
        std::string key = KeyToString((EFUSE_KEY)extra->items[i].key);

        switch(extra->items[i].type)
        {
        case T_BOOLEAN:
            DumpEfuseOptEx(extra->items[i], data.enable, key);
            break;

        case T_INT:
            DumpIntHexEx(extra->items[i], data.iPair.value, key);

            break;

        case T_BUF:
            if(extra->items[i].data.key_pair.key.buf_len > 0)
            {
                if(to_write)
                    DumpTextEx(extra->items[i], data.key_pair.key, key);
                else
                    DumpBinaryEx(extra->items[i], data.key_pair.key, key);
            }
            break;
        }
    }
}

void EfuseCommand::DumpSetting()
{
    DumpCommonArg(p_setting_->CommonArg());
    DumpSecureArg(p_setting_->SecureArg());
    DumpLockArg(p_setting_->LockArg());
    DumpSTBArg(p_setting_->StbKeyArg());
    DumpExtraArg(p_setting_->extraArg());
}

void EfuseCommand::DumpResult(
        const std::string &rb_file,
        const Efuse_Common_Arg *common,
        const Efuse_Secure_Arg *secure,
    const Efuse_Lock_Arg *lock,
        const Efuse_STB_Key_Arg *stb,
        const Efuse_Extra_Arg   *extra)
{
    FILE *fp = fopen(rb_file.c_str(), "wb");

    if (NULL == fp)
    {
        LOGE("Failed to open file: %s",
             rb_file.c_str());
    }
    else
    {
        SaveCommonArg(fp, common);
        SaveSecureArgR(fp, secure);
        SaveLockArg(fp, lock);

        SaveStbArg(fp, stb);
        SaveExtraArg(fp, extra);
        fclose(fp);
    }

    DumpCommonArg(common, false);
    DumpSecureArg(secure, false);
    DumpLockArg(lock);
    DumpSTBArg(stb, false);
    DumpExtraArg(extra, false);
}

void EfuseCommand::SaveCommonArg(
        FILE *rb_file,
        const Efuse_Common_Arg *common)
{
    fprintf(rb_file,
            "========== Efuse Common Setting ==========\r\n");

    SaveEfuseOpt(rb_file, common, emmc_boot_dis);
    SaveEfuseOpt(rb_file, common, nand_boot_dis);
    SaveEfuseOpt(rb_file, common, nand_boot_speedup_dis);
    SaveEfuseOpt(rb_file, common, pid_vid_custom_en);
    SaveEfuseOpt(rb_file, common, sbc_pub_hash_dis);
    SaveEfuseOpt(rb_file, common, sbc_pub_hash1_dis);

    if (common->spare.buf_len > 0)
    {
        SaveBinary(rb_file, common, spare);
    }

    SaveInteger(rb_file, common, usbdl_type);

    if (common->usb_id_blow == EFUSE_ENABLE)
    {
        SaveShortHex(rb_file, common, usb_pid);
        SaveShortHex(rb_file, common, usb_vid);
    }

    fprintf(rb_file, "\r\n");
}

void EfuseCommand::SaveSecureArgR(
        FILE *rb_file,
        const Efuse_Secure_Arg *secure)
{
    fprintf(rb_file,
            "========== Efuse Secure Setting ==========\r\n");

    SaveEfuseOpt(rb_file, secure, acc_en);
    SaveEfuseOpt(rb_file, secure, sbc_en);
    SaveEfuseOpt(rb_file, secure, daa_en);
    SaveEfuseOpt(rb_file, secure, sla_en);
    SaveEfuseOpt(rb_file, secure, ack_en);
    SaveEfuseOpt(rb_file, secure, jtag_dis);
    SaveEfuseOpt(rb_file, secure, jtag_sw_con);
    SaveEfuseOpt(rb_file, secure, rom_cmd_dis);
    SaveEfuseOpt(rb_file, secure, dbgport_lock_dis);
    SaveEfuseOpt(rb_file, secure, md1_sbc_en);
    SaveEfuseOpt(rb_file, secure, c2k_sbc_en);
    SaveEfuseOpt(rb_file, secure, pl_ar_en);
    SaveEfuseOpt(rb_file, secure, pk_cus_en);

    if (secure->ac_key.buf_len > 0)
    {
        SaveBinary(rb_file, secure, ac_key);
    }
    if (secure->sbc_pub_key_hash.buf_len > 0)
    {
        SaveBinary(rb_file, secure, sbc_pub_key_hash);
    }

    if (secure->sbc_pub_key_hash1.buf_len > 0)
    {
        SaveBinary(rb_file, secure, sbc_pub_key_hash1);
    }


    fprintf(rb_file, "\r\n");
}

void EfuseCommand::SaveLockArg(
        FILE *rb_file,
        const Efuse_Lock_Arg *lock)
{
    fprintf(rb_file,
            "========== Efuse Lock Setting ==========\r\n");

    SaveEfuseOpt(rb_file, lock, common_ctrl_lock);
    SaveEfuseOpt(rb_file, lock, usb_id_lock);
    SaveEfuseOpt(rb_file, lock, spare_lock);
    SaveEfuseOpt(rb_file, lock, sec_ctrl_lock);
    SaveEfuseOpt(rb_file, lock, ackey_lock);
    SaveEfuseOpt(rb_file, lock, sbc_pubk_hash_lock);
    SaveEfuseOpt(rb_file, lock, sec_msc_lock);
    SaveEfuseOpt(rb_file, lock, custk_lock);
    SaveEfuseOpt(rb_file, lock, sbc_pubk_hash1_lock);

    fprintf(rb_file, "\r\n");
}

void EfuseCommand::SaveStbArg(FILE *rb_file,
                              const Efuse_STB_Key_Arg *stb)
{
    if(stb_lock_enable_)
        SaveSTBKeyLockArg(rb_file, &stb->stb_lock);

    if(!stb_key_enable_)
        return;

    fprintf(rb_file,
            "========== Efuse STB Key Setting ==========\r\n");

    if (stb->stb_key_chipid_blow == EFUSE_ENABLE)
    {
        SaveShortHex(rb_file, stb, stb_key_chipID);
    }

    if(stb->stb_key_operatorid_blow == EFUSE_ENABLE)
    {
        SaveShortHex(rb_file, stb, stb_key_operatorid);
    }

    for(int i = 0; i < 16; i++)
    {
        if(stb->stb_blow_keys[i].stb_key.buf_len > 0 &&
                stb->stb_blow_keys[i].key_name != NULL &&
                strlen(stb->stb_blow_keys[i].key_name) > 0)
        {
            SaveSTBKeyName(rb_file, stb->stb_blow_keys[i], key_name);
            SaveSTBBinary(rb_file, stb->stb_blow_keys[i], stb_key);
    }
    }

    fprintf(rb_file, "\r\n");
}

void EfuseCommand::SaveSTBKeyLockArg(FILE *rb_file,
                                     const STB_Lock_PARAM *stb_lock)
{
    fprintf(rb_file,
            "========== Efuse STB Key Lock Setting ==========\r\n");

    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g7_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g6_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g5_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g4_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g3_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g2_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g1_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_g0_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_operatorid_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_chipid_lock);
    SaveEfuseOpt(rb_file, stb_lock,  stb_key_sn_lock);

     fprintf(rb_file, "\r\n");
}

void EfuseCommand::SaveExtraArg(FILE *rb_file,
                                const Efuse_Extra_Arg *extra)
{
    fprintf(rb_file,
            "==========Efuse Extra Setting =======\r\n");

    for(int i = 0; i < END_KEY; i++)
    {
        std::string key = KeyToString((EFUSE_KEY)extra->items[i].key);

        switch(extra->items[i].type)
        {
        case T_BOOLEAN:
            SaveEfuseOptEx(rb_file, extra->items[i], data.enable, key);
            break;

        case T_INT:
            SaveIntegerEx(rb_file, extra->items[i], data.iPair.value, key);
            break;

        case T_BUF:
            if(extra->items[i].data.key_pair.key.buf_len > 0)
            {
                SaveBinaryEx(rb_file, extra->items[i], data.key_pair.key, key);
            }
            break;
        }
    }

    fprintf(rb_file, "\r\n");
}

} /*namespace APCore*/
