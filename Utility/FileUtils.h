#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <iostream>
#include <list>
#include <stdio.h>
#include <string>
#include "../BootRom/flashtool_handle.h"

enum E_CHECK_DIR_STATUS{
    DIR_EXIST,
    DIR_NOT_EXIST,
    CREATE_DIR_SUCCESS,
    CREATE_DIR_FAIL,
    DELETE_DIR_FAIL
};

namespace FileUtils {

    //Common Directory OPs
    E_CHECK_DIR_STATUS CheckDirectory(const std::string &dir, bool createIfNotExist = true);
    bool CreateFiles(std::string fileName);
    bool IsDirectoryExist(const std::string &dir);

    bool IsFileExist(const std::string &file);
    bool QIsFileExist(const std::string &file);

    bool DeleteDirectory(const std::string &dir);
    bool QDeleteDirectory(const std::string &dir);
    bool QDeleteFile(const std::string &file);

    //Application related OPs
    static std::string g_app_path;
    const std::string &GetAppDirectory();
    void UpdateAppPath(const std::string &app_path);

    //make absolute path relative to tool dir
    std::string AbsolutePath(const char *name);
    std::string AbsolutePath(const std::string &name);

#define ABS_PATH        FileUtils::AbsolutePath
#define ABS_PATH_C(x)   ABS_PATH(x).c_str()

#if 0   //deprecated function
    //prepare backup NVRAM folder.
    E_CHECK_DIR_STATUS CheckDirByCOMPort(int port, std::string &dir,
        bool createIfNotExist = true);

    E_CHECK_DIR_STATUS CheckDirByChipID(unsigned char* chipId, unsigned int length, std::string &dir,
        bool createIfNotExist = true);
#endif

    //Declaration for nvram backup folder
    extern const std::string kBackupFolder;

    //Check the backup directory, if it does not exsit, create it
    bool MakeParentBackupDir(const std::string& folder = kBackupFolder);

    std::string ComposeDirNameByChipID(const unsigned char* chipId,
                                       const unsigned int length,
                                       const std::string& folder = kBackupFolder,
                                       bool createIfNotExist = true);

    //File Search OPs
    typedef bool (*COMPARE_METHOD)(const std::string &file_name, const std::string &pattern);

    bool FindFile(const std::string& path, std::list<std::string> &files, const std::string& pattern,
                  COMPARE_METHOD compare_m = NULL, bool recursive = false);

    unsigned int GetFileSize(const std::string &filename);
    unsigned char* GetFileContent(const std::string &filename);

    void DebugLogsOn(void);
    void DebugLogsOff(void);

    //file checksum
    bool ComputeCloneCheckSum(const std::string &src_file, RomMemChecksumArg  &chksum_arg);
    bool ComputeCheckSum(const std::string &src_file, unsigned char &chksum);
    bool LoadCheckSum(const std::string &chksum_file, unsigned char &chksum);
    bool SaveCheckSum(const std::string &chksum_file, const unsigned char chksum);

    //load file
    int LoadFile(const char *pathname, unsigned char **pp_buf, unsigned int *p_nFileLength, long *p_last_modify_time);
    int LoadFileInfo(const char *pathname, unsigned int *p_nFileLength, long* p_last_modify_time);

      //save file
    int SaveFile(const char*filepath, unsigned char*p_buf, unsigned int file_len);

    class FileOpenSentry {
    public:
        FileOpenSentry(const std::string &filepath, const std::string &mode, FILE *&fp)
                : m_fp(fp) {
            m_fp = NULL;
            if (filepath.length() == 0 || mode.length() == 0)
                return;
            m_fp = fopen(filepath.c_str(), mode.c_str());
        }
        ~FileOpenSentry() {
            if (NULL != m_fp) {
                fclose(m_fp);
                m_fp = NULL;
            }
        }
    private:
        FILE *&m_fp;
    };

};

#endif // FILEUTILS_H
