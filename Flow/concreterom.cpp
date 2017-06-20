#include "concreterom.h"
#include "dlproxy.h"
#include "restoreworker.h"
#include "../Logger/Log.h"
#include "../Host/Inc/RuntimeMemory.h"

/*****************************************************/
/***************** CCompositeBLRom *******************/
/*****************************************************/

CCompositeBLRom::CCompositeBLRom()
    : bl_alpha_(NULL),
      bl_beta_(NULL),
      name_(NULL)
{
}

CCompositeBLRom::~CCompositeBLRom()
{
    if (bl_alpha_ != NULL)
        delete bl_alpha_;
    if (bl_beta_ != NULL)
        delete bl_beta_;
    if (name_ != NULL)
        delete [] name_;
}

bool CCompositeBLRom::attach(const ROM_INFO &rom)
{
    if (bl_alpha_ == NULL)
    {
        bl_alpha_ = new CDefaultRom(rom);
    }
    else if (bl_beta_ == NULL)
    {
        bl_beta_ = new CDefaultRom(rom);
    }
    else
    {
        return false;
    }

    return true;
}

const char *CCompositeBLRom::name() const
{
    if (bl_alpha_ != NULL)
    {
        if (bl_beta_ != NULL)
        {
            if (name_ == NULL)
            {
                const char *name1 = bl_alpha_->name();
                const char *name2 = bl_beta_->name();
                name_ = new char[strlen(name1)+strlen(name2)+4];
                sprintf(name_, "%s & %s", name1, name2);
            }
            return name_;
        }
        return bl_alpha_->rom_info().name;
    }
    return "";
}

ERROR_T CCompositeBLRom::check(CDLProxy *proxy)
{
    if (bl_alpha_ != NULL && bl_beta_ != NULL)
    {
        if (bl_alpha_->rom_info().enable != bl_beta_->rom_info().enable)
        {
            return ERROR_BL_INCONSISTENT;
        }
    }

    if (bl_alpha_ != NULL && !bl_alpha_->CheckPmtLayout(proxy))
    {
        return ERROR_ROM_MUST_ENABLE;
    }

    if (bl_beta_ != NULL && !bl_beta_->CheckPmtLayout(proxy))
    {
        return ERROR_ROM_MUST_ENABLE;
    }

    return ERROR_OK;
}

ERROR_T CCompositeBLRom::read(CDLProxy*)
{
    return ERROR_OK;    // no read
}

ERROR_T CCompositeBLRom::format(CDLProxy *proxy)
{
    ERROR_T err = ERROR_OK;

    if (bl_alpha_ != NULL && bl_alpha_->rom_info().enable)
    {
        if ((err = bl_alpha_->format(proxy)) != ERROR_OK)
            return err;
    }

    if (bl_beta_ != NULL && bl_alpha_->rom_info().enable)
    {
        if ((err = bl_beta_->format(proxy)) != ERROR_OK)
            return err;
    }

    return err;
}

ERROR_T CCompositeBLRom::download(CDLProxy *proxy)
{
    ERROR_T err = ERROR_OK;

    if (bl_alpha_ != NULL && bl_alpha_->rom_info().enable)
    {
        if ((err = bl_alpha_->download(proxy)) != ERROR_OK)
            return err;
    }

    if (bl_beta_ != NULL && bl_alpha_->rom_info().enable)
    {
        if ((err = bl_beta_->download(proxy)) != ERROR_OK)
            return err;
    }

    return err;
}

ERROR_T CCompositeBLRom::restore(CDLProxy*)
{
    return ERROR_OK;
}

ERROR_T CCompositeBLRom::verify(CDLProxy*)
{
    return ERROR_OK;
}

/*****************************************************/
/******************* CMT6582CompositeBLROm *******************/
/*****************************************************/

// for MT6582, cannot format preloader

CMT6582CompositeBLRom::CMT6582CompositeBLRom()
{
}

CMT6582CompositeBLRom::~CMT6582CompositeBLRom()
{
}

ERROR_T CMT6582CompositeBLRom::format(CDLProxy *)
{
    return ERROR_OK;
}

/*****************************************************/
/******************* CInvisibleRom *******************/
/*****************************************************/

// for invisible ROM, only "format" is useful
// and "format" has been implemented in default ROM
// so, we just provide empty bodies for other funcs

CInvisibleRom::CInvisibleRom(const ROM_INFO &rom_info)
    : CDefaultRom(rom_info)
{
    rom_info_.enable = _TRUE;
}

CInvisibleRom::~CInvisibleRom()
{
}

ERROR_T CInvisibleRom::download(CDLProxy*)
{
    return ERROR_OK;    // invisible: no DL
}

ERROR_T CInvisibleRom::verify(CDLProxy*)
{
    return ERROR_OK;    // no verification
}

/*****************************************************/
/******************** CProtectedRom ******************/
/*****************************************************/

CProtectedRom::CProtectedRom(const ROM_INFO &rom_info)
    : CDefaultRom(rom_info)
{
    rom_info_.enable = _TRUE;
}

CProtectedRom::~CProtectedRom()
{
}

ERROR_T CProtectedRom::read(CDLProxy *proxy)
{
    // Layout Unchanged: Don't touch it
    // Layout Changed: Readback to protect it

    const PART_INFO *pmt = proxy->read_pmt(rom_info().name);

    if (pmt == NULL)
    {
        return ERROR_PMT_UNAVAILABLE;
    }

    if ( pmt->image_length == 0 ||
         !pmt_is_changed(pmt, rom_info()))
    {
        return ERROR_OK;
    }

    LOGD("PMT changed for <%s>!", rom_info().name);

    if (pmt->image_length == _rom_length(rom_info()))
    {
        return proxy->read(
                    pmt->begin_addr,
                    pmt->image_length);
    }

    return ERROR_DATA_LOST;
}

ERROR_T CProtectedRom::format(CDLProxy *proxy)
{
    const PART_INFO *pmt = proxy->read_pmt(rom_info().name);

    if (pmt == NULL || pmt->image_length == 0)
    {
        LOGD("warning: no PMT info for %s, "
                 "format according to scatter layout...",
                 rom_info().name);

        return proxy->format(rom_info().begin_addr,
                             _rom_length(rom_info()),
                             rom_info().part_id);
    }

    if ( pmt->image_length == 0 ||
         !pmt_is_changed(pmt, rom_info()))
    {
        rom_info_.enable = _FALSE;
        return ERROR_OK;
    }
    else
    {
       return proxy->format(pmt->begin_addr,
                             pmt->image_length,
                             pmt->part_id);
    }
}

ERROR_T CProtectedRom::restore(CDLProxy *proxy)
{
    // Layout Unchanged: Don't touch it
    // Layout Changed: Readback to protect it

    const PART_INFO *pmt = proxy->read_pmt(rom_info().name);

    // NOTE: simplified conditions here
    // because all errors have been checked in "read"

    if (pmt != NULL)
    {
        NUTL_ReadFlag_E flag = NUTL_READ_PAGE_ONLY;

        if(rom_info().rom_type == YAFFS_IMG)
            flag = NUTL_READ_PAGE_SPARE_WITH_ECCDECODE;

        return proxy->restore(
                    new CDefaultRestoreWorker(
                        pmt->name,
                        pmt->begin_addr,
                        pmt->image_length,
                        rom_info().begin_addr,
                        flag,
                        rom_info().part_id));
    }
    return ERROR_OK;
}

ERROR_T CProtectedRom::verify(CDLProxy*)
{
    return ERROR_OK;    // no verification?
}

/*****************************************************/
/******************** CBinRegionRom ******************/
/*****************************************************/

CBinRegionRom::CBinRegionRom(const ROM_INFO &rom_info)
    : CProtectedRom(rom_info)
{
}

CBinRegionRom::~CBinRegionRom()
{
}

ERROR_T CBinRegionRom::restore(CDLProxy *proxy)
{
    // Layout Unchanged: Don't touch it
    // Layout Changed: Readback to protect it

    const PART_INFO *pmt = proxy->read_pmt(rom_info().name);

    // NOTE: simplified conditions here
    // because all errors have been checked in "read"

    if (pmt != NULL)
    {
        NUTL_ReadFlag_E flag = NUTL_READ_PAGE_ONLY;

       if(rom_info().rom_type == YAFFS_IMG)
            flag = NUTL_READ_PAGE_SPARE_WITH_ECCDECODE;

        return proxy->restore(
                    new CNvRamRestoreWorker(
                        pmt->name,
                        pmt->begin_addr,
                        pmt->image_length,
                        rom_info().begin_addr,
                        flag,
                        pmt->part_id));
    }
    return ERROR_OK;
}
