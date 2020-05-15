#include "AsyncUpdater.h"
#include <QRunnable>
#include <QThreadPool>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QSettings>
#include <algorithm>
#include <QDebug>
#include <QFuture>
#include <QtConcurrentRun>

#include "../../BootRom/host.h"
#include "../../Utility/version.h"
#include "../../Utility/FileUtils.h"
#include "../../Utility/constString.h"
#include "../../Host/Inc/RuntimeMemory.h"
#include "../../Utility/Utils.h"

#ifdef _WIN32
#define STR_RELEASE_FOLDER  "Windows"
#else
#define STR_RELEASE_FOLDER "Linux"
#endif

#define Tool_Version_key            "Version"
#define New_Feature_key             "New-Features/Enhancements"
#define Bug_Fixed_Key               "Bug-Fixed"
#define Limitation_Key              "Limitations"
#define Limitation_Bringup_SubKey   "Bringup"
#define Limitation_Update_SubKey    "Update"
#define Support_Platform_Key        "Support-Platforms"

class CheckUpdateRunnable : public QRunnable
{
public:
    CheckUpdateRunnable(AsyncUpdater *updater):updater_(updater)
    {

    }

    ~CheckUpdateRunnable()
    {

    }

    virtual void run();

private:
    AsyncUpdater *updater_;
};

void CheckUpdateRunnable::run()
{
    updater_->CheckUpdate();
    updater_->CheckFinish();
}

class UpdateRunnable:public QRunnable
{
public:
    UpdateRunnable(AsyncUpdater *updater):updater_(updater)
    {

    }

    ~UpdateRunnable()
    {

    }

    virtual void run();

    AsyncUpdater *updater_;
};

void UpdateRunnable::run()
{
    updater_->UpdateTool();
}

void AsyncUpdater::parserUpdateInfo()
{
    QSettings settings(ABS_PATH_C("update.ini"), QSettings::IniFormat);

    settings.beginGroup("Exe Info");

    ReleaseFolder = settings.value("path").toString().toStdString();
    UpdaterExe = settings.value("name").toString().toStdString();
    ReleaseNote = settings.value("releaseNote").toString().toStdString();

    settings.endGroup();
}

AsyncUpdater::AsyncUpdater(QObject *parent) :
    QObject(parent)
{
    hasNewVersion_ = false;

    parserUpdateInfo();
}

AsyncUpdater::~AsyncUpdater()
{

}

void AsyncUpdater::AsyncUpdateTool()
{
    QRunnable *task = new UpdateRunnable(this);
    QThreadPool::globalInstance()->start(task);
}

void AsyncUpdater::UpdateTool()
{
    std::string updater = ABS_PATH(UpdaterExe);

    QString extPath = QString(updater.c_str());

    QProcess process;

    QObject::connect(&process, SIGNAL(error(QProcess::ProcessError)),
                     this, SLOT(processError(QProcess::ProcessError)));

    QStringList parameter;

    std::string temp = newVersionPackage;

    parameter << temp.c_str();

    process.startDetached(extPath, parameter);

    QApplication::exit(0);
}

void AsyncUpdater::CheckAsyncNewVersion()
{
    QRunnable *task = new CheckUpdateRunnable(this);

    //QThreadPool::globalInstance()->start(task);
    future = QtConcurrent::run(task, &QRunnable::run);
}

void AsyncUpdater::waitForFinished()
{
    future.waitForFinished();
}

void AsyncUpdater::CheckUpdate()
{
    std::vector<std::string> allVersions;

    GetAllVersions(allVersions);

    if(allVersions.empty())
        return;

    std::sort(allVersions.begin(), allVersions.end());

    std::string latestVer = allVersions.back();

    if(latestVer.compare(ToolInfo::VersionNum()) > 0)
    {
        std::list<std::string> files;

        FileUtils::FindFile(ReleaseFolder + C_SEP_STR + latestVer + C_SEP_STR + STR_RELEASE_FOLDER,
                            files, "", IsUpdatePackage);

        if(!files.empty())
        {
            hasNewVersion_ = true;
            newVersion = latestVer;
            newVersionFolder = ReleaseFolder + C_SEP_STR + latestVer + C_SEP_STR + STR_RELEASE_FOLDER;
            newVersionPackage = newVersionFolder + C_SEP_STR + files.front();

            ReadReleaseNotes(newVersionFolder + C_SEP_STR + ReleaseNote);
        }
    }
}

void AsyncUpdater::ConstructReleaseNotes(QtJson::JsonObject &jsonObject){
    QString tool_version = jsonObject[Tool_Version_key].toString();
    if(!tool_version.isEmpty()){
        releaseNotes = "Tool version: \n    "  + tool_version + "\n\n";
    }

    QtJson::JsonArray new_featureList = jsonObject[New_Feature_key].toList();

    if (new_featureList.count() > 0){
        releaseNotes += New_Feature_key;
        releaseNotes += "\n";

        foreach(QVariant feature, new_featureList){
            releaseNotes += "   " + feature.toString() + "\n\n";
        }
    }

    QtJson::JsonArray bug_fixedList = jsonObject[Bug_Fixed_Key].toList();

    if(bug_fixedList.count() > 0){
        releaseNotes += Bug_Fixed_Key;
        releaseNotes += "\n";

        foreach(QVariant bug, bug_fixedList){
            releaseNotes += "   " + bug.toString() + "\n\n";
        }
    }
}

void AsyncUpdater::ReadReleaseNotes(const std::string &path)
{
    QtJson::JsonObject jsonObject;

    Utils::parserJSONFile(path.c_str(), jsonObject);

    ConstructReleaseNotes(jsonObject);

    qDebug() <<"Release notes: \n" << releaseNotes;

    std::string curr_releaseFilePath = ABS_PATH_C("Release.json");
    QtJson::JsonObject currJsonObject;

    Utils::parserJSONFile(curr_releaseFilePath.c_str(),currJsonObject);

    QtJson::JsonObject limitations = currJsonObject[Limitation_Key].toMap();

    QString bringup_chip = limitations[Limitation_Bringup_SubKey].toString();

    QtJson::JsonArray platforms = jsonObject[Support_Platform_Key].toList();

    qDebug() << "Support platforms: \n";
    foreach(QVariant platform, platforms){
        qDebug() << " -" << platform.toString();
    }

    if(!bringup_chip.isEmpty() && !platforms.contains(bringup_chip))
        hasNewVersion_ = false;

    bool tempVersion_update = limitations[Limitation_Update_SubKey].toBool();
    if(!tempVersion_update)
        hasNewVersion_ = false;

 #if 0
    QFile file(QString(path.c_str()));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        releaseNotes += line + "\n";
    }
#endif
}

void AsyncUpdater::GetAllVersions(std::vector<std::string> &versions)
{
    std::string path(ReleaseFolder);

    QDir dir(QString(path.c_str()));
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Dirs);

    QFileInfoList list = dir.entryInfoList();
    int i = 0;

    if(list.empty())
        return;

    do
    {
        QFileInfo info = list.at(i);

        versions.push_back(info.fileName().toStdString());

        i++;
    }while(i < list.size());
}

void AsyncUpdater::CheckFinish()
{
    emit signal_check_finish(hasNewVersion_);
}

bool AsyncUpdater::IsUpdatePackage(const std::string &file_name,
                                   const std::string &/*pattern*/)
{
    QSettings settings(ABS_PATH_C("update.ini"), QSettings::IniFormat);

    settings.beginGroup("Exe Info");

    QString fileName = settings.value("ToolName").toString();

    settings.endGroup();

    return file_name.find(fileName.toStdString()) != std::string::npos
            && file_name.find(".zip") != std::string::npos;
}

void AsyncUpdater::processError(QProcess::ProcessError err)
{
    switch(err)
    {
    case QProcess::FailedToStart:
        QMessageBox::information(0, tr("Smart Phone Flash Tool"), tr("Failed to Start Updater.exe!"));
        break;

    case QProcess::Crashed:
        QMessageBox::critical(0, tr("Smart Phone Flash Tool"), tr("Crashed!"));
        break;

    case QProcess::Timedout:
        QMessageBox::information(0, tr("Smart Phone Flash Tool"), tr("Time out!"));
        break;

    default:
        QMessageBox::information(0, tr("Smart Phone Flash Tool"), tr("Unknow Error!"));
        break;
    }
}
