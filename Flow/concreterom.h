#ifndef CONCRETEROM_H
#define CONCRETEROM_H

#include "defaultrom.h"


class CCompositeBLRom : public IRomBase
{
public:
    CCompositeBLRom();
    virtual ~CCompositeBLRom();

public:
    DECLARE_VFUNCS()

public:
    bool attach(const ROM_INFO&);

    virtual const char *name() const;

private:
    CDefaultRom *bl_alpha_;
    CDefaultRom *bl_beta_;
    mutable char *name_;
};

class CMT6582CompositeBLRom : public CCompositeBLRom
{
public:
    CMT6582CompositeBLRom();
    virtual ~CMT6582CompositeBLRom();

    virtual ERROR_T format(CDLProxy *);
};

// Erase according to PMT
class CInvisibleRom : public CDefaultRom
{
public:
    CInvisibleRom(const ROM_INFO&);
    virtual ~CInvisibleRom();

public:
    //virtual ERROR_T check(CDLProxy*);
    //virtual ERROR_T format(CDLProxy*);
    virtual ERROR_T download(CDLProxy*);
    virtual ERROR_T verify(CDLProxy*);
};

// Layout Unchanged: Don't touch it
// Layout Changed: Readback to protect it
class CProtectedRom : public CDefaultRom
{
public:
    CProtectedRom(const ROM_INFO&);
    virtual ~CProtectedRom();

public:
    virtual ERROR_T read(CDLProxy*);
    virtual ERROR_T format(CDLProxy *proxy);
    virtual ERROR_T restore(CDLProxy*);
    virtual ERROR_T verify(CDLProxy*);
};

class CBinRegionRom : public CProtectedRom
{
public:
    CBinRegionRom(const ROM_INFO&);
    virtual ~CBinRegionRom();

public:
    virtual ERROR_T restore(CDLProxy*);
};

// To DL: erase -> download
// To keep:
//  Layout Unchanged: Dont touch it
//  Layout Changed: doomsday!
typedef CDefaultRom CKeepVisibleRom;


#endif // CONCRETEROM_H
