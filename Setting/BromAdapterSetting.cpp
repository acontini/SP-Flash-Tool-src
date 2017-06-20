#include "BromAdapterSetting.h"
#include "../Cmd/BromAdapterCommand.h"
#include "../XMLParser/XMLNode.h"
#include "../Host/Inc/RuntimeMemory.h"

namespace APCore
{

BromAdapterSetting::BromAdapterSetting():
    cb_wbuf_init(NULL),
    cb_wbuf_progress(NULL),
    start_address(0),
    jump_address(0),
    buf_len(0),
    buf(NULL),
    is_jump(false),
    is_only_jump(false)
{
}

QSharedPointer<APCore::ICommand> BromAdapterSetting::CreateCommand(APKey key)
{
    QSharedPointer<APCore::BromAdapterCommand> cmd(new APCore::BromAdapterCommand(key));

    if(cb_wbuf_init != NULL)
        cmd->brom_dl_arg_.set_cb_wbuf_init(cb_wbuf_init);

    if(cb_wbuf_progress != NULL)
        cmd->brom_dl_arg_.set_cb_wbuf_progress(cb_wbuf_progress);

    cmd->brom_dl_arg_.set_buf_len(buf_len);
    cmd->brom_dl_arg_.set_dl_begin_addr(start_address);
    cmd->brom_dl_arg_.set_dl_buf(buf);
    cmd->brom_dl_arg_.set_is_jump(is_jump);
    cmd->brom_dl_arg_.set_jump_addr(jump_address);
    cmd->set_is_only_jump(is_only_jump);
    cmd->set_is_manual_addr(is_manual_addr);

    return cmd;
}

void BromAdapterSetting::LoadXML(const XML::Node &node)
{
    Q_ASSERT("BromAdapterSetting::LoadXML was not implemented.");

    LOG("The node name is %s.", node.GetName().c_str());
}

void BromAdapterSetting::SaveXML(XML::Node &node) const
{
    Q_ASSERT("BromAdapterSetting::SaveXML was not implemented.");

    LOG("The node name is %s.", node.GetName().c_str());
}

}
