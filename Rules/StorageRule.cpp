#include "StorageRule.h"

#include "../Utility/Utils.h"
#include "../Logger/Log.h"
#include "../BootRom/flashtool.h"
#include "../XMLParser/XMLDocument.h"
#include "../Utility/FileUtils.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include <cassert>
#include <sstream>

#include <QCoreApplication>
#include <QFile>
#include <QDir>

#ifdef __BORLANDC__
#pragma warn -8004
#endif

using std::string;
using std::map;
using std::vector;
using XML::Node;
using XML::Document;

const string FlashToolStorageTable::kStoragePlatformName("name");
const string FlashToolStorageAttrTable::kStorageTypeNameAttr("type-name");
const string FlashToolStorageAttrTable::kStorageIsSupportAttr("is-support");
const string FlashToolStorageAttrTable::kStorageIsSupportAutoFmtAttr("is-support-auto-format");
const string FlashToolStorageAttrTable::kStorageHWTypeAttr("hw-storage-type");
const string FlashToolStorageAttrTable::kStorageMEMTypeAttr("mem-storage-type");
const string FlashToolStorageTable::kStorageRootNode("storage-setting");
const string FlashToolStorageTable::kStorageSettingFile("storage_setting.xml");

FlashToolStorageAttrTable::FlashToolStorageAttrTable() :
            is_support_(false),
            is_support_auto_fmt_(false),
            storage_type_(HW_STORAGE_NAND),
    memory_type_(HW_MEM_NAND)
{
    InitStorageTypeMap();
    InitMemoryTypeMap();
}

void FlashToolStorageAttrTable::InitStorageTypeMap(void)
{
    storage_type_map_["HW_STORAGE_NOR"] = HW_STORAGE_NOR;
    storage_type_map_["HW_STORAGE_NAND"] = HW_STORAGE_NAND;
    storage_type_map_["HW_STORAGE_EMMC"] = HW_STORAGE_EMMC;
    storage_type_map_["HW_STORAGE_SDMMC"] = HW_STORAGE_SDMMC;
    storage_type_map_["HW_STORAGE_UFS"] = HW_STORAGE_UFS;
}

void FlashToolStorageAttrTable::InitMemoryTypeMap(void)
{
    memory_type_map_["HW_MEM_NOR"] = HW_MEM_NOR;
    memory_type_map_["HW_MEM_NAND"] = HW_MEM_NAND;
    memory_type_map_["HW_MEM_EXT_SRAM"] = HW_MEM_EXT_SRAM;
    memory_type_map_["HW_MEM_EXT_DRAM"] = HW_MEM_EXT_DRAM;
    memory_type_map_["HW_MEM_EMMC"] = HW_MEM_EMMC;
    memory_type_map_["HW_MEM_SDMMC"] = HW_MEM_SDMMC;
    memory_type_map_["HW_MEM_UFS"] = HW_MEM_UFS;
}

void FlashToolStorageAttrTable::LoadXML(const XML::Node& node)
{
    //USES_CONVERSION;
    this->storage_name_ = node.GetAttribute(kStorageTypeNameAttr);
    this->is_support_ = ("true" == node.GetAttribute(kStorageIsSupportAttr)) ||
            ("TRUE" == node.GetAttribute(kStorageIsSupportAttr)) ? true : false;
    this->is_support_auto_fmt_ = ("true" == node.GetAttribute(kStorageIsSupportAutoFmtAttr)) ||
            ("TRUE" == node.GetAttribute(kStorageIsSupportAutoFmtAttr)) ? true : false;
    string storage_type = node.GetAttribute(kStorageHWTypeAttr);
    string memory_type = node.GetAttribute(kStorageMEMTypeAttr);
    std::map<std::string, HW_StorageType_E>::const_iterator it_storage = storage_type_map_.find(storage_type);
    assert(it_storage != storage_type_map_.end());
    this->storage_type_ = it_storage->second;
    std::map<std::string, HW_MemoryType_E>::const_iterator it_memory = memory_type_map_.find(memory_type);
    assert(it_memory != memory_type_map_.end());
    this->memory_type_ = it_memory->second;
}

void FlashToolStorageAttrTable::SaveXML(Node& node) const
{
    if(node.GetName().empty()) return;
    assert(0 && "Not support!");
}
FlashToolStorageTable::FlashToolStorageTable(const string& file_name)
{
    LoadFile(file_name);
}

void FlashToolStorageTable::LoadFile(const string& file_name)
{
        //USES_CONVERSION;
    Document document(file_name);
    const Node root_node = document.GetRootNode();
    assert(root_node.GetName() == FlashToolStorageTable::kStorageRootNode);

    StorageRule storage_rule;
    FlashToolStorageAttrTable storage_attr;
    Node child_node = root_node.GetFirstChildNode();

    std::string platform;
    StorageAttrMap storage_attr_table;

    while (!child_node.IsEmpty())
    {
        platform = child_node.GetAttribute(kStoragePlatformName);
        Node grandchild = child_node.GetFirstChildNode();

        while (!grandchild.IsEmpty())
        {
            storage_attr.LoadXML(grandchild);
            storage_rule.set_storage_name(storage_attr.storage_name());
            storage_rule.set_is_support(storage_attr.is_support());
            storage_rule.set_is_support_auto_format(storage_attr.is_support_auto_fmt());
            storage_rule.set_storage_type(storage_attr.storage_type());
            storage_rule.set_memory_type(storage_attr.memory_type());
            storage_attr_table[storage_rule.storage_name()]= storage_rule;
            storage_operation_map_[storage_rule.storage_type()] = storage_rule.storage_name();

            grandchild = grandchild.GetNextSibling();
        }
        this->platform_storage_table_.push_back(std::make_pair(platform, storage_attr_table));
        //Clear storage attribute map for next platform storage vector content
        storage_attr_table.clear();

        child_node = child_node.GetNextSibling();
    }
    Dump();
}

void FlashToolStorageTable::Dump(void)
{
    std::list<PlatformOperPair>::iterator it_plat_oper;
    StorageAttrMap::iterator it_storage_attr_map;
    it_plat_oper = platform_storage_table_.begin();
    //platform vector
    for(; it_plat_oper != platform_storage_table_.end(); it_plat_oper++) {
        LOG("platform(%s).",it_plat_oper->first.c_str());
        it_storage_attr_map = it_plat_oper->second.begin();
        //storage vector
        for(; it_storage_attr_map != it_plat_oper->second.end(); it_storage_attr_map++) {
            LOG("storage type(%s) -> is_support(%s), is_support_auto_format(%s), storage_type_(%s), memory_type_(%s).",
                it_storage_attr_map->first.c_str(),
                it_storage_attr_map->second.is_support() ? "true" : "false",
                it_storage_attr_map->second.is_support_auto_format() ? "true" : "false",
                HWStorageTypeToString(it_storage_attr_map->second.storage_type()),
                HWMemoryTypeToString(it_storage_attr_map->second.memory_type()));
        }
    }
    std::map<HW_StorageType_E, std::string>::const_iterator it_storage_type_name_map = storage_operation_map_.begin();
    for(; it_storage_type_name_map != storage_operation_map_.end(); it_storage_type_name_map++) {
        LOG("Storage type(%s) -> Storage name(%s).",
            HWStorageTypeToString(it_storage_type_name_map->first),
            it_storage_type_name_map->second.c_str());
    }
}

bool FlashToolStorageTable::QueryStorageInfo(
    const string& platform,
    const string& storage_name,
    StorageRule& storage_rule) {
    LOG("platform(%s), storage_name(%s) to be queried.", platform.c_str(), storage_name.c_str());
    bool is_platform_found = false;
    bool is_storage_name_found = false;
    std::list<PlatformOperPair>::iterator it_plat_vector = platform_storage_table_.begin();
    StorageAttrMap::iterator it_storage_attr_map;
    for (; it_plat_vector != platform_storage_table_.end(); it_plat_vector++)
    {
        LOG("platform(%s) in the table.", it_plat_vector->first.c_str());
        if (platform == it_plat_vector->first)
        {
            LOG("%s is matched.", it_plat_vector->first.c_str());
            is_platform_found = true;
            it_storage_attr_map = it_plat_vector->second.find(storage_name);
            if (it_storage_attr_map != it_plat_vector->second.end())
            {
                is_storage_name_found = true;
                storage_rule.set_is_support(it_storage_attr_map->second.is_support());
                LOG("%s type is matched, and (%s) be supported.",
                    storage_name.c_str(),
                    storage_rule.is_support() ? "Yes" : "No");
                if(storage_rule.is_support()) {
                    storage_rule.set_storage_name(it_storage_attr_map->second.storage_name());
                    storage_rule.set_storage_type(it_storage_attr_map->second.storage_type());
                    storage_rule.set_memory_type(it_storage_attr_map->second.memory_type());
                    storage_rule.set_is_support_auto_format(it_storage_attr_map->second.is_support_auto_format());
                } else {
                    storage_rule.set_storage_name("NAND");
                    storage_rule.set_storage_type(HW_STORAGE_NAND);
                    storage_rule.set_memory_type(HW_MEM_NAND);
                    storage_rule.set_is_support_auto_format(true);
                }
            } else {
                //default nand is applied
                storage_rule.set_is_support(false);
                storage_rule.set_storage_name("NAND");
                storage_rule.set_storage_type(HW_STORAGE_NAND);
                storage_rule.set_memory_type(HW_MEM_NAND);
            }
            break;
        }
    }
    return (is_platform_found && is_storage_name_found);
}

std::map<HW_StorageType_E, std::string> FlashToolStorageTable::storage_operation_map(void)
{
    //Note: For std::map don't use assignment return
    //Reason: The elements contained in the object before the call are dropped after assignment
    std::map<HW_StorageType_E, std::string> tmp_map;
    std::map<HW_StorageType_E, std::string>::iterator it_storage_oper_map = storage_operation_map_.begin();
    for(; it_storage_oper_map != storage_operation_map_.end(); it_storage_oper_map++)
    {
        tmp_map[it_storage_oper_map->first] = it_storage_oper_map->second;
    }
    return tmp_map;
}

//Singleton FlashToolStorageTable
FlashToolStorageTable& GetFlashToolStorageTable()
{
    std::string xml_file =
            ABS_PATH(FlashToolStorageTable::kStorageSettingFile);

    if(!FileUtils::IsFileExist(xml_file.c_str())) {
        LOGE("xml_file(%s) does NOT existed!", xml_file.c_str());
        THROW_APP_EXCEPTION(FT_FIND_STORAGE_ERROR, "");
    }

    static FlashToolStorageTable storage_table(xml_file);

    return storage_table;
}

FlashToolStorageConfig::FlashToolStorageConfig() :
    platform_("MT6573"),
    storage_name_("NAND"),
    memory_type_(HW_MEM_NAND),
    storage_type_(HW_STORAGE_NAND),
    is_support_auto_format_(false)
{
}

FlashToolStorageConfig::FlashToolStorageConfig(const FlashToolStorageConfig &storage)
{
    this->platform_ = storage.platform_;
    this->storage_name_ = storage.storage_name_;
    this->storage_type_ = storage.storage_type_;
    this->memory_type_ = storage.memory_type_;
    this->is_support_auto_format_ = storage.is_support_auto_format_;
}

FlashToolStorageConfig& FlashToolStorageConfig::operator =(const FlashToolStorageConfig &storage)
{
    if ( this == &storage) {
        return* this;
    } else {
        this->platform_ = storage.platform_;
        this->storage_name_ = storage.storage_name_;
        this->storage_type_ = storage.storage_type_;
        this->memory_type_ = storage.memory_type_;
        this->is_support_auto_format_ = storage.is_support_auto_format_;
        return *this;
    }
}

FlashToolStorageConfig::~FlashToolStorageConfig()
{}

bool FlashToolStorageConfig::QueryStorage(
        const string& platform,
        const string& storage_name,
        string& error_msg)
{
    try {
        FlashToolStorageTable storage_table = GetFlashToolStorageTable();
        StorageRule storage_rule;
        storage_table.QueryStorageInfo(platform, storage_name, storage_rule);
        this->platform_ = platform;
        this->storage_name_ = storage_rule.storage_name();
        this->storage_type_ = storage_rule.storage_type();
        this->memory_type_ = storage_rule.memory_type();
        this->is_support_auto_format_ = storage_rule.is_support_auto_format();
        if (storage_rule.is_support()) {
            LOG("(%s) is supported by (%s).",
                platform.c_str(),
                storage_rule.storage_name().c_str());
        } else {
            std::ostringstream oss;
            oss << storage_name << " is not supported by "
                << platform << ", default \""
                << storage_rule.storage_name() << "\" storage type is applied!";
            LOG("%s.", oss.str().c_str());
            error_msg = oss.str();
        }
        return storage_rule.is_support();
    } catch (const BaseException &ex) {
        LOGE("%s.", ex.err_msg().c_str());
        return false;
    }
}

void FlashToolStorageConfig::Dump()
{
    std::ostringstream tmp_oss;
    tmp_oss << "Storage name("
            <<GetStorageName().c_str()
           <<"), StorageType("
          <<HWStorageTypeToString(GetStorageType())
         <<"), MemoryType("
        <<HWMemoryTypeToString(GetMemoryType())
       <<"), IsSupportAutoFormat("
      <<(this->IsSupportAutoFormat() ? "true" : "false")
     <<").";
    LOG("%s", tmp_oss.str().c_str());
}

std::string FlashToolStorageConfig::QueryStorageNameByStorageType(const HW_StorageType_E type)
{
    try {
        FlashToolStorageTable storage_table = GetFlashToolStorageTable();
        std::map<HW_StorageType_E, std::string> tmp_map = storage_table.storage_operation_map();
        map<HW_StorageType_E, string>::const_iterator it = tmp_map.find(type);
        if ( it != storage_table.storage_operation_map().end() ) {
            return it->second;
        } else {
            return string("Unknown");
        }
    } catch (const BaseException &ex) {
        LOGE("%s.", ex.err_msg().c_str());
        return string("Unknown");
    }
}

bool FlashToolStorageConfig::IsOperationChangend(const string& platform, const string& storage_name)
{
    bool is_operation_change = false;
    static std::string platform_history;
    static std::string storage_name_history;
    LOG("Old platform(%s), storage type(%s).",
        platform_history.c_str(),
        storage_name_history.c_str());
    if ((platform != platform_history) || (storage_name != storage_name_history)) {
        is_operation_change = true;
    }
    LOG("New platform(%s), storage type(%s).", platform.c_str(), storage_name.c_str());
    platform_history = platform;
    storage_name_history = storage_name;
    return is_operation_change;
}


