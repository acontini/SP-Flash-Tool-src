#include "AsyncResourceLoader.h"

#include "../../Public/AppCore.h"
#include "ICallback.h"
#include "../../Logger/Log.h"
#include "../../Err/Exception.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "ProcessingDialog.h"
#include <QRunnable>
#include <QThreadPool>

class LoadScatterRunnable : public QRunnable
{
public:
    LoadScatterRunnable(const APKey session_id, AppCore* ap_core,
                        const QString &file, ICallback *cb,
                        ICallback *failed_cb,ICallback *cancel_cb,
                        AsyncResourceLoader *loader)
        : session_id_(session_id), ap_core_(ap_core),
          file_(file), cb_(cb), failed_cb_(failed_cb),cancel_cb_(cancel_cb),
          loader_(loader)
    {
        this->setAutoDelete(true);
        LOG("ctor of LoadScatterRunnable...");
    }

    ~LoadScatterRunnable()
    {
        if (NULL != cb_)
            delete cb_;
        if (NULL != failed_cb_)
            delete failed_cb_;
        if (NULL != cancel_cb_)
            delete cancel_cb_;
        LOG("dtor of LoadScatterRunnable...");
    }

    virtual void run();

private:
    const APKey session_id_;
    AppCore* ap_core_;
    QString file_;
    ICallback *cb_;
    ICallback *failed_cb_;
    ICallback *cancel_cb_;
    AsyncResourceLoader *loader_;
};

void LoadScatterRunnable::run()
{
    try
    {
        ap_core_->LoadScatter(NORMAL_SCATTER, session_id_, file_.toLocal8Bit().constData());
        ap_core_->AutoLoadRomImages(session_id_, file_.toLocal8Bit().constData());
        ap_core_->VerifyImageChksum(session_id_, file_.toLocal8Bit().constData());
    }
    catch(const UserStopException &exp)
    {
        Q_UNUSED(exp);
        LOG("UserStopException");
        cancel_cb_->Execute();

        return;
    }
    catch(const BromException &exp)
    {
        std::string msg(exp.err_msg());
        msg.append("\n").append(exp.context());
        LOG("BromException: %s", msg.c_str());

        loader_->PopError(exp.err_msg());

        if(exp.err_code() == S_DL_LOAD_REGION_IS_OVERLAP
                || exp.err_code() == S_DL_LOAD_REGION_NOT_FOUND)
        {
            cb_->Execute();
        }
        else
        {
            failed_cb_->Execute();
        }

        return;
    }
    catch(const BaseException &exp)
    {
        std::string msg(exp.err_msg());
        msg.append("\n").append(exp.context());
        LOG("BaseException: %s", msg.c_str());

        loader_->PopError(exp.err_msg());

        failed_cb_->Execute();

        return;
    }

    if(cb_ != NULL)
    {
        cb_->Execute();
    }
}

class LoadSCIScatterRunable: public QRunnable
{
public:
    LoadSCIScatterRunable(const APKey session_id, AppCore* ap_core,
                          const QString &file, ICallback *cb,
                          ICallback *failed_cb,
                          AsyncResourceLoader *loader)
          : session_id_(session_id), ap_core_(ap_core),
            file_(file), cb_(cb), failed_cb_(failed_cb),
            loader_(loader)
    {
        this->setAutoDelete(true);
    }

    ~LoadSCIScatterRunable()
    {
        if (NULL != cb_)
            delete cb_;
        if (NULL != failed_cb_)
            delete failed_cb_;

        LOG("dtor of LoadSCIScatterRunable...");
    }

    virtual void run();

private:
    const APKey session_id_;
    AppCore* ap_core_;
    QString file_;
    ICallback *cb_;
    ICallback *failed_cb_;
    AsyncResourceLoader *loader_;
};

void LoadSCIScatterRunable::run()
{
    try
    {
        ap_core_->LoadScatter(SCI_SCATTER, session_id_, file_.toLocal8Bit().constData());
    }
    catch(const BaseException &exp)
    {
        std::string msg(exp.err_msg());

        msg.append("\n").append(exp.context());
        LOG("BaseException: %s", msg.c_str());

        loader_->PopError(exp.err_msg());

        failed_cb_->Execute();

        return;
    }

    if(cb_ != NULL)
    {
        cb_->Execute();
    }
}

class LoadRomRunnable : public QRunnable
{
public:
    LoadRomRunnable(const APKey session_id, AppCore* ap_core,
                        const QString &file, unsigned int index,
                        ICallback *cb,
                        ICallback *failed_cb,
                        AsyncResourceLoader *loader)
        : session_id_(session_id), ap_core_(ap_core),
          file_(file),index_(index), cb_(cb), failed_cb_(failed_cb),
          loader_(loader)
    {
        this->setAutoDelete(true);
        LOG("ctor of LoadRomRunnable...");
    }

    ~LoadRomRunnable()
    {
        LOG("dtor of LoadRomRunnable...");
    }

    virtual void run();

private:
    const APKey session_id_;
    AppCore* ap_core_;
    QString file_;
    unsigned int index_;
    ICallback *cb_;
    ICallback *failed_cb_;
    AsyncResourceLoader *loader_;

};

void LoadRomRunnable::run()
{
    try
    {
        bool result = ap_core_->LoadROM(session_id_, file_.toLocal8Bit().constData(),index_);

        if(false == result)
        {
            failed_cb_->Execute();
            failed_cb_ = NULL;

            return;
        }
    }
    catch(const BaseException &exp)
    {
        std::string msg(exp.err_msg());
        msg.append("\n").append(exp.context());
        LOG("BaseException: %s", msg.c_str());

        loader_->PopError(exp.err_msg());

        failed_cb_->Execute();
        failed_cb_ = NULL;
        return;
    }

    if(cb_ != NULL)
    {
        cb_->Execute();
        cb_ = NULL;
    }
}

AsyncResourceLoader::AsyncResourceLoader(const APKey session_id, AppCore* ap_core, QObject *parent)
    :QObject(parent), session_id_(session_id), ap_core_(ap_core)
{
}

AsyncResourceLoader::~AsyncResourceLoader()
{
}

void AsyncResourceLoader::LoadAsyncByScatter(const QString& file_name,
                                             ICallback* cb,
                                             ICallback *failed_cb,
                                             ICallback *cancel_cb)
{
    QRunnable *task = new LoadScatterRunnable(
                session_id_,ap_core_,file_name,cb,failed_cb, cancel_cb,this);
    QThreadPool::globalInstance()->start(task);
}

void AsyncResourceLoader::LoadSCIScatterByAsync(const QString &file_name,
                                                ICallback *cb,
                                                ICallback *failed_cb)
{
    QRunnable *task = new LoadSCIScatterRunable(
                session_id_,ap_core_,file_name,cb,failed_cb, this);

    QThreadPool::globalInstance()->start(task);
}

void AsyncResourceLoader::LoadAsyncROMFile(const QString& file_name,
                                           unsigned int index,
                                           ICallback* cb,
                                           ICallback *failed_cb)
{
    QRunnable *task = new LoadRomRunnable(
                session_id_,ap_core_,file_name,index,cb,failed_cb, this);
    QThreadPool::globalInstance()->start(task);
}

void AsyncResourceLoader::SetChksumSetting(const QSharedPointer<APCore::ChksumSetting> &chksumSetting)
{
    ap_core_->SetChksumSetting(chksumSetting);
}

void AsyncResourceLoader::SetStopFlag(int *stop_flag)
{
    try{
        ap_core_->SetStopLoadFlag(session_id_,stop_flag);
    }catch(const BromException& e){
        THROW_BROM_EXCEPTION_EX(e);
    }
}

void AsyncResourceLoader::PopError(const std::string& err)
{
    emit signal_err_msg(-1, err);
}
