#ifndef CHKSUM_RULE_H_
#define CHKSUM_RULE_H_

#include <map>
#include <string>

class ChksumRule{
public:
    ChksumRule(const std::string &path);
    ~ChksumRule();

    bool is_do_chksum(void)const
    {
        return is_do_chksum_;
    }

    unsigned int QueryChksumByImageName(const std::string &image_name);

private:
    void IniChksumMap(const std::string &path);

private:
    static const std::string kChksumFileName;
    std::map<std::string, unsigned int> image_chksum_map_;
    bool is_do_chksum_;
};
#endif
