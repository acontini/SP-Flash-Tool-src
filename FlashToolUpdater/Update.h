#ifndef UPDATE_H
#define UPDATE_H

#include <vector>
#include <QString>

class Update
{
public:
    Update(const QString& update_iniFile, const QString& current_version, const QString& app_directory);

    bool CheckNewVersion();

    void UpdateTool();

    static bool IsUpdatePackage(const std::string &file_name, const std::string &pattern);

private:
    void GetAllVersions(std::vector<std::string> &versions);

    void parserUpdateInfo();

    void ReadReleaseNotes(std::string path);

    void InitUpdaterPackageInfo(const QString& package);
    bool InitFolders();
    void ClearFolders();

private:
    std::string ReleaseFolder;
    std::string UpdaterExe;
    std::string ReleaseNote;

    std::string newVersion;
    std::string newVersionFolder;
    std::string newVersionPackage;
    QString releaseNotes;

    QString update_iniFile_;
    QString current_version_;
    QString app_directory_;

    QString UpdateZipName;
    QString UserTempDir;
    QString TempZipName;
    QString TempExtractDir;
    QString InsideZipFolderName;
    QString TempBinariesFolder;
};

#endif // UPDATE_H
