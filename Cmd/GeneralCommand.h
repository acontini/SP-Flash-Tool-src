#ifndef GENERALCOMMAND_H
#define GENERALCOMMAND_H

#include "../Arg/GeneralArg.h"
#include "../Public/AppCore.h"
namespace ConsoleMode
{

class GeneralCommand
{
public:
    GeneralCommand(QSharedPointer<GeneralArg> arg, const QSharedPointer<AppCore>& app, const APKey& key);
    bool exec();
private:
    bool LoadScatterFile();
    bool LoadRoms();
    bool LoadDA();
    bool LoadCertFile();
    bool LoadAuthFile();
    QSharedPointer<GeneralArg> m_pclArg;
    QSharedPointer<AppCore> m_pclApp;
    APKey m_clApKey;
};
}
#endif // MACROCOMMAND_H
