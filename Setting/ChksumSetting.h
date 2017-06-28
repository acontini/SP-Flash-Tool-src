#ifndef CHKSUM_SETTING_H
#define CHKSUM_SETTING_H

#include "../BootRom/flashtool_api.h"
#include "../Arg/BromChksumArg.h"
#include "../XMLParser/XMLSerializable.h"

namespace APCore{

class ChksumSetting : public XML::Serializable
{
public:
    ChksumSetting();
    ChksumSetting(const XML::Node &node);
    ~ChksumSetting();

    void set_chksum_value(unsigned int value)
    {
       this->chksum_value_ = value;
    }

    void set_chksum_index(unsigned short index)
    {
       this->index_ = index;
    }

    void set_cb_checksum_init(CALLBACK_ROM_MEM_CHECKSUM_PROGRESS_INIT cb)
    {
        cb_checksum_init_ = cb;
    }

    void set_cb_checksum_progress(CALLBACK_ROM_MEM_CHECKSUM_PROGRESS cb)
    {
        cb_checksum_progress_ = cb;
    }

    void set_stop_flag(int * p_stop)
    {
        stop_flag_ = p_stop;
    }

    void set_chksum_level(int checksum_level)
    {
        chksum_level_ = checksum_level;
    }

    int chksum_level() const
    {
        return chksum_level_;
    }

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;
    static ChksumSetting* FromXML(const XML::Node &node);

   BromChksumArg GetBromChksumArg() const;

   ChksumSetting* Clone() const;
private:
    CALLBACK_ROM_MEM_CHECKSUM_PROGRESS_INIT cb_checksum_init_;
    CALLBACK_ROM_MEM_CHECKSUM_PROGRESS cb_checksum_progress_;

    unsigned int chksum_value_;
    unsigned short index_;
    int *stop_flag_;
    unsigned int chksum_level_;

};
}
#endif
