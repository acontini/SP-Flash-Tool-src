#include "DLWithoutScatterSetting.h"
#include "../Cmd/DLWithoutScatterCommand.h"
#include "../Logger/Log.h"
#include "../XMLParser/XMLNode.h"
#include "../Public/AppCore.h"

namespace APCore
{
DLWithoutScatterSetting::DLWithoutScatterSetting():
    cb_operation_progress_(NULL),
    cb_stage_message_(NULL)
{
}

QSharedPointer<APCore::ICommand> DLWithoutScatterSetting::CreateCommand(APKey key){
    QSharedPointer<APCore::DLWithoutScatterCommand> cmd(new APCore::DLWithoutScatterCommand(key));

    cmd->set_partion_list(partList_);
    cmd->set_da_file(da_);

    return cmd;
}

void DLWithoutScatterSetting::LoadXML(const XML::Node &node){
    LOGI("The node name is %s.", node.GetName().c_str());

    XML::Node child = node.GetFirstChildNode();

    while(!child.IsEmpty()){
        const std::string name = child.GetName();

        if(name == "download-list"){
            XML::Node dl_node = child.GetFirstChildNode();

            while(!dl_node.IsEmpty()){
                std::string path = dl_node.GetText();
                std::string partion_name = dl_node.GetAttribute("partition");

                op_part_list_t item;
                memset(&item, 0, sizeof(item));
                strncpy(item.part_name, partion_name.c_str(), partion_name.length() + 1);
                strncpy(item.file_path, path.c_str(), path.length() + 1);

                partList_.push_back(item);

                dl_node = dl_node.GetNextSibling();
            }
        }

        child = child.GetNextSibling();
    }
}

void DLWithoutScatterSetting::SaveXML(XML::Node &node) const{
    LOGI("The node name is %s.", node.GetName().c_str());
}

}
