#ifndef READBACKRULE_H
#define READBACKRULE_H

#include "../BootRom/DOWNLOAD.H"

#include <string>
#include <map>

typedef struct
{
  HW_StorageType_E storage_type;
  bool does_set_rb_flag;
  unsigned align_number;
  unsigned addressing_type;
}ReadbackRuleInfo;

class ReadbackRule
{
public:
    ReadbackRule();
    ~ReadbackRule();

    bool does_set_rb_flag()
    {
       return does_set_rb_flag_;
    }

    unsigned align_number()
    {
       return align_number_;
    }

    bool QueryReadBackRule(HW_StorageType_E storage_type);

private:
    void InitReadBackRuleMap(void);
    void Dump(void);
private:
    bool does_set_rb_flag_;
    unsigned int align_number_;
    std::map <HW_StorageType_E, ReadbackRuleInfo>readbackrule_map_;
};


#endif // READBACKRULE_H
