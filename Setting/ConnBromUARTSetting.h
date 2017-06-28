#ifndef CONNBROMUARTSETTING_H
#define CONNBROMUARTSETTING_H

#include "ConnSetting.h"
#include "../Arg/BootRomArg.h"

namespace APCore
{
class ConnBromUARTSetting : public ConnSetting
{
public:
    ConnBromUARTSetting(const std::string& port);
    ConnBromUARTSetting(const XML::Node &node);

    virtual Connection *CreateConnection(
            APKey key, HW_StorageType_E stor, bool pwr_key_reset);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    void set_cb_dl_da_init(
            CALLBACK_WRITE_BUF_PROGRESS_INIT cb_dl_da_init)
    {
        cb_download_da_init_ = cb_dl_da_init;
    }

    void set_cb_dl_da(
            CALLBACK_WRITE_BUF_PROGRESS cb_dl_da)
    {
        cb_download_da_ = cb_dl_da;
    }

    void set_baudrate(unsigned int baudrate)
    {
        m_baudrate = baudrate;
    }

    unsigned int get_baudrate()
    {
        return m_baudrate;
    }

    DA_LOG_LEVEL_E get_da_log_level()
    {
        return m_da_log_level;
    }

    void set_da_log_level(unsigned int log_level)
    {
        m_da_log_level = (DA_LOG_LEVEL_E)log_level;
    }

private:
    std::string com_port_;
    unsigned int m_baudrate;
    DA_LOG_LEVEL_E m_da_log_level;
    CALLBACK_WRITE_BUF_PROGRESS_INIT cb_download_da_init_;
    CALLBACK_WRITE_BUF_PROGRESS      cb_download_da_;
};

}

#endif // CONNBROMUARTSETTING_H
