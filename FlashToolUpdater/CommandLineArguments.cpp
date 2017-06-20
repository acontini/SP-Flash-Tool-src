#include "CommandLineArguments.h"

#include <iostream>
#include <stdlib.h>

using std::string;

const string TOOL_USAGE (
    "\nUsage:FlashToolUpdater...\n"
    "Run Flash tool updater.\n"
    "Example: FlashToolUpdater -i updater.ini -v 1.1312.05\n"
    "\nMandatory:\n"
    "-i flashtool updater file path\n"
    "-v current version\n"
    "\nMiscellaneous:\n"
    "-h display help and exit\n"
);

CommandLineArguments::CommandLineArguments(int argc, char **argv):
    update_iniFile_(),
    current_version_()
{
    Parse(argc, argv);
}

void CommandLineArguments::Parse(int argc, char **argv)
{
    int opt;

#ifndef _WIN32
    while(1)
    {
        opt = getopt(argc, argv, "i:v:h");

        if(opt == -1)
            break;

        switch(opt)
        {
        case 'i':
            update_iniFile_ = optarg;
            break;

        case 'v':
            current_version_ = optarg;

            break;

        case 'h':
            std::cout << TOOL_USAGE;
            exit(0);

        default:
            std::cout << TOOL_USAGE;
            exit(-1);
        }
    }
#endif

    if(update_iniFile_.length() == 0 || current_version_.length() == 0)
    {
        std::cout << "Invalid parameter." << std::endl;
        std::cout << TOOL_USAGE;
        exit(-1);
    }
}
