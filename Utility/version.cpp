
#include "version.h"

#include <stdio.h>
#include "../Logger/Log.h"
#include <limits.h>
#include <iostream>
//TODO fix ssream compile bug
#ifdef _WIN32
#include "windows.h"
#undef max
#undef min
#else
#include<unistd.h>
#include <stdlib.h>
#include <memory.h>
#endif
#include <sstream>
#include <errno.h>
#include "../Utility/FileUtils.h"

using std::string;
#if !defined(_MSC_VER)
#define _MAX_PATH PATH_MAX
#endif

#include "../BootRom/host.h"

//************** PLEASE DO NOT MODIFY THE NAMES *******************

static const bool           CUSTOMER_VER     = true;
static const bool           TEMP_VER     = false;
static const bool           DEFAULT_ENABLE_LOGS = true;

static const unsigned int  MAJOR_VER        = 5;
static const unsigned int  MINOR_VER        = 1720;
static const unsigned int  BUILD_NUM        = 00;
static const unsigned int  BUILD_SN         = 000;
//Note: REVISION_NUM is commit hash of the tool, should be HEX(0x...)
static const unsigned int  REVISION_NUM     = 0x379103;
static const char* const   BUILD_DATE       = "2017/05/23 23:39";
static const char* const   COMMENTS         = "N/A";

static const char* const   APP_NAME         = "MediaTek SP Flash Tool";
static const char* const   PROC_NAME        = "flash_tool.exe";

//*****************************************************************

// FIXME need remove this functions, this should be implement in Utils.
template<typename T>
std::string HexToString(T hex) {   //Usage: HexToString ( Hex );
    std::ostringstream ss;
    ss <<"0x"<< std::hex<<hex;
    return ss.str();
}

template<typename T>
T StringToNumber(const std::string &text) {  //Usage: StringToNumber<Type> ( String );
    std::istringstream ss(text);
    T result;
    return ss >> result ? result : 0;
}


bool ToolInfo::IsCustomerVer(){
    return CUSTOMER_VER;
}

bool ToolInfo::IsTempVer(){
    return TEMP_VER;
}

std::string ToolInfo::ToolName() {
    char toolName[256] = { 0 };
    sprintf(toolName, "%s v%d.%d.0%d", APP_NAME, MAJOR_VER, MINOR_VER,
            BUILD_NUM);
    return std::string(toolName);
}

std::string ToolInfo::BuildInfo() {
    return std::string(BUILD_DATE).append(" (")
            .append(HexToString(REVISION_NUM)).append(")");
}

std::string ToolInfo::Comments() {
    return std::string(COMMENTS);
}
// TODO should re-generate GetModuleFileName.
std::string ToolInfo::LoadPath() {

   return FileUtils::GetAppDirectory();
}

std::string ToolInfo::ToString() {
    return std::string(
        "==================================================================================================================================\n")
        .append(ToolName()).append(" runtime trace is ON.\n") .append(
        "Build: ").append(BuildInfo()).append("\n").append(
        "Comment: ").append(COMMENTS).append("\n").append(
            "Load Path: ").append(LoadPath()) .append(
        "\n==================================================================================================================================\n");
}

std::string ToolInfo::VersionNum()
{
    char ver[20] = {0};

    sprintf(ver, "%d.%d.0%d.sn%d", MAJOR_VER, MINOR_VER, BUILD_NUM, BUILD_SN);

    return std::string(ver);
}
