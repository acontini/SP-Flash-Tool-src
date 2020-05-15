#ifndef GENERALSETTING_H
#define GENERALSETTING_H

#include <QSharedPointer>

#include <iostream>
#include <list>
#include "../Arg/GeneralArg.h"
#include "../BootRom/flashtool.h"
#include "../Cmd/GeneralCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../XMLParser/XMLSerializable.h"
#include "../Setting/ConnSetting.h"
#include "../Setting/ChksumSetting.h"
#include "CommandLineArguments.h"

namespace ConsoleMode
{

class GeneralSetting : public XML::Serializable
{
public:
    GeneralSetting();
    ~GeneralSetting();

    GeneralSetting(const GeneralSetting& setting);

    GeneralSetting(const XML::Node &node);

    void vSetArgs(const CommandLineArguments& args);

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    GeneralSetting *pclClone() const
    {
        return new GeneralSetting(*this);
    }

    QSharedPointer<APCore::ConnSetting> pclGetConnSetting() const
    {
        return m_pclConnSetting;
    }

    void vSetConnSetting(const QSharedPointer<APCore::ConnSetting> setting)
    {
        m_pclConnSetting = setting;
    }

    QSharedPointer<APCore::ChksumSetting> pclGetChksumSetting() const
    {
        return m_pclChksumSetting;
    }

    void SetChksumSetting(const QSharedPointer<APCore::ChksumSetting> setting)
    {
        m_pclChksumSetting = setting;
    }

    QSharedPointer<GeneralArg> pclGetGeneralArg() const
    {
        return m_pclArg;
    }

    void vSetGeneralArg(const QSharedPointer<GeneralArg> arg)
    {
        m_pclArg = arg;
    }

    QSharedPointer<GeneralCommand> pclCreateCommand(const QSharedPointer<AppCore>& app, const APKey& key);
private:
    GeneralSetting& operator=(const GeneralSetting&);
    QSharedPointer<GeneralArg> m_pclArg;
    QSharedPointer<APCore::ConnSetting> m_pclConnSetting;
    QSharedPointer<APCore::ChksumSetting> m_pclChksumSetting;
};

}

#endif // GENERALSETTING_H
