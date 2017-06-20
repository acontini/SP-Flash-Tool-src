#ifndef CONSOLEMODEENTRY_H
#define CONSOLEMODEENTRY_H

#include "../Public/AppCore.h"

namespace ConsoleMode
{

class ConsoleModeEntry
{
public:
    ConsoleModeEntry();
    ~ConsoleModeEntry();

    int Run(int argc, char *argv[]);

private:
    void CheckNewVersion();
};

}

#endif // CONSOLEMODEENTRY_H
