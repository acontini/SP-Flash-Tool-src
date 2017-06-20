#ifndef DEFAULTROM_H
#define DEFAULTROM_H

#include "rombase.h"

#define _rom_length(rom)        \
    ((rom).end_addr - (rom).begin_addr)

#define pmt_is_changed(pmt, rom)    \
    ((pmt)->begin_addr != (rom).begin_addr || \
     (pmt)->image_length != _rom_length(rom) )


#define DECLARE_VFUNCS()    \
    virtual ERROR_T check(CDLProxy*);   \
    virtual ERROR_T read(CDLProxy*);    \
    virtual ERROR_T format(CDLProxy*);  \
    virtual ERROR_T download(CDLProxy*);\
    virtual ERROR_T restore(CDLProxy*); \
    virtual ERROR_T verify(CDLProxy*);


class CDefaultRom : public IRomBase
{
public:
    CDefaultRom(const ROM_INFO&);
    virtual ~CDefaultRom();

public:
    DECLARE_VFUNCS()

 public:
    const ROM_INFO &rom_info() const
    {
        return rom_info_;
    }

    bool CheckPmtLayout(CDLProxy*) const;

    virtual const char *name() const
    {
        return rom_info_.name;
    }

 protected:
     /*const*/ ROM_INFO rom_info_;
};

#endif // DEFAULTROM_H
