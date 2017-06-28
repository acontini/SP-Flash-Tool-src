#ifndef __FLASH_TOOL_VERSION
#define __FLASH_TOOL_VERSION

#include <iostream>
class ToolInfo{
public:
    static std::string ToolName();
    static std::string BuildInfo();
    static std::string Comments();
    static std::string LoadPath();
    static std::string ToString();
    static bool IsCustomerVer();
    static bool IsTempVer();
    static std::string VersionNum();
private:
    ToolInfo();
};
#endif
