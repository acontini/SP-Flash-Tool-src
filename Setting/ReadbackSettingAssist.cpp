#include "ReadbackSettingAssist.h"
#include <QString>
#include "../BootRom/host.h"
#include "../Utility/FileUtils.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../Logger/Log.h"

namespace APCore
{

std::string APCore::ReadbackSettingAssist::GetDirNameFromPath(const std::string &path)
{
    int pos = path.find_last_of(C_SEP_CHR);
    return path.substr(0, pos);
}


std::string APCore::ReadbackSettingAssist::GetFileNameFromPath(const std::string &path)
{
    int pos = path.find_last_of(C_SEP_CHR);
    return path.substr(pos+1);
}


std::string APCore::ReadbackSettingAssist::GenerateValidPath(const std::string &path)
{
    std::string _path = path;
    if (QString(_path.c_str()).trimmed().isEmpty())
    {
        LOGE("read file name error: readback file name has empty!\n");
        THROW_APP_EXCEPTION(FT_FILE_IS_NOT_EXIST, "readback file name has empty!");
    }
    std::string dirName = GetDirNameFromPath(_path);
    E_CHECK_DIR_STATUS check_dir_status = FileUtils::CheckDirectory(dirName, true);
    if (check_dir_status == CREATE_DIR_FAIL)
    {
        std::string fileName = GetFileNameFromPath(_path);
        _path = FileUtils::AbsolutePath(fileName);
    }
    return _path;
}

}
