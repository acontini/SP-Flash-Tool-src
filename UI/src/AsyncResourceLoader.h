#ifndef ASYNCRESOURCELOADER_H
#define ASYNCRESOURCELOADER_H

#include <QObject>
#include <QString>
#include <QtCore/QThread>
#include <QSharedPointer>
#include "../../Public/Host.h"
#include "../../Setting/ChksumSetting.h"

class AppCore;
class ICallback;

class AsyncResourceLoader : public QObject
{
    Q_OBJECT
public:
    explicit AsyncResourceLoader(const APKey session_id, AppCore* ap_core, QObject *parent = 0);
    ~AsyncResourceLoader();

    void LoadAsyncByScatter(const QString& file_name,
                            ICallback* cb, ICallback *failed_callback,ICallback *cancel_cb);

    void LoadSCIScatterByAsync(const QString& file_name,
                               ICallback* cb, ICallback *failed_callback);

    void LoadAsyncROMFile(const QString& file_name, unsigned int index,
                          ICallback* cb, ICallback *failed_callback);

    void PopError(const std::string& err);

    void SetChksumSetting(const QSharedPointer<APCore::ChksumSetting> &chksumSetting);

    void SetStopFlag(int *stop_flag);
private:


    const APKey session_id_;
    AppCore* ap_core_;

signals:
    void signal_err_msg(int err_code, const std::string& err_msg);
    void signal_finish();
public slots:
};


#endif // ASYNCRESOURCELOADER_H
