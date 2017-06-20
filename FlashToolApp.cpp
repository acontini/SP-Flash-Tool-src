#include "FlashToolApp.h"
#include "Err/Exception.h"
#include "Logger/Log.h"
#include <iostream>

FlashToolApp::FlashToolApp(int &argc, char **argv) :
    QApplication(argc, argv)
{
}

bool FlashToolApp::notify(QObject *receiver, QEvent *e)
{
    bool result = false;
    try
    {
        result = QApplication::notify(receiver,e);
    }
    catch(const BaseException &base_exp)
    {
        std::string msg(base_exp.err_msg());
        msg.append("\n");
        msg.append(base_exp.context());

        LOG("Uncaught BaseException: %s", msg.c_str());

        QApplication::exit(1);

    }
    catch(const std::exception &std_exp)
    {
        LOG("Uncaught std::exception: %s", std_exp.what());

        QApplication::exit(2);
    }

    return result;
}
