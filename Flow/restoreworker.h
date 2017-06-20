#ifndef RESTOREWORKER_H
#define RESTOREWORKER_H

#include "../BootRom/DOWNLOAD.H"

class CStorHelper;

class CRestoreWorker
{
public:
    CRestoreWorker(std::string part_name, U64 addr, U64 leng, U64 dest, NUTL_ReadFlag_E flag, unsigned short part);
    virtual ~CRestoreWorker();

    bool SetChipId(
            const char chip_id[],
            const size_t length);

    std::string Path(void) const;

    static std::string BasePath(
            const char chip_id[],
            const size_t length);

    static std::string BasePathNoCreateIfNotExist(
            const char chip_id[],
            const size_t length);

public:
    std::string part_name() const {return part_name_;}
    U64 addr() const { return addr_; }
    U64 leng() const { return leng_; }
    U64 dest() const { return dest_; }
    U64 end()  const { return addr_+leng_; }
    NUTL_ReadFlag_E flag() const { return flag_;}
    unsigned short part() const { return part_;}

    bool inpos() const
    {
        return (leng_==0 || addr_==dest_);
    }

public:
    virtual bool Stamp(
            const CStorHelper *stor) = 0;

    virtual bool Verify(
            const CStorHelper *stor) const;

    virtual bool Clean(void) = 0;


protected:
    std::string part_name_;
    U64 addr_;
    U64 leng_;
    U64 dest_;
    NUTL_ReadFlag_E flag_;
    unsigned short part_;

    std::string base_dir_;
};

class CDefaultRestoreWorker : public CRestoreWorker
{
public:
    CDefaultRestoreWorker(std::string part_name, U64 addr, U64 leng, U64 dest, NUTL_ReadFlag_E flag, unsigned short part);

    virtual bool Stamp(
            const CStorHelper *stor);

    virtual bool Verify(
            const CStorHelper *stor) const;

    virtual bool Clean(void);

private:
    std::string ChksumFilePath(void) const;

};

class CNvRamRestoreWorker : public CRestoreWorker
{
public:
    CNvRamRestoreWorker(std::string part_name, U64 addr, U64 leng, U64 dest, NUTL_ReadFlag_E flag, unsigned short part);

    virtual bool Stamp(
            const CStorHelper *stor);

    virtual bool Verify(
            const CStorHelper *stor) const;

    virtual bool Clean(void);
};

#endif // RESTOREWORKER_H
