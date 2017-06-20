#include "ReadbackRule.h"
#include "../Logger/Log.h"

#define ReadbackRuleCount (sizeof(ReadbackruleTable)/sizeof(ReadbackRuleInfo))

ReadbackRuleInfo ReadbackruleTable[]=
{
    {HW_STORAGE_NOR, true, 4096, 32},
    {HW_STORAGE_NAND, true, 4096, 32},
    {HW_STORAGE_EMMC, false, 512, 64},
    {HW_STORAGE_UFS, false, 2048, 64}
};

ReadbackRule::ReadbackRule():
    does_set_rb_flag_(false),
    align_number_(0)
{
    this->InitReadBackRuleMap();
}

ReadbackRule::~ReadbackRule(){}

void ReadbackRule::InitReadBackRuleMap()
{
    for(unsigned int i=0; i<ReadbackRuleCount; i++)
    {
        readbackrule_map_[ReadbackruleTable[i].storage_type] = ReadbackruleTable[i];
    }
}

bool ReadbackRule::QueryReadBackRule(HW_StorageType_E storage_type)
{
    bool is_found = false;
    std::map<HW_StorageType_E, ReadbackRuleInfo>::iterator it = readbackrule_map_.find(storage_type);
    if(it != readbackrule_map_.end())
    {
       is_found = true;
       does_set_rb_flag_ = it->second.does_set_rb_flag;
       align_number_ = it->second.align_number;
    }

    is_found = false;

    return is_found;
}

void ReadbackRule::Dump()
{
    LOGD("does_set_rb_flag(%d)", this->does_set_rb_flag_);
    LOGD("align_number_(%d)", this->align_number_);
}
