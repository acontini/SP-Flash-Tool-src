#ifndef COMMANDLINEARGUMENTS_H
#define COMMANDLINEARGUMENTS_H

#include <string>

class CommandLineArguments
{
public:
    explicit CommandLineArguments(int argc, char ** argv);

    std::string GetUpdaterIniFile() const { return update_iniFile_;}

    std::string GetCurrentVersion() const { return current_version_;}

private:
    void Parse(int argc, char ** argv);

private:
    std::string update_iniFile_;
    std::string current_version_;
};

#endif // COMMANDLINEARGUMENTS_H
