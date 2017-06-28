#include "defaultrom.h"
#include "dlproxy.h"
#include "../Logger/Log.h"

CDefaultRom::CDefaultRom(const ROM_INFO &rom_info)
    : rom_info_(rom_info)
{
}

CDefaultRom::~CDefaultRom()
{
}

ERROR_T CDefaultRom::check(CDLProxy *proxy)
{
    if (!CheckPmtLayout(proxy))
    {
        return ERROR_ROM_MUST_ENABLE;
    }
    return ERROR_OK;
}

ERROR_T CDefaultRom::read(CDLProxy * /*proxy*/)
{
    return ERROR_OK;    // default: no read
}

ERROR_T CDefaultRom::format(CDLProxy *proxy)
{
    if (rom_info().enable)
    {
        const PART_INFO *pmt =
                proxy->read_pmt(rom_info().name);

        if (pmt == NULL || pmt->image_length == 0)
        {
            LOGD("warning: no PMT info for %s, "
                 "format according to scatter layout...",
                 rom_info().name);

            return proxy->format(rom_info().begin_addr,
                                 _rom_length(rom_info()),
                                 rom_info().part_id);
        }
        else
        {
            return proxy->format(pmt->begin_addr,
                                 pmt->image_length,
                                 pmt->part_id);
        }
    }
    return ERROR_OK;
}

ERROR_T CDefaultRom::download(CDLProxy *proxy)
{
    if (rom_info().enable)
    {
        return proxy->download(rom_info());
    }
    return ERROR_OK;
}

ERROR_T CDefaultRom::restore(CDLProxy * /*proxy*/)
{
    return ERROR_OK;    // default: no restore
}

ERROR_T CDefaultRom::verify(CDLProxy *proxy)
{
    return proxy->verify(rom_info().begin_addr,
                         rom_info().end_addr);
}

bool CDefaultRom::CheckPmtLayout(CDLProxy *proxy) const
{
    if (rom_info().enable != _TRUE)
    {
        const PART_INFO *pmt =
                proxy->read_pmt(rom_info().name);

        if (pmt == NULL || pmt_is_changed(pmt, rom_info()))
        {
            LOGD("PMT changed for <%s>!", rom_info().name);
            return false;
        }
    }
    return true;
}
