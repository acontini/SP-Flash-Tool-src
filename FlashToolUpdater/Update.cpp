#include "Update.h"

#include <algorithm>
#include <list>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QTextStream>

#include "FileOps.h"

Update::Update(const QString& update_iniFile, const QString& current_version, const QString& app_directory):
    update_iniFile_(update_iniFile),
    current_version_(current_version),
    app_directory_(app_directory)
{
    UserTempDir = QDir::currentPath();

    if(UserTempDir.length() <= 0)
    {
        std::cout << "Create temp directory failed.\n";
        exit(1);
    }

    TempZipName = UserTempDir + QDir::separator() + "temp.zip";
    TempExtractDir = UserTempDir + QDir::separator() + "temp_zip";

    parserUpdateInfo();
}

void Update::GetAllVersions(std::vector<std::string> &versions)
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

void Update::parserUpdateInfo()
{
    QSettings settings(update_iniFile_, QSettings::IniFormat);

    settings.beginGroup("Exe Info");

    ReleaseFolder = settings.value("path").toString().toStdString();
    UpdaterExe = settings.value("name").toString().toStdString();
    ReleaseNote = settings.value("releaseNote").toString().toStdString();

    settings.endGroup();
}

void Update::ReadReleaseNotes(std::string path)
{
    QFile file(QString(path.c_str()));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        releaseNotes += line + "\n";
    }

    std::cout<< ("%s\n", releaseNotes.toStdString().c_str());
}

bool Update::IsUpdatePackage(const std::string &file_name,
                                   const std::string &/*pattern*/)
{
    QSettings settings(ABS_PATH_C("update.ini"), QSettings::IniFormat);

    settings.beginGroup("Exe Info");

    QString fileName = settings.value("ToolName").toString();

    settings.endGroup();

    return file_name.find(fileName.toStdString()) != std::string::npos
            && file_name.find(".zip") != std::string::npos;
}

bool Update::CheckNewVersion()
{
    std::vector<std::string> allVersions;

    GetAllVersions(allVersions);

    if(allVersions.empty())
        return false;

    std::sort(allVersions.begin(), allVersions.end());

    std::string latestVer = allVersions.back();

    if(latestVer.compare(current_version_.toStdString()) > 0)
    {
        std::list<std::string> files;

        FindFile(ReleaseFolder + "/" + latestVer,
                 files, "", IsUpdatePackage);

        if(!files.empty())
        {
            std::cout<<("There is new version: %s\n", latestVer.c_str());

            newVersion = latestVer;

            newVersionFolder = ReleaseFolder + QDir::separator().toAscii() + latestVer;
            newVersionPackage = newVersionFolder + QDir::separator().toAscii() + files.front();

            ReadReleaseNotes(newVersionFolder + QDir::separator().toAscii() + ReleaseNote);

            InitUpdaterPackageInfo(newVersionPackage.c_str());

            return true;
        }
    }

    return false;
}

void Update::InitUpdaterPackageInfo(const QString &package)
{
    UpdateZipName = package;

    int pos = package.lastIndexOf(QDir::separator()) + 1;

    InsideZipFolderName = UpdateZipName.mid(pos, UpdateZipName.lastIndexOf(".zip") - pos);

    TempBinariesFolder = TempExtractDir + QDir::separator() + InsideZipFolderName;
}

bool Update::InitFolders()
{
    ClearFolders();

    return MakeDir(TempExtractDir);
}

void Update::ClearFolders()
{
    DeleteDir(TempBinariesFolder);
    DeleteDir(TempExtractDir);
}

void Update::UpdateTool()
{
    std::cout<< ("Initializing temp folder...\n");

    if(!InitFolders())
    {
        std::cout << ("InitFolders failed.\n");
        exit(1);
    }

    std::cout << ("Downloading update package...\n");

    if(FileCopy(UpdateZipName, TempZipName, true) == false)
    {
        std::cout << ("Downloading zip file failed.\n");
        exit(1);
    }

    std::cout << ("Unzip update package...\n");

    if(UnzipArchive(TempZipName, TempExtractDir, InsideZipFolderName) == false)
    {
        QFile::remove(TempZipName);

        std::cout << ("Unzip package failed.\n");

        exit(1);
    }

    std::cout << ("Updating Flash Tool binaries...\n");

    if(DirectoryCopy(QDir::toNativeSeparators(TempBinariesFolder), app_directory_, "", true) == false)
    {
        QFile::remove(TempZipName);

        std::cout << ("Updating binaries failed.\n");

        exit(1);
    }

    std::cout << ("Clearing temp folders...\n");

    ClearFolders();

    QFile::remove(TempZipName);
}
