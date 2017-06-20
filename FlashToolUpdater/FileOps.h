#ifndef FILEOPS_H
#define FILEOPS_H

#include <QString>
#include <list>
#include <string>

#define C_SEP_CHR   '\\'
#define C_SEP_STR   "\\"

#define ABS_PATH        AbsolutePath
#define ABS_PATH_C(x)   ABS_PATH(x).c_str()

typedef bool (*COMPARE_METHOD)(const std::string &file_name,
                               const std::string &pattern);

std::string AbsolutePath(const std::string &name);

bool MakeDir(const QString& dir);

bool DeleteDir(const QString &dir);

bool FileCopy(const QString& srcFile, const QString& desFile,
              const bool &override);

bool DirectoryCopy(const QString &srcDir, const QString &desDir,
                   const QString &except, const bool override = true);

bool UnzipArchive(const QString &zipFile, const QString &destPath,
                  const QString &insideZipName);

bool FindFile(const std::string& path, std::list<std::string> &files,
                         const std::string& pattern,
              COMPARE_METHOD compare_m = NULL,
              bool recursive = false);

#endif // FILEOPS_H
