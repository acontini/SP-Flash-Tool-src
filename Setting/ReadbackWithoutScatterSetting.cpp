#include "ReadbackWithoutScatterSetting.h"
#include "../Cmd/ReadbackWithoutScatterCommand.h"
#include "../Logger/Log.h"
#include "../XMLParser/XMLNode.h"
#include "../Public/AppCore.h"
#include "ReadbackSettingAssist.h"

namespace APCore
{
ReadbackWithoutScatterSetting::ReadbackWithoutScatterSetting()
{
}

QSharedPointer<APCore::ICommand> ReadbackWithoutScatterSetting::CreateCommand(APKey key){
    QSharedPointer<APCore::ReadbackWithoutScatterCommand> cmd(new APCore::ReadbackWithoutScatterCommand(key));

    cmd->set_partion_list(items);
    cmd->set_da_file(da_);

    return cmd;
}

void ReadbackWithoutScatterSetting::LoadXML(const XML::Node &node)
{
    LOGI("The node name is %s.", node.GetName().c_str());

    XML::Node child = node.GetFirstChildNode();

    while(!child.IsEmpty()){
        const std::string name = child.GetName();

        if(name == "readback-list"){
            XML::Node dl_node = child.GetFirstChildNode();

            while(!dl_node.IsEmpty()){
                std::string path = APCore::ReadbackSettingAssist::GenerateValidPath(dl_node.GetText());
                std::string partion_name = dl_node.GetAttribute("readback-partion");

                op_part_list_t item;
                memset(&item, 0, sizeof(item));
                strncpy(item.part_name, partion_name.c_str(), partion_name.length() + 1);
                strncpy(item.file_path, path.c_str(), path.length() + 1);

                items.push_back(item);

                dl_node = dl_node.GetNextSibling();
            }
        }

        child = child.GetNextSibling();
    }
}

void ReadbackWithoutScatterSetting::SaveXML(XML::Node &node) const{
    LOGI("The node name is %s.", node.GetName().c_str());
}

}
