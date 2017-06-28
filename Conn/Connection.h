/*
 * Connection.h
 *
 *  Created on: Aug 27, 2011
 *      Author: MTK81019
 */

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "../BootRom/flashtool.h"
#include "../Public/Host.h"
#include "ConnImpl.h"
#include "ConnMedia.h"
#include "ConnArgBox.h"
#include <QSharedPointer>
#include <list>

namespace APCore
{

#ifndef _SHARED_PTR
#define _SHARED_PTR QSharedPointer
#endif

#define _SHR_OBSERVER_LIST  \
    std::list<_SHARED_PTR<ConnObserver> >

#define _SHR_OBSERVER_ITERC  \
    _SHR_OBSERVER_LIST::const_iterator

class ConnObserver
{
public:
    virtual void OnBromConnected(const BOOT_RESULT&, const std::string &) = 0;
    virtual void OnDAConnected(const DA_REPORT_T&, const std::string &, const int) = 0;
    /*
    virtual void OnDisconnected() = 0;
    */
    virtual ~ConnObserver(){};
};

class Connection
{
public:
    Connection(
            _SHARED_PTR<ConnMedia> med,
            _SHARED_PTR<ConnArgBox> box)
        : connected_(0),
          p_media_(med),
          p_arg_box_(box),
          p_inner_conn_(NULL)
    {
    }

    ~Connection()
    {
        Disconnect();
    }

public:
/*
    bool IsConnected() const
    {
        return connected_ > 0;
    }
*/
    const std::string &Device() const;

    const std::string &FriendlyName() const;

    int USBStatus() const;

    FLASHTOOL_API_HANDLE_T FTHandle() const;

    APKey ap_key() const;

    DA_REPORT_T &da_report();
    const BOOT_RESULT &boot_result();
/*
    void ResetWDT(
            const FlashTool_EnableWDT_Arg *p_wdt_arg);
*/
    int ConnectBROM();
    int ConnectDA(CONN_DA_END_STAGE stage = SECOND_DA, _BOOL enableDRAMin1stDA = _FALSE);

    int Disconnect();

    void AttachObserver(_SHARED_PTR<ConnObserver> o)
    {
        observer_list_.push_back(o);
    }

    void setPowerType(int force_chg);
    bool isDAConnected(){return connected_ > 1;}
private:
    void NotifyBromConnected(const BOOT_RESULT&, const std::string&) const;
    void NotifyDAConnected(const DA_REPORT_T&, const std::string&, const int usb_status) const;
    /*
    void NotifyDisconnected();
    */
private:
    int connected_;
    _SHARED_PTR<ConnMedia> p_media_;
    _SHARED_PTR<ConnArgBox> p_arg_box_;
    _SHARED_PTR<ConnBase> p_inner_conn_;
    _SHR_OBSERVER_LIST observer_list_;
};


} /* namespace APCore */
#endif /* _CONNECTION_H_ */
