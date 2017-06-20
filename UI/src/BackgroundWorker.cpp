#include "BackgroundWorker.h"
#include "../../Conn/Connection.h"
#include "../../Public/Host.h"

#include "../../Logger/Log.h"
#include "../../Err/Exception.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "MainController.h"
#include "connob.h"

BackgroundWorker::BackgroundWorker(const APKey session_id, AppCore* ap_core, MainController *control,
                                   void_callback cb, void_callback cb_ok, QObject *parent) :
    QThread(parent),session_id_(session_id), ap_core_(ap_core),
    current_cb(NULL),conn_setting_(NULL),
    platform_setting_(new APCore::PlatformSetting), stop_flag(AP_STOP),finished_cb(cb), ok_cb(cb_ok), control_(control)
{
    LOG("ctor of BackgroundWorker...");
}

BackgroundWorker::~BackgroundWorker()
{
    LOG("dtor of BackgroundWorker...");
    this->terminate();
    this->wait();
/*
    if (conn_setting_ != NULL)
    {
        delete conn_setting_;
    }*/
    if (current_cb != NULL)
    {
        delete current_cb;
    }
}

void BackgroundWorker::SetConnSetting(const QSharedPointer<APCore::ConnSetting> &setting)
{
    conn_setting_ = setting;
    conn_setting_->set_stop_flag(&stop_flag);
}

void BackgroundWorker::SetPlatformSetting(const QSharedPointer<APCore::PlatformSetting> &setting)
{
    platform_setting_ = setting;
}

void BackgroundWorker::AddJob(const QSharedPointer<ISetting> &job)
{
    job->set_stop_flag(&stop_flag);
    job_settings.push_back(job);
}

void BackgroundWorker::run()
{
    stop_flag = 0;
    bool isOK = true;
    try
    {
        HW_StorageType_E storage_type =
                this->platform_setting_->getFlashToolStorageConfig().GetStorageType();

        QSharedPointer<APCore::Connection> conn(
                    conn_setting_->CreateConnection(
                        session_id_,
                        storage_type,
                        platform_setting_->supportPowerKeyReset()
                        ));

        QSharedPointer<APCore::ConnOb> observer
                = QSharedPointer<APCore::ConnOb>(new APCore::ConnOb(control_));

        conn->AttachObserver(observer);

        for(std::list<QSharedPointer<ISetting> >::const_iterator it = job_settings.begin();
            it != job_settings.end(); ++it)
        {
            QSharedPointer<APCore::ICommand> cmd = (*it)->CreateCommand(session_id_);
            cmd->exec(conn);
        }
    }
    catch(const UserStopException& /*user_stop*/)
    {
        LOGI("User stopped.");
        isOK = false;
    }
    catch(const BromException& brom_exp)
    {
        if(is_stopping()/*brom_exp.err_code() == S_STOP*/)
        {
            //user stop
            LOGI("User stopped.");
        }
        else
        {
            LOGE("BROM Exception! (%s)(%s)", brom_exp.err_msg().c_str(),
                 brom_exp.context().c_str());

            emit signal_err_msg(brom_exp.err_code(), brom_exp.err_msg());
        }

        isOK = false;
    }
    catch(const AppException& app_exp)
    {
        LOGE("App Exception! (%s)(%s)", app_exp.err_msg().c_str(),
             app_exp.context().c_str());

        emit signal_err_msg(app_exp.err_code(), app_exp.err_msg());

        isOK = false;
    }
    catch(const BaseException& base_exp)
    {
        LOGE("Base Exception! (%s)(%s)", base_exp.err_msg().c_str(),
             base_exp.context().c_str());

        emit signal_err_msg(-1, base_exp.err_msg());

        isOK = false;
    }

    control_->set_loop_dl_ok(isOK);

    if(current_cb != NULL)
    {
        current_cb->Execute();
    }
    if(finished_cb != NULL)
    {
        finished_cb();
    }

    if(ok_cb != NULL && isOK)
    {
        ok_cb();
    }

    ClearJobQueue();
}


void BackgroundWorker::slot_stop_executing()
{
    stop_flag = AP_STOP;
    LOG("stopping,(0x%08X,%d)",&stop_flag,stop_flag);
}
