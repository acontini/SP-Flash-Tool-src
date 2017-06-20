#ifndef CONNSETTING_H
#define CONNSETTING_H


#include "../BootRom/flashtool_api.h"
#include "../Conn/Connection.h"
#include "../XMLParser/XMLSerializable.h"
#include "../Logger/Log.h"

namespace APCore
{

class ConnSetting : public XML::Serializable
{
public:
    ConnSetting();
    virtual ~ConnSetting();

    virtual Connection *CreateConnection(
            APKey key, HW_StorageType_E stor, bool pwr_key_reset) = 0;

    void set_stop_flag(int * p_stop)
    {
        stop_flag_ = p_stop;
    }

    void set_timeout(int timeout)
    {
        m_timeout = timeout;
    }

    void set_com_port(std::string com_port)
    {
        m_com_port = com_port;
    }

    static ConnSetting* FromXML(const XML::Node &node);

protected:
    int *get_stop_flag()
    {
        return stop_flag_;
    }

    int get_timeout()
    {
        return m_timeout;
    }
protected:
    int *stop_flag_;
    int m_timeout;
    std::string m_com_port;
};


}

#endif // CONNSETTING_H
