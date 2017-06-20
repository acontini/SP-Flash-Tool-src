#include "DADownloadAllSetting.h"
#include "../Cmd/DADownloadAll.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{
DADownloadAllSetting::DADownloadAllSetting():
    cb_da_report(NULL),
    cb_bl_dl_init(NULL),
    cb_bl_dl_prog(NULL),
    cb_da_chksum_init(NULL),
    cb_da_chksum_progress(NULL),
    cb_dl_init(NULL),
    cb_dl_prog(NULL)
{
}

QSharedPointer<APCore::ICommand> DADownloadAllSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::DADownloadAll> cmd(new APCore::DADownloadAll(key));

    if(cb_da_report != NULL)
        cmd->download_arg.set_cb_da_report(cb_da_report);
    if(cb_bl_dl_init != NULL)
        cmd->download_arg.set_download_bloader_init(cb_bl_dl_init);
    if(cb_bl_dl_prog != NULL)
        cmd->download_arg.set_cb_download_bloader(cb_bl_dl_prog);
    if(cb_dl_init != NULL)
        cmd->download_arg.set_cb_download_flash_init(cb_dl_init);
    if(cb_dl_prog != NULL)
        cmd->download_arg.set_cb_download_flash(cb_dl_prog);
    if(cb_da_chksum_init!= NULL)
        cmd->download_arg.set_cb_da_chksum_init(cb_da_chksum_init);
    if(cb_da_chksum_progress!= NULL)
        cmd->download_arg.set_cb_da_chksum_progress(cb_da_chksum_progress);

    return cmd;
}

void DADownloadAllSetting::LoadXML(const XML::Node &node)
{
    //dummy impl, as there's no setting yet
    XML::Node child_node = node.GetFirstChildNode();

    while (!child_node.IsEmpty())
    {
        if(child_node.GetName() == "connection")
        {
            this->conn_setting = APCore::ConnSetting::FromXML(child_node);
            break;
        }
        child_node = child_node.GetNextSibling();
    }
}
void DADownloadAllSetting::SaveXML(XML::Node &node) const
{
    //dummy impl, as there's no setting yet
    XML::Node root_node = node.AppendChildNode("da-download-all");

    if(this->conn_setting != NULL)
    {
        this->conn_setting->SaveXML(root_node);
    }
}
}
