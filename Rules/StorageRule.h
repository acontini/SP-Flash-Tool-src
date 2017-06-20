#ifndef STORAGERULE_H
#define STORAGERULE_H

#include <map>
#include <list>
#include <string>
#include "../BootRom/DOWNLOAD.H"
#include "../XMLParser/XMLSerializable.h"
#include "../ResourceManager/NonCopyable.h"

class StorageRule
{
public:
    StorageRule() :
        storage_name_("Unknown"),
        is_support_(false),
        is_support_auto_format_(false),
        storage_type_(HW_STORAGE_NAND),
        memory_type_(HW_MEM_NAND)

    {}
    ~StorageRule(){}

public:
    //Getters
    std::string storage_name(void) {
        return this->storage_name_;
    }
    bool is_support(void) const {
        return this->is_support_;
    }
    bool is_support_auto_format(void) const {
        return this->is_support_auto_format_;
    }
    HW_StorageType_E storage_type(void) const {
        return this->storage_type_;
    }
    HW_MemoryType_E memory_type(void) const {
        return this->memory_type_;
    }
    //Setters
    void set_storage_name(const std::string& name) {
        this->storage_name_ = name;
    }
    void set_is_support(const bool is_support) {
        this->is_support_ = is_support;
    }
    void set_storage_type(const HW_StorageType_E storage_type) {
        this->storage_type_ = storage_type;
    }
    void set_memory_type(const HW_MemoryType_E mem_type) {
        this->memory_type_ = mem_type;
    }
    void set_is_support_auto_format(const bool is_support) {
        this->is_support_auto_format_ = is_support;
    }

private:
    std::string storage_name_;
    bool is_support_;
    bool is_support_auto_format_;
    HW_StorageType_E storage_type_;
    HW_MemoryType_E memory_type_;
};

class FlashToolStorageAttrTable : public XML::Serializable, private NonCopyable
{
public:
    static const std::string kStorageTypeNameAttr;
    static const std::string kStorageIsSupportAttr;
    static const std::string kStorageIsSupportAutoFmtAttr;
    static const std::string kStorageHWTypeAttr;
    static const std::string kStorageMEMTypeAttr;
public:
    FlashToolStorageAttrTable();
    virtual ~FlashToolStorageAttrTable(){}

    virtual void LoadXML(const XML::Node &node);
    virtual void SaveXML(XML::Node &node) const;

    std::string storage_name(void) {
        return this->storage_name_;
    }

    bool is_support(void) {
        return this->is_support_;
    }

    bool is_support_auto_fmt(void) {
        return this->is_support_auto_fmt_;
    }

    HW_StorageType_E storage_type(void) {
        return this->storage_type_;
    }

    HW_MemoryType_E memory_type(void) {
        return this->memory_type_;
    }

private:
    void InitStorageTypeMap(void);
    void InitMemoryTypeMap(void);

private:
    std::string storage_name_;
    bool is_support_;
    bool is_support_auto_fmt_;
    HW_StorageType_E storage_type_;
    HW_MemoryType_E memory_type_;

    std::map<std::string, HW_StorageType_E> storage_type_map_;
    std::map<std::string, HW_MemoryType_E> memory_type_map_;

};

class FlashToolStorageTable
{
public:
    static const std::string kStorageSettingFile;
    static const std::string kStorageRootNode;
    static const std::string kStoragePlatformName;

private:
    typedef std::map<std::string, StorageRule> StorageAttrMap;
    //<platform, storage-type-vector>
    typedef std::pair<std::string, StorageAttrMap> PlatformOperPair;

public:
    explicit FlashToolStorageTable(const std::string& file_name);
    virtual ~FlashToolStorageTable() {}
    bool QueryStorageInfo(
        const std::string& platform,
        const std::string& storage_name,
        StorageRule& storage_info);
    std::map<HW_StorageType_E, std::string> storage_operation_map(void);
    void Dump(void);
private:
    void LoadFile(const std::string& fileName);

private:
    std::list<PlatformOperPair> platform_storage_table_;
    std::map<HW_StorageType_E, std::string> storage_operation_map_;
};

FlashToolStorageTable& GetFlashToolStorageTable();

class FlashToolStorageConfig
{
public:
    explicit FlashToolStorageConfig(const std::string& platform, const std::string& storage_name) ;
    FlashToolStorageConfig();
    FlashToolStorageConfig(const FlashToolStorageConfig& storage);
    FlashToolStorageConfig& operator=(const FlashToolStorageConfig& storage);
    ~FlashToolStorageConfig();

    //Query Methods
    bool QueryStorage(const std::string& platform, const std::string& storage_name, std::string& error_msg);
    std::string QueryStorageNameByStorageType(const HW_StorageType_E type);
    //Getters Interface
    HW_StorageType_E GetStorageType() const {
        return storage_type_;
    }
    HW_MemoryType_E GetMemoryType() const {
        return memory_type_;
    }
    std::string GetStorageName() const {
        return storage_name_;
    }
    std::string GetPlatform() const {
        return platform_;
    }
    bool IsNandOperation() const {
        return (HW_STORAGE_NAND == GetStorageType());
    }

    bool IsEMMCOperation() const {
        return (HW_STORAGE_EMMC == GetStorageType());
    }

    bool IsNorOperation() const {
        return (HW_STORAGE_NOR == GetStorageType());
    }

    bool IsUfsOperation() const {
        return (HW_STORAGE_UFS == GetStorageType());
    }

    bool IsSupportAutoFormat() const {
        return is_support_auto_format_;
    }
    bool IsOperationChangend(const std::string& platform, const std::string& storage_name);

    void Dump(void);

private:
    std::string platform_;
    std::string storage_name_;
    HW_MemoryType_E memory_type_;
    HW_StorageType_E storage_type_;
    bool is_support_auto_format_;
};

#endif // STORAGERULE_H
