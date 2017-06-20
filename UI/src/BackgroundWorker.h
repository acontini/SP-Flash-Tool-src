#ifndef BACKGROUNDWORKER_H
#define BACKGROUNDWORKER_H

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include "../../Setting/ISetting.h"
#include "../../Setting/ConnSetting.h"
#include "../../Setting/PlatformSetting.h"
#include "../../Public/AppCore.h"
#include "../../Conn/Connection.h"
#include "ICallback.h"
#include <iostream>
#include <list>

class MainController;

class BackgroundWorker : public QThread
{
    Q_OBJECT
public:
    //TODO: refine & unify the callback mechanism(such as std::bind...)
    typedef void (__stdcall * void_callback)();

    explicit BackgroundWorker(const APKey session_id, AppCore* ap_core, MainController *control,
                              void_callback cb=NULL, void_callback cb_ok= NULL, QObject *parent = 0);

    ~BackgroundWorker();

    void SetCallback(ICallback* cb) { current_cb = cb; }

    void AddJob(const QSharedPointer<ISetting> &job);

    void ClearJobQueue(){ job_settings.clear(); }

    void SetConnSetting(const QSharedPointer<APCore::ConnSetting> &setting);

    void SetPlatformSetting(const QSharedPointer<APCore::PlatformSetting> &setting);

    bool is_stopping() const { return stop_flag == AP_STOP; }

protected:
    virtual void run();

private:
    const APKey session_id_;
    AppCore* ap_core_;

    ICallback *current_cb;

    std::list<QSharedPointer<ISetting> > job_settings;

    QSharedPointer<APCore::ConnSetting> conn_setting_;

    QSharedPointer<APCore::PlatformSetting> platform_setting_;

    int stop_flag;

    void_callback finished_cb;
    void_callback ok_cb;

    MainController *control_;

signals:
    void signal_err_msg(int err_code, const std::string& err_msg);
    void signal_finish();

public slots:
    void slot_stop_executing();

};

#endif // BACKGROUNDWORKER_H
