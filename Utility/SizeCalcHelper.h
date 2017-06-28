#ifndef SIZECALCHELPER_H
#define SIZECALCHELPER_H

#include <QtCore>

#include "../BootRom/DOWNLOAD.H"
#include "../Conn/Connection.h"
#include "../Flow/storhelper.h"

typedef enum
{
    FORMAT_ONLY,
    DOWNLOAD
}OPER_TYPE;

class SizeCalcHelper
{
public:
    SizeCalcHelper(const QSharedPointer<APCore::Connection> &conn,
                   HW_StorageType_E storage_type, OPER_TYPE oper_type);
    SizeCalcHelper(const QSharedPointer<APCore::Connection> &conn,
                   HW_StorageType_E storage_type, OPER_TYPE oper_type, NUTL_AddrTypeFlag_E addr_type);

    U64 size();

    U64 physicalSize(const CStorHelper &helper);
    U64 LogicalSize(const CStorHelper &helper);

    void set_addr_type(const NUTL_AddrTypeFlag_E addr_type)
    {
        this->addr_type_ = addr_type;
    }

    void set_storage_type(const HW_StorageType_E storage_type)
    {
        this->storage_type_ = storage_type;
    }

private:
    const QSharedPointer<APCore::Connection> conn_;
    HW_StorageType_E storage_type_;
    OPER_TYPE  oper_type_;
    NUTL_AddrTypeFlag_E addr_type_;
};

#endif // SIZECALCHELPER_H
