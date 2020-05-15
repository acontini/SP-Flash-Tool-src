#include "../Conn/Connection.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Handle/DLHandle.h"
#include "storhelper.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"
#include "../Host/Inc/RuntimeMemory.h"

class CStorHelperImpl
{
protected:
    CStorHelperImpl(U64 align,
                    U64 block,
                    U64 logical,
                    U64 physical)
        : align_size_(align),
          block_size_(block),
          logic_size_(logical),
          physi_size_(physical)
    {
    }

public:
    U64 AlignSize(void) const
    {
        return align_size_;
    }

    U64 BlockSize(void) const
    {
        return block_size_;
    }

    U64 LogicalSize(void) const
    {
        return logic_size_;
    }

    U64 PhysicalSize(void) const
    {
        return physi_size_;
    }

protected:
    static U64 GetReservedRomSize(APCore::Connection *conn)
    {
        U64 rsv_size = 0;
        APKey key = conn->ap_key();

        DL_HANDLE_T dl_handle = GET_DL_HANDLE_T(key);
        FLASHTOOL_API_HANDLE_T ft_handle = conn->FTHandle();

        int ret = FlashTool_GetReservedRomSize(
                    ft_handle,
                    dl_handle,
                    &rsv_size);

        return (ret != S_DONE) ? 0 : rsv_size;
    }

protected:
    const U64 align_size_;
    const U64 block_size_;
    const U64 logic_size_;
    const U64 physi_size_;
};


class CStorNorHelper : public CStorHelperImpl
{
public:
    CStorNorHelper(APCore::Connection *conn)
        : CStorHelperImpl(
              CalcAlign(conn->da_report()),
              CalcBlock(conn->da_report()),
              CalcLogical(conn),
              CalcPhysical(conn->da_report()))
    {
    }

private:
    static U64 CalcAlign(const DA_REPORT_T &rep)
    {
	Q_UNUSED(rep);
        return 4*1024;
    }

    static U64 CalcBlock(const DA_REPORT_T &rep)
    {
        Q_UNUSED(rep);
        return 128*1024;
    }

    static U64 CalcLogical(APCore::Connection *conn)
    {
        const DA_REPORT_T &rep = conn->da_report();
        const U64 rsv_size = GetReservedRomSize(conn);
        return rep.m_nor_flash_size - rsv_size;
    }

    static U64 CalcPhysical(const DA_REPORT_T &rep)
    {
        return rep.m_nor_flash_size;
    }
};

class CStorNandHelper : public CStorHelperImpl
{
public:
    CStorNandHelper(APCore::Connection *conn)
        : CStorHelperImpl(
              CalcAlign(conn->da_report()),
              CalcBlock(conn->da_report()),
              CalcLogical(conn),
              CalcPhysical(conn->da_report()))
    {
    }

private:
    static U64 CalcAlign(const DA_REPORT_T &rep)
    {
        return rep.m_nand_pagesize * rep.m_nand_pages_per_block;
    }

    static U64 CalcBlock(const DA_REPORT_T &rep)
    {
        return rep.m_nand_pagesize * rep.m_nand_pages_per_block;
    }

    static U64 CalcLogical(APCore::Connection *conn)
    {
        const DA_REPORT_T &rep = conn->da_report();
        const U64 rsv_size = GetReservedRomSize(conn); // PMT included
        return rep.m_nand_flash_size - rsv_size;
    }

    static U64 CalcPhysical(const DA_REPORT_T &rep)
    {
        return rep.m_nand_flash_size;
    }
};

class CStorEmmcHelper : public CStorHelperImpl
{
public:
    CStorEmmcHelper(APCore::Connection *conn)
        : CStorHelperImpl(
              CalcAlign(conn->da_report()),
              CalcBlock(conn->da_report()),
              CalcLogical(conn),
              CalcPhysical(conn->da_report()))
    {
    }

private:
    static U64 CalcAlign(const DA_REPORT_T &)
    {
        return 512;
    }

    static U64 CalcBlock(const DA_REPORT_T &)
    {
        return 128*1024;
    }

    static U64 CalcLogical(APCore::Connection *conn)
    {
        const DA_REPORT_T &rep = conn->da_report();
        const U64 rsv_size = GetReservedRomSize(conn);
        return CalcPhysical(rep) - rsv_size;
    }

    static U64 CalcPhysical(const DA_REPORT_T &rep)
    {
        return rep.m_emmc_boot1_size +
                rep.m_emmc_boot2_size +
                rep.m_emmc_rpmb_size +
                rep.m_emmc_gp1_size +
                rep.m_emmc_gp2_size +
                rep.m_emmc_gp3_size +
                rep.m_emmc_gp4_size +
                rep.m_emmc_ua_size;
    }
};

class CStorSdmmcHelper : public CStorHelperImpl
{
public:
    CStorSdmmcHelper(APCore::Connection *conn)
        : CStorHelperImpl(
              CalcAlign(conn->da_report()),
              CalcBlock(conn->da_report()),
              CalcLogical(conn),
              CalcPhysical(conn->da_report()))
    {
    }

private:
    static U64 CalcAlign(const DA_REPORT_T &)
    {
        return 512;
    }

    static U64 CalcBlock(const DA_REPORT_T &)
    {
        return 128*1024;
    }

    static U64 CalcLogical(APCore::Connection *conn)
    {
        const DA_REPORT_T &rep = conn->da_report();
        const U64 rsv_size = GetReservedRomSize(conn);
        return CalcPhysical(rep) - rsv_size;
    }

    static U64 CalcPhysical(const DA_REPORT_T &rep)
    {
        return rep.m_sdmmc_ua_size;
    }
};

class CStorUfsHelper : public CStorHelperImpl
{
public:
    CStorUfsHelper(APCore::Connection *conn)
        : CStorHelperImpl(
              CalcAlign(conn->da_report()),
              CalcBlock(conn->da_report()),
              CalcLogical(conn),
              CalcPhysical(conn->da_report()))
    {
    }

private:
    static U64 CalcAlign(const DA_REPORT_T &rep)
    {
        Q_UNUSED(rep);
        return 2*1024;
    }

    static U64 CalcBlock(const DA_REPORT_T &rep)
    {
        Q_UNUSED(rep);
        // YAFFS_IMG phased out, no need to calculate nvram offset for UFS
        return -1;
    }

    static U64 CalcLogical(APCore::Connection *conn)
    {
        const DA_REPORT_T &rep = conn->da_report();
        const U64 rsv_size = GetReservedRomSize(conn);
        return CalcPhysical(rep) - rsv_size;
    }

    static U64 CalcPhysical(const DA_REPORT_T &rep)
    {
        return rep.m_ufs_lu0_size +
                rep.m_ufs_lu1_size +
                rep.m_ufs_lu2_size/* +
                rep.m_ufs_lu3_size*/;
    }
};

CStorHelper::CStorHelper(APCore::Connection *conn)
{
    const DA_REPORT_T &rep = conn->da_report();

    if (S_DONE == rep.m_nand_ret)
    {
        my_impl_ = new CStorNandHelper(conn);
    }
    else if (S_DONE == rep.m_emmc_ret)
    {
        my_impl_ = new CStorEmmcHelper(conn);
    }
    else if (S_DONE == rep.m_sdmmc_ret)
    {
        my_impl_ = new CStorSdmmcHelper(conn);
    }
    else if (S_DONE == rep.m_nor_ret)
    {
        my_impl_ = new CStorNorHelper(conn);
    }
    else if (S_DONE == rep.m_ufs_ret)
    {
        my_impl_ = new CStorUfsHelper(conn);
    }
    else
    {
        //assert(false);
        my_impl_ = NULL;
        THROW_APP_EXCEPTION(FT_INVALID_MEM_INFO,"");
    }
}

CStorHelper::~CStorHelper()
{
    if (my_impl_ != NULL)
    {
        delete my_impl_;
    }
}

U64 CStorHelper::AlignSize(void) const
{
    return my_impl_!=NULL ? my_impl_->AlignSize() : 0;
}

U64 CStorHelper::BlockSize(void) const
{
    return my_impl_!=NULL ? my_impl_->BlockSize() : 0;
}

U64 CStorHelper::LogicalSize(void) const
{
    return my_impl_!=NULL ? my_impl_->LogicalSize() : 0;
}

U64 CStorHelper::PhysicalSize(void) const
{
    return my_impl_!=NULL ? my_impl_->PhysicalSize() : 0;
}
