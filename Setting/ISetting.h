#ifndef ISETTING_H
#define ISETTING_H

#include "../Public/Host.h"
#include <iostream>
#include <QSharedPointer>
#include "../Cmd/ICommand.h"
#include "../XMLParser/XMLSerializable.h"
#include "ConnSetting.h"
#include "../Logger/Log.h"

class ISetting : public XML::Serializable
{
public:
    ISetting()
        : conn_setting(NULL),
          storage_type_(HW_STORAGE_NAND)
    {
    }

    virtual ~ISetting()
    {
        if(conn_setting!=NULL)
            delete conn_setting;
    }

    virtual QSharedPointer<APCore::ICommand> CreateCommand(APKey key) = 0;

    virtual void set_storage_type(HW_StorageType_E type)
    {
        storage_type_ = type;
    }

    virtual void set_da(const std::string &da){
        da_ = da;
    }

    virtual void set_stop_flag(int *stop_flag)
    {
        //dummy
        LOG("dummpy stop_flag(0x%08X) set.", stop_flag);
    }


    APCore::ConnSetting* GetConnSetting() const
    {
        return conn_setting;
    }

    void SetConnSetting(APCore::ConnSetting* setting)
    {
        conn_setting = setting;
    }

protected:
    std::string com_port; //empty means USB
    APCore::ConnSetting* conn_setting;
    HW_StorageType_E storage_type_;
    std::string da_;
};

#endif // ISETTING_H
