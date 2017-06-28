#ifndef ROMBASE_H
#define ROMBASE_H

#include "fw_defs.h"


class CDLProxy;

class IRomBase
{
public:
    virtual ~IRomBase(){}

public:
    virtual const char *name() const = 0;

public:
    virtual ERROR_T check(CDLProxy*) = 0;
    virtual ERROR_T read(CDLProxy*) = 0;
    virtual ERROR_T format(CDLProxy*) = 0;
    virtual ERROR_T download(CDLProxy*) = 0;
    virtual ERROR_T restore(CDLProxy*) = 0;    // write back
    virtual ERROR_T verify(CDLProxy*) = 0;     // checksum

};

#endif // ROMBASE_H
