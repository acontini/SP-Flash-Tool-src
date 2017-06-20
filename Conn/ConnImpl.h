/*
 * ConnImpl.h
 *
 */

#ifndef _CONNECTION_IMPL_H_
#define _CONNECTION_IMPL_H_

#include "../BootRom/flashtool.h"
#include "../Public/Host.h"


namespace APCore
{

class ConnMedia;
class ConnArgBox;

class ConnBase
{
public:
    virtual ~ConnBase()
    {
    }

    virtual int Connect(ConnMedia*, ConnArgBox*) = 0;
    virtual int Disconnect(ConnArgBox*) = 0;
};

class BromConnection : public ConnBase
{
public:
    virtual ~BromConnection();

    virtual int Connect(ConnMedia*, ConnArgBox*);
    virtual int Disconnect(ConnArgBox*);
};

class DAConnection : public ConnBase
{
public:
    virtual ~DAConnection();

    virtual int Connect(ConnMedia*, ConnArgBox*);
    virtual int Disconnect(ConnArgBox*);

public:
    DA_REPORT_T &Result()
    {
        return conn_result_.m_da_report;
    }

protected:
    FlashTool_Connect_Result conn_result_;
};


} /* namespace APCore */
#endif /* _CONNECTION_IMPL_H_ */
