#include "CommandLineArguments.h"

#ifndef _WIN32
#include <getopt.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <QString>
#include <QStringList>
#include <algorithm>
#include <cctype>
#include "../Utility/FileUtils.h"
#include "../Utility/Utils.h"
#include "../Logger/Log.h"
#include "SchemaValidator.h"
#include "../XMLParser/XMLDocument.h"
#include "../XMLParser/XMLNode.h"

using std::string;

namespace ConsoleMode
{

const string CONSOLE_MODE_USAGE(
        "\nUsage: flash_tool OPTION...\n"
        "Run FlashTool in console mode.\n"
        "Example: flash_tool -i download.xml\n"
        "\nMandatory:\n"
        "  -i    console mode configuration file\n"
        "\nOverwriting settings specified in the configuration file:\n"
        "  -d    download agent file path\n"
        "  -s    scatter file path\n"
        "  -c    flash tool features, just support:\n"
        "            format\n"
        "            download\n"
        "            format-download\n"
        "            firmware-upgrade\n"
        "  -p    specify com port(Windows Only), format is as COM4+COM6 (BROM download), COM5 (preloader download)\n"
        "\nMiscellaneous:\n"
        "  -r    redirect debug log to parent process\n"
        "  -h    display this help and exit\n"
        "-b  specify whether to reboot after action (e.g format, download, efuse write)"
    );

const QString SUPPORTED_CMDS("combo-format,download-only,format-download,firmware-upgrade,cert-download,efuse");

CommandLineArguments::CommandLineArguments()
    :m_szInputFilename(),
     m_szDownloadAgentFilename(),
     m_szScatterFilename(),
     m_szAuthFilename(),
     m_szCertFilename(),
     m_szCommand(),
     m_szPort(),
     m_fgRedirect(false),
     m_fgDisplayHepInfo(false),
     m_onlyOutput(false),
      m_reboot(false)
{
}

const string &CommandLineArguments::GetInputFilename() const
{
    return m_szInputFilename;
}

const string &CommandLineArguments::GetScatterFilename() const
{
    return m_szScatterFilename;
}

const string &CommandLineArguments::GetDownloadAgentFilename() const
{
    return m_szDownloadAgentFilename;
}

const string &CommandLineArguments::GetCommand() const
{
    return m_szCommand;
}

const string &CommandLineArguments::GetAuthFilename() const
{
    return m_szAuthFilename;
}

const string &CommandLineArguments::GetCertFilename() const
{
    return m_szCertFilename;
}

const std::string &CommandLineArguments::GetComPort() const
{
    return m_szPort;
}

const std::string &CommandLineArguments::GetHelpInfo() const
{
    return CONSOLE_MODE_USAGE;
}

bool CommandLineArguments::DoRedirect() const
{
    return m_fgRedirect;
}

bool CommandLineArguments::DisplayHelpInfo() const
{
    return m_fgDisplayHepInfo;
}

bool CommandLineArguments::Parse(int argc, char **argv)
{
    bool result = false;
#ifndef _WIN32
    //int opt;

    while (1)
    {
        int c = getopt(argc, argv, "i:d:c:s:p:rhb");

        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 'i':
                m_szInputFilename = optarg;
                result = true;
                break;

            case 'r':
                m_fgRedirect = true;
                break;

            case 'd':
                m_szDownloadAgentFilename = optarg;
                break;

            case 's':
                m_szScatterFilename = optarg;
                if(!m_szCommand.empty())
                {
                    result = true;
                }
                break;
            case 'c':
                m_szCommand = optarg;
                if(!m_szScatterFilename.empty())
                {
                    result = true;
                }
                break;

            case 'p':
                m_szPort = optarg;
                break;

            case 'h':
                m_fgDisplayHepInfo = true;
                result = true;
                break;

        case 'b':
            m_reboot = true;
            break;

            default:
               return false;
        }
    }

    return  result;

#else
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(tolower(argv[i][1]))
            {
            case 'i':
                if (++i >= argc)
                {
                    return false;
                }
                m_szInputFilename = argv[i];
                result = true;
                break;

            case 'r':
                m_fgRedirect = true;
                break;

            case 'd':
                if (++i >= argc)
                {
                    return false;
                }
                m_szDownloadAgentFilename = argv[i];
                break;

            case 's':
                if (++i >= argc)
                {
                    return false;
                }
                m_szScatterFilename = argv[i];
                if(!m_szCommand.empty())
                {
                    result = true;
                }
                break;
            case 'c':
                if (++i >= argc)
                {
                    return false;
                }
                m_szCommand = argv[i];
                if(!m_szScatterFilename.empty())
                {
                    result = true;
                }
                break;
            case 'p':
                if (++i >= argc)
                {
                    return false;
                }
                m_szPort = argv[i];
                break;

            case 'h':
                m_fgDisplayHepInfo = true;
                result = true;
                break;

            case 'o':
                m_onlyOutput = true;
                break;

            case 'b':
                m_reboot = true;
                break;

            default:
               return false;
            }
        }
    }

    return result;

#endif
}

bool CommandLineArguments::Validate()
{
    // validate configuration file
    if(!m_szInputFilename.empty())
    {
        if (!FileUtils::IsFileExist(m_szInputFilename))
        {
            LOGE("\"%s\": file not found", m_szInputFilename.c_str());
            return false;
        }

        SchemaValidator validator(Utils::GetTextCodec()->toUnicode(ABS_PATH_C("console_mode.xsd")));

        try
        {
            validator.Validate(QString::fromStdString(m_szInputFilename));
        }
        catch(...)
        {
            LOGI("Invalid xml file, please check the input file and try again.");
            return false;
        }


    }

    if(!this->m_szScatterFilename.empty() && !FileUtils::IsFileExist(m_szScatterFilename))
    {
        LOGE("\"%s\": file not found", m_szScatterFilename.c_str());
        return false;
    }

    if(!this->m_szDownloadAgentFilename.empty() && !FileUtils::IsFileExist(m_szDownloadAgentFilename))
    {
        LOGE("\"%s\": file not found", m_szDownloadAgentFilename.c_str());
        return false;
    }

    if(!this->m_szCommand.empty())
    {
        transform(m_szCommand.begin(),m_szCommand.end(),m_szCommand.begin(),tolower);
        if("download" ==m_szCommand)
        {
            m_szCommand = "download-only";
        }
        else if ("format" == m_szCommand)
        {
            m_szCommand = "combo-format";
        }
        QString cmd = QString::fromStdString(m_szCommand);

        QStringList list = SUPPORTED_CMDS.split(",");
        if(!list.contains(cmd))
        {
            LOGE("Command \"%s\":is not support!", m_szCommand.c_str());
            return false;
        }
    }

    if(!this->m_szPort.empty())
    {
        std::string::size_type index = m_szPort.find("+");
        int p2 = -1;
        int p1 = -1;
        if(index != std::string::npos)
        {
            p1 = Utils::GetPortNumFromStr(m_szPort.substr(0,index).c_str());
            if(p1>0)
            {
                p2 = Utils::GetPortNumFromStr(m_szPort.substr(index+1).c_str());
                Q_UNUSED(p2);
            }
        }
        else
        {
            p1 = Utils::GetPortNumFromStr(m_szPort.c_str());
        }
        if (p1 <=0)
        {
            return false;
        }
    }

    if(!this->m_szAuthFilename.empty() && !FileUtils::IsFileExist(m_szAuthFilename))
    {
        LOGE("\"%s\": file not found", m_szAuthFilename.c_str());
        return false;
    }

    if(!this->m_szCertFilename.empty() && !FileUtils::IsFileExist(m_szCertFilename))
    {
        LOGE("\"%s\": file not found", m_szCertFilename.c_str());
        return false;
    }

    return true;
}

}
