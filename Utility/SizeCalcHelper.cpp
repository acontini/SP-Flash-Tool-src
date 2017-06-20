#include "SizeCalcHelper.h"


SizeCalcHelper::SizeCalcHelper(const QSharedPointer<APCore::Connection> &conn,
                               HW_StorageType_E storage_type, OPER_TYPE oper_type)
{
    SizeCalcHelper(conn, storage_type, oper_type, NUTL_ADDR_LOGICAL);
}

SizeCalcHelper::SizeCalcHelper(const QSharedPointer<APCore::Connection> &conn,
    HW_StorageType_E storage_type, OPER_TYPE oper_type,  NUTL_AddrTypeFlag_E addr_type):
    conn_(conn),
    storage_type_(storage_type),
    oper_type_(oper_type),
    addr_type_(addr_type)
{

}

U64 SizeCalcHelper::size()
{
    CStorHelper storage(conn_.data());

    if(addr_type_ == NUTL_ADDR_PHYSICAL)
        return physicalSize(storage);
    else
        return LogicalSize(storage);
}

U64 SizeCalcHelper::physicalSize(const CStorHelper &storage)
{
    return storage.PhysicalSize();
}

U64 SizeCalcHelper::LogicalSize(const CStorHelper &storage)
{
    if(storage_type_ == HW_STORAGE_NAND)
        return storage.LogicalSize();
    else if(storage_type_ == HW_STORAGE_NOR)
    {
        //TODO:mtk80800 NOR
        return storage.LogicalSize();
    }
    else if(storage_type_ == HW_STORAGE_UFS)
    {
        //TODO:mtk80800 UFS
        if(oper_type_ == FORMAT_ONLY)
            return storage.PhysicalSize();
        else
            return storage.LogicalSize();
    }
    else
    {
        if(oper_type_ == FORMAT_ONLY)
            return storage.PhysicalSize();
        else
            return storage.LogicalSize();
    }
}
