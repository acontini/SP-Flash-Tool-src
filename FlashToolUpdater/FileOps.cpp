#include "FileOps.h"
#include <QDir>
#include "./quazip/quazip.h"
#include "./quazip/quazipfile.h"
#include <iostream>

extern std::string g_app_path;

std::string AbsolutePath(const std::string &name)
{
    return g_app_path + C_SEP_STR + name;
}

bool MakeDir(const QString& dir)
{
    return QDir().mkdir(dir);
}

bool DeleteDir(const QString &dir)
{
    bool result = false;

    if(NULL == dir)
        return result;

    QDir fileDir(dir);

    if(!fileDir.exists())
        return result;

    Q_FOREACH(QFileInfo info,
              fileDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System |
                                                    QDir::Hidden | QDir::AllDirs | QDir::Files,
                                                    QDir::DirsFirst))
    {
        if(info.isDir())
        {
            result = DeleteDir(info.absoluteFilePath());
        }
        else
        {
            result = QFile::remove(info.absoluteFilePath());
        }

        if(!result)
            return result;
    }

    result = fileDir.rmdir(dir);

    return result;
}

bool FileCopy(const QString& srcFile, const QString& desFile, const bool& override)
{
    QString destionationFile = desFile;

    QFileInfo sourceInfo(srcFile), destInfo(desFile);

    if(!sourceInfo.exists() || !sourceInfo.isFile())
        return false;

    if(destInfo.isDir())
        destionationFile = (QFileInfo(QDir(destInfo.absoluteFilePath()), sourceInfo.fileName())).absoluteFilePath();

    if(!override && QFile::exists(destionationFile))
        return false;

    if(QFile::copy(srcFile, destionationFile))
    {
        QFile::setPermissions(destionationFile, QFile::ReadOwner | QFile::WriteOwner);
        return true;
    }

    return false;
}

bool DirectoryCopy(const QString &srcDir, const QString &desDir, const QString &except, const bool override)
{
    QDir directory(srcDir);
    bool result= false;

    if(!directory.exists())
        return result;

    QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

    QList<QString>::iterator iter = files.begin();

    for(; iter != files.end(); ++iter)
    {
        QString filePath = QDir::convertSeparators(directory.path() + '/' + (*iter));
        QString destPath = desDir + "/" + directory.relativeFilePath(filePath);
        QFileInfo info(filePath);

        if(info.isFile() || info.isSymLink())
        {
            bool result;

            if(override)
            {
               result = QFile::remove(destPath);
            }

            result = QFile::copy(filePath, destPath);

            if(result)
            {
                QFile::setPermissions(destPath, QFile::WriteOwner | QFile::ReadOwner | QFile::ExeOwner);
            }
        }
        else if(info.isDir())
        {
            QDir subDir(destPath);
            subDir.mkpath(destPath);

            if(!DirectoryCopy(filePath, destPath, except, override))
            {
                result = false;
            }
        }
    }

    return true;
}

bool UnzipArchive(const QString &zipFile, const QString &destPath, const QString &insideZipName)
{
    QString cwd = QDir::currentPath();

    if(cwd.length() <= 0)
        return false;

    if(!QDir::setCurrent(destPath))
    {
        std::cout << "Set current working directory failed.\n";
        return false;
    }

   QuaZip zip(zipFile);

   if(!zip.open(QuaZip::mdUnzip))
   {
       qWarning("zip open failed: %d", zip.getZipError());
       return false;
   }

   qWarning("%d entries.\n", zip.getEntriesCount());
   qWarning("Global comment: %s\n", zip.getComment().toLocal8Bit().constData());

   QuaZipFileInfo info;
   QuaZipFile file(&zip);

   QFile out;
   QString name;

   for(bool more = zip.goToFirstFile(); more; more = zip.goToNextFile())
   {
       if(!zip.getCurrentFileInfo(&info))
       {
           qWarning("get current file information failed:%d.\n", zip.getZipError());
           return false;
       }

       if(!file.open(QIODevice::ReadOnly))
       {
           qWarning("file open():%d.\n", file.getZipError());
           return false;
       }

       QString temp = file.getActualFileName();

      int pos = temp.lastIndexOf("/") + 1;

      QString directory;

      if(pos == 0)  // file
          directory = destPath + QDir::separator() + insideZipName;
      else
          directory = destPath + QDir::separator() + insideZipName + QDir::separator() + temp.left(pos - 1);

      QDir dir(directory);
      if(!dir.exists(directory))
      {
          MakeDir(directory);
      }

      name = directory + "/" + temp.right(temp.length() - pos);

       if(file.getZipError() != UNZ_OK)
       {
           qWarning("file.getFileName:%d.\n", file.getZipError());
           return false;
       }

       out.setFileName(name);
       QString error = out.errorString();

       out.open(QIODevice::WriteOnly);
       error = out.errorString();

       for(qint64 pos = 0, len = file.size(); pos < len;)
       {
           char buf[4096];
           qint64 readSize = qMin(static_cast<qint64>(4096), len - pos);
           qint64 l;

           if((l = file.read(buf, readSize)) != readSize)
           {
               out.write(buf, l);
           }

           out.write(buf, readSize);

           pos += readSize;
       }

       out.close();

       file.close();

       QFile::setPermissions(name, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);

       if(file.getZipError() != UNZ_OK)
       {
           qWarning("file.close():%d\n", file.getZipError());
           return false;
       }
   }

   zip.close();

   if(zip.getZipError() != UNZ_OK)
   {
       qWarning("file.close():%d\n", zip.getZipError());
       return false;
   }

   QDir::setCurrent(cwd);

   return true;
}

bool FindFile(const std::string& path, std::list<std::string> &files,
                         const std::string& pattern, COMPARE_METHOD compare_m, bool recursive)
{
    QDir dir(path.c_str());

    if(!dir.exists())
        return false;

    dir.setFilter(QDir::Dirs | QDir::Files);

    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    bool isDirectory;
    std::string fileName;

    if(list.empty())
        return false;

    do
    {
        QFileInfo fileInfo = list.at(i);

        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            i++;
            continue;
        }

        isDirectory = fileInfo.isDir();

        if(isDirectory && recursive)
        {
            FindFile(fileInfo.filePath().toStdString(), files, pattern, compare_m, recursive);
        }
        else
        {
            fileName = fileInfo.fileName().toStdString();

            if(compare_m == NULL)
            {
                if(pattern == "")
                {
                    files.push_back(fileName);
                }
                else if(pattern == fileName)
                {
                    files.push_back(fileName);
                }
            }
            else
            {
                if(compare_m(fileName, pattern))
                {
                    files.push_back(fileName);
                }
            }
        }

        i++;
    }while(i < list.size());

    return true;
}

