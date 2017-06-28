#ifndef APPTYPES_H
#define APPTYPES_H

#include <iostream>
#include "../BootRom/flashtool_handle.h"

class ImageInfo
{
public:
    ImageInfo():index(-1),enabled(false),visible(false), name(),location(),
        region_addr(0),begin_addr(0),end_addr(0), region(), partition_size(0){}

    int index;
    bool enabled;
    bool visible;
    std::string name;
    std::string location;
    U64 region_addr;
    U64  begin_addr;
    U64 end_addr;
    U32 region;
    U64 partition_size;
};

class ReadbackItem
{
public:
    ReadbackItem(unsigned int index, bool enable,
                  U64 addr, U64 len,
                 const std::string &path,
                 NUTL_ReadFlag_E read_flag,
                 U32 region_id = 8,
                 NUTL_AddrTypeFlag_E addr_flag = NUTL_ADDR_LOGICAL)
        :index_(index), enable_(enable),
          addr_(addr), len_(len), path_(path),
          read_flag_(read_flag), region_id_(region_id),
          addr_flag_(addr_flag)

    {}

    explicit ReadbackItem(RB_INFO &rb_info, NUTL_AddrTypeFlag_E addr_flag):
        index_(rb_info.index), enable_(rb_info.enable==_TRUE),
          addr_(rb_info.readback_addr), len_(rb_info.readback_len),
          path_(rb_info.filepath), read_flag_(rb_info.m_read_flag),
        region_id_(rb_info.part_id), addr_flag_(addr_flag)
    {}


    unsigned int index() const { return index_; }
    bool enable() const { return enable_; }

    U64 addr() const { return addr_; }
    U64 len() const { return len_; }
    std::string path() const { return path_; }
    U32 region() const { return region_id_;}

    NUTL_ReadFlag_E read_flag() const { return read_flag_; }
    NUTL_AddrTypeFlag_E addr_flag() const { return addr_flag_; }

private:
    unsigned int index_;
    bool enable_;
    U64 addr_;
    U64 len_;
    std::string path_;

    NUTL_ReadFlag_E read_flag_;
    U32 region_id_;
    NUTL_AddrTypeFlag_E addr_flag_;
};

#endif // APPTYPES_H
