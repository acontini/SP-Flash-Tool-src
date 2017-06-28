#include "Assistant.h"
#include <QLibraryInfo>
#include <QDir>
#include <QApplication>
#include <QDesktopServices>
#include "MainWindow.h"
#include "../../Utility/FileUtils.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "../../Utility/constString.h"

Assistant::Assistant(QObject *parent) :
    QObject(parent),
    process_(NULL),
    cache_dir_()
{
    main_window_ = static_cast<MainWindow*>(parent);
    this->UpdateCacheDir();
    this->ClearUpEnv();
}

Assistant::~Assistant()
{
    if (process_ && process_->state() == QProcess::Running)
    {
        process_->terminate();
        process_->waitForFinished(3000);
    }

    delete process_;
}

bool Assistant::StartAssistant()
{
    if(!process_)
        process_ = new QProcess();

    if(process_->state() != QProcess::Running)
    {
        QString base =  QString::fromStdString(FileUtils::GetAppDirectory()) + QDir::separator();//QCoreApplication::applicationDirPath() + QDir::separator();
        QString app = base;

#ifdef _WIN32
        app += QLatin1String("assistant.exe");
#else
        QString var = base;
        var.append("lib");
        setenv("LD_LIBRARY_PATH", var.toLocal8Bit().constData(), 1);

        app += QLatin1String("bin/assistant");
#endif

        QString qhc = base.append("flashtool.qhc");
        QStringList args;
        args << QLatin1String("-collectionFile")
             << QLatin1String(qhc.toLocal8Bit().constData())
             << QLatin1String("-enableRemoteControl");

        process_->start(app, args);

        if (!process_->waitForStarted())
        {
            main_window_->ShowHelpContents(0,IDS_STRING_LAUNCH_ASSISTANT_ERROR,"E_APP_START_ASSISTANT.htm");

            return false;
        }
    }

    return true;
}

void Assistant::ShowDocumentation(const QString &page, bool showContent, bool showIndex)
{
    if (!StartAssistant())
        return;

    QByteArray ba;

    if(showContent)
        ba.append("show contents;");
    else if(showIndex)
        ba.append("show index;");

    ba.append("setSource qthelp://com.mediatek.flashtool/doc/");

    ba.append(page.toLocal8Bit());

    ba.append('\0');

    process_->write(ba);
}

inline void Assistant::UpdateCacheDir(void) {
    this->cache_dir_ = QDesktopServices::storageLocation(QDesktopServices::DataLocation)
#ifdef _WIN32
            + QString("\\Mediatek\\FlashTool");
#else
            + QString("Mediatek/FlashTool");
#endif
}

bool Assistant::ClearUpEnv() {
    return FileUtils::QDeleteDirectory(this->cache_dir_.toLocal8Bit().constData());
}
