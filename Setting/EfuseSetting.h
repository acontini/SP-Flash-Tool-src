#ifndef _EFUSE_SETTING_H_
#define _EFUSE_SETTING_H_

#include "ISetting.h"
#include "../BootRom/flashtool_api.h"

#ifndef _SHARED_PTR
#define _SHARED_PTR QSharedPointer
#endif

#define EFUSE_STK_KEY_NUMBER        16

typedef struct
{
   bool bypass_c_ctrl0;
   bool bypass_c_ctrl1;
   bool bypass_c_data0;
   bool bypass_c_data1;
   bool bypass_c_data2;
   bool bypass_c_data3;
   bool bypass_c_data4;
   bool bypass_c_data5;
}  bypass_items;

namespace APCore
{
class EfuseSetting : public ISetting
{
public:
    EfuseSetting();
    virtual ~EfuseSetting();

    virtual _SHARED_PTR<ICommand> CreateCommand(APKey key);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    const Efuse_Common_Arg *CommonArg() const
    {
        return &common_arg_;
    }
    const Efuse_Secure_Arg *SecureArg() const
    {
        return &secure_arg_;
    }
    const Efuse_Lock_Arg *LockArg() const
    {
        return &lock_arg_;
    }

    const Efuse_STB_Key_Arg *StbKeyArg() const
    {
        return &stb_key_arg_;
    }

    const Efuse_Extra_Arg* extraArg() const
    {
        return &extra_arg_;
    }

    std::string ReadbackFile() const;

    void SetOnlyRead(bool onlyOutput){
        only_output_ = onlyOutput;
    }

private:
    EfuseSetting(const EfuseSetting&);
    EfuseSetting &operator=(const EfuseSetting&);

    void LoadCommonCtrl(const XML::Node &node);
    void LoadUsbID(const XML::Node &node);
    void LoadSpare(const XML::Node &node);

    void LoadSecureCtrl(const XML::Node &node);
    void LoadSbcPubKey(const XML::Node &node);
    void LoadAcKey(const XML::Node &node);

    void LoadCLock(const XML::Node &node);
    void LoadMHWRes(const XML::Node &node);
    void LoadSecCtrl1Key(const XML::Node &node);
    void LoadCtrlKey(const XML::Node &node);

    void LoadCCtrl0Key(const XML::Node &node);
    void LoadCustomerCtrl0Key(const XML::Node &node);
    void LoadCCtrl1Key(const XML::Node &node);
    void LoadCData0(const XML::Node &node);
    void LoadCData1(const XML::Node &node);
    void LoadCData2(const XML::Node &node);
    void LoadCData3(const XML::Node &node);
    void LoadCData4(const XML::Node &node);
    void LoadCData5(const XML::Node &node);                    

    void LoadUnitValue(EFUSE_KEY key, const XML::Node &node);
    void LoadKey(EFUSE_KEY key,const XML::Node &node);
    void LoadCommonLock(const XML::Node &node);
    void LoadSecureLock(const XML::Node &node);

    void LoadReadBack(const XML::Node &node);

    void LoadSTBLock(const XML::Node &node);
    void LoadSTBID(const XML::Node &node);
    void LoadSTBKey(const XML::Node &node);

    void LoadSecMsc(const XML::Node &node);
    void LoadCmFlag(const XML::Node &node);
    void LoadCustk(const XML::Node &node);
    void LoadBypassItems(const XML::Node &node);
    void BypassKeys(EFUSE_KEY key);
    void LoadSbcPubKey1(const XML::Node &node);

private:
    Efuse_Common_Arg    common_arg_;
    Efuse_Secure_Arg    secure_arg_;
    Efuse_Lock_Arg      lock_arg_;
    Efuse_STB_Key_Arg   stb_key_arg_;
    Efuse_Extra_Arg     extra_arg_;

    std::string         output_dir_;
    std::string         output_file_;
    bool                time_prefix_;
    bool                only_output_;
    bool                b_output_stb_lock_;
    bool                b_output_stb_key_;

    bypass_items bypass_rule_;
};

}

#endif // _EFUSE_SETTING_H_
