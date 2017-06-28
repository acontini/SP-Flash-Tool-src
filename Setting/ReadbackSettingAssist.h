#ifndef READBACKSETTINGASSIST_H
#define READBACKSETTINGASSIST_H
#include <iostream>
#include <string>

namespace APCore
{

class ReadbackSettingAssist
{
public:
    static std::string GenerateValidPath(const std::string &path);

private:
    static std::string GetDirNameFromPath(const std::string &path);
    static std::string GetFileNameFromPath(const std::string &path);
};

}

#endif // READBACKSETTINGASSIST_H

