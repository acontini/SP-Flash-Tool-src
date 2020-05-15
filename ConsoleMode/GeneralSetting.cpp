#include "GeneralSetting.h"
#include "../Conn/Connection.h"
#include "../Utility/FileUtils.h"
#include "../Setting/ConnBromUSBSetting.h"
#include <QtGlobal>
#include <QString>
#include <QDir>

namespace ConsoleMode
{

GeneralSetting::GeneralSetting()
{
    this->m_pclArg = QSharedPointer<GeneralArg> ( new GeneralArg() );
    m_pclConnSetting = QSharedPointer<APCore::ConnSetting> (
              new APCore::ConnBromUSBSetting(
                  APCore::USBHighSpeed,
                  APCore::WithoutBattery));
    m_pclConnSetting->set_timeout(120000);
    m_pclChksumSetting = QSharedPointer<APCore::ChksumSetting>(new APCore::ChksumSetting());
    m_pclChksumSetting->set_chksum_level(0);
}

GeneralSetting::~GeneralSetting()
{
}

GeneralSetting::GeneralSetting(const XML::Node &node)
    :m_pclConnSetting(NULL),
      m_pclChksumSetting(NULL)
{
    this->m_pclArg = QSharedPointer<GeneralArg> ( new GeneralArg() );
    LoadXML(node);
}

void GeneralSetting::vSetArgs(const CommandLineArguments& args)
{

    std::string scatter = args.GetScatterFilename();
    std::string da = args.GetDownloadAgentFilename();
    std::string auth = args.GetAuthFilename();
    std::string cert = args.GetCertFilename();
    std::string port = args.GetComPort();

    if(!scatter.empty())
    {
        m_pclArg->scatter_file = scatter;
    }

    if(!da.empty())
    {
        m_pclArg->da_file = args.GetDownloadAgentFilename();
    }
    else if(m_pclArg->da_file.empty())
    {
        m_pclArg->da_file = ABS_PATH_C("MTK_AllInOne_DA.bin");
    }

    if(!auth.empty())
    {
        m_pclArg->auth_file = args.GetAuthFilename();
    }

    if(!cert.empty())
    {
        m_pclArg->cert_file = args.GetCertFilename();
    }

    if(!port.empty())
    {
        m_pclConnSetting->set_com_port(args.GetComPort());
    }

}

GeneralSetting::GeneralSetting(const GeneralSetting& setting)
{
    this->m_pclArg = QSharedPointer<GeneralArg> ( setting.pclGetGeneralArg()->Clone() );
    this->m_pclConnSetting = setting.pclGetConnSetting();
    this->m_pclChksumSetting = setting.pclGetChksumSetting();
}

void GeneralSetting::LoadXML(const XML::Node &node)
{
    QString path;
    XML::Node child = node.GetFirstChildNode();

    while(!child.IsEmpty())
    {
        const std::string name = child.GetName();
        const std::string text = child.GetText();//TODO: trim it?
        if(name == "chip-name")
        {
            this->m_pclArg->chip_name = text;
        }
        else if(name == "storage-type")
        {
            std::string type = text;
            if(type == "NOR")
            {
                this->m_pclArg->storage_type = HW_STORAGE_NOR;
            }
            else if(type == "NAND")
            {
                this->m_pclArg->storage_type = HW_STORAGE_NAND;
            }
            else if(type == "EMMC")
            {
                this->m_pclArg->storage_type = HW_STORAGE_EMMC;
            }
            else if(type == "SDMMC")
            {
                this->m_pclArg->storage_type = HW_STORAGE_SDMMC;
            }
            else if(type == "UFS")
            {
                this->m_pclArg->storage_type = HW_STORAGE_UFS;
            }
            else
            {
                Q_ASSERT(0 && "unknown storage type");
            }
        }
        else if(name == "download-agent")
        {
            /*TINYXML:To decode string with UTF-8 when encounters path, {*/
            path = QString::fromUtf8(text.c_str());
            QFileInfo info(path);
            if (info.isRelative())
                path = info.absoluteFilePath();
            /*TINYXML:To decode string with UTF-8 when encounters path, }*/
            this->m_pclArg->da_file = path.toLocal8Bit().constData();
        }
        else if(name == "scatter")
        {
            path = QString::fromUtf8(text.c_str());

            this->m_pclArg->scatter_file = path.toLocal8Bit().constData();
        }
        else if(name == "authentication")
        {
            path = QString::fromUtf8(text.c_str());

            this->m_pclArg->auth_file = path.toLocal8Bit().constData();
        }
        else if(name == "certification")
        {
            path = QString::fromUtf8(text.c_str());

           this->m_pclArg->cert_file = path.toLocal8Bit().constData();
        }
        else if(name == "rom-list")
        {
            XML::Node rom_node = child.GetFirstChildNode();
            while(!rom_node.IsEmpty())
            {
                RomItem item;
                QString index_str = rom_node.GetAttribute("index").c_str();
                item.index = index_str.toUInt();

                item.enable = rom_node.GetAttribute("enable")=="true"?true:false;

                path = QString::fromUtf8(rom_node.GetText().c_str());

                item.path = path.toLocal8Bit().constData();

                this->m_pclArg->rom_list.push_back(item);

                rom_node = rom_node.GetNextSibling();
            }
        }
        else if(name == "connection")
        {
            this->m_pclConnSetting = QSharedPointer<APCore::ConnSetting>(APCore::ConnSetting::FromXML(child));
        }

        else if(name == "checksum-level")
        {
            this->m_pclChksumSetting = QSharedPointer<APCore::ChksumSetting>(APCore::ChksumSetting::FromXML(child));
        }

        else
        {
            Q_ASSERT(0 && "unknown general node");
        }

        child = child.GetNextSibling();
    }
}

void GeneralSetting::SaveXML(XML::Node &root_node) const
{
    root_node.AppendChildNode("chip-name", m_pclArg->chip_name);

    std::string storage;
    switch(this->m_pclArg->storage_type)
    {
    case HW_STORAGE_NOR:
        storage = "NOR";
        break;
    case HW_STORAGE_NAND:
        storage = "NAND";
        break;
    case HW_STORAGE_EMMC:
        storage = "EMMC";
        break;
    case HW_STORAGE_SDMMC:
        storage = "SDMMC";
        break;
    case HW_STORAGE_UFS:
        storage = "UFS";
        break;
    default:
        Q_ASSERT(0);
    }
    root_node.AppendChildNode("storage-type", storage);
    root_node.AppendChildNode("download-agent", this->m_pclArg->da_file);
    root_node.AppendChildNode("scatter", this->m_pclArg->scatter_file);
    root_node.AppendChildNode("authentication", this->m_pclArg->auth_file);
    root_node.AppendChildNode("certification", this->m_pclArg->cert_file);

    if(!this->m_pclArg->rom_list.empty())
    {
        XML::Node rom_list_node = root_node.AppendChildNode("rom-list");
        for(std::list<RomItem>::const_iterator it=this->m_pclArg->rom_list.begin();
            it!=this->m_pclArg->rom_list.end(); ++it)
        {
            QString path = QString::fromLocal8Bit(it->path.c_str());

            XML::Node rom_node = rom_list_node.AppendChildNode("rom",path.toUtf8().constData());

            QString index_str = QString::number(it->index);
            rom_node.SetAttribute("index",index_str.toStdString());

            rom_node.SetAttribute("enable",it->enable?"true":"false");
        }
    }

    if(m_pclConnSetting != NULL)
    {
        m_pclConnSetting->SaveXML(root_node);
    }

    if(m_pclChksumSetting != NULL)
    {
        m_pclChksumSetting->SaveXML(root_node);
    }
}

QSharedPointer<GeneralCommand> GeneralSetting::pclCreateCommand(const QSharedPointer<AppCore>& app, const APKey& key)
{
    LOGI("General settings creat command");
    return QSharedPointer<GeneralCommand>(new GeneralCommand(m_pclArg,app, key));
}

}
