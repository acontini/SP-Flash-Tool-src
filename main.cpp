#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QString>
#include <iostream>
#include <QDir>
#include <QMessageBox>

#include "UI/src/MainWindow.h"
#include "ConsoleMode/ConsoleModeEntry.h"
#include "Err/Exception.h"
#include "FlashToolApp.h"
#include "Logger/Log.h"
#include "Utility/FileUtils.h"
#include "Utility/IniItem.h"
#include "Utility/Utils.h"
#include "Utility/version.h"
#include "Host/Inc/RuntimeMemory.h"

#include "TimeStamp/TempPlcCtrl.h"

static void init_app_path(char* argv[])
{
    QDir dir = QFileInfo(argv[0]).absoluteDir();
    QString app_path = dir.absolutePath();
    FileUtils::UpdateAppPath(app_path.toStdString());
}

static int runQtApplication(int argc, char *argv[])
{
    FlashToolApp a(argc, argv);
    if(ToolInfo::IsTempVer() == true)
    {
        TempPlcCtrl plcCtrl;
        if (plcCtrl.isExpire() == true)
        {
            QMessageBox message(QMessageBox::Critical,"Smart Phone Flash Tool","The temp Flash Tool has expired.",QMessageBox::Ok,NULL);
            message.exec();

            return 4;
        }
    }

    QPixmap pixmap(":/images/FlashToolSplash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("SP Flash Tool is loading...\n\n\n\n\n\n\n\n\n\n\n\n\n"
                       "   Copyright (c) 2001-2017, MediaTek Inc. All rights reserved."
                       ,Qt::AlignBottom,Qt::gray);

    MainWindow w;
    IniItem item("option.ini", "Language", "CurrentLang");
    int index =item.GetIntValue();
    w.ChangeLanguage(index);
    w.LoadDAFile();
    w.LoadScatterFile();
    w.LoadLastAuthFile();
    w.show();

    splash.finish(&w);
    return a.exec();
}

int runConsoleMode(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ConsoleMode::ConsoleModeEntry cm_entry;
    return cm_entry.Run(argc, argv);
}

int main(int argc, char *argv[])
{
    try
    {
        EnableMemLeakCheck();
        Utils::SetTextCodec();
        init_app_path(argv);

        if(argc>1)
        {
            if(strcmp(argv[1], "UT") == 0)
            {
                return 0;
            }
            else
            {
                return runConsoleMode(argc,argv);
            }
        }
        else
        {
            //GUI
            return runQtApplication(argc, argv);
        }
    }
    catch(const BaseException &base_exp)
    {
        std::string msg(base_exp.err_msg());
        msg.append("\n");
        msg.append(base_exp.context());

        LOGE("Uncaught BaseException in main(): %s", msg.c_str());
        return 1;
    }
    catch(const std::exception &std_exp)
    {
        LOGE("Uncaught std::exception in main(): %s", std_exp.what());
        return 2;
    }
    catch(...)
    {
        LOGE("Uncaught std::exception #$$#$ in main()");
        return 3;
    }
}
