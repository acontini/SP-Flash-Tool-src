#include <QtCore/QCoreApplication>

#include "Update.h"
#include "CommandLineArguments.h"

#include <QDir>
#include <stdio.h>
#include <iostream>


std::string g_app_path;

void init_app_path(char* argv[])
{
    QDir dir = QFileInfo(argv[0]).absoluteDir();
    QString app_path = dir.absolutePath();
    g_app_path = app_path.toStdString();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    init_app_path(argv);

    CommandLineArguments commandLine(argc, argv);

    Update updater(commandLine.GetUpdaterIniFile().c_str(),
                   commandLine.GetCurrentVersion().c_str(),
                   a.applicationDirPath());


    if(updater.CheckNewVersion())
    {
        std::cout << "Do you want to update or not, if yes, please click 'Y' or 'y', otherwise click 'N' or 'n'\n";

        char c;

        std::cin >> c;

      //  std::cout << ("the input character is %c", c);

        if(c == 'Y' || c == 'y')
        {
            updater.UpdateTool();
        }
        else
        {
            std::cout << "Input error.";
        }
    }

  //  return a.exec();
}
