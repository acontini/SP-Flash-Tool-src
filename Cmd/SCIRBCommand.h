#ifndef CLONEDLCOMMAND_H
#define CLONEDLCOMMAND_H

#include "ICommand.h"
#include "./Setting/ReadbackSetting.h"

#include <QSharedPointer>

#include "./Utility/CloneInfo.h"

namespace APCore
{
class SCIRBCommand : public ICommand
{
public:
    SCIRBCommand(APKey key);

    virtual ~SCIRBCommand();
    virtual void exec(const QSharedPointer<Connection> &conn);

    void set_readback_setting(const QSharedPointer<ReadbackSetting> &setting)
    {
        this->rb_setting_ = setting;
    }

    void set_readback_folderPath(const std::string &path){
        folder_path_ = path;
    }

    void read_pmt(const QSharedPointer<Connection> &conn);

    void DoReadback(const QSharedPointer<Connection> &conn);

    void DoCommand(const QSharedPointer<Connection> &conn);

    void SaveCheckSum();

    void SaveXMLFile();

    unsigned long long calculateFlashSize(DA_REPORT_T &da_report);

private:
    QSharedPointer<APCore::ReadbackSetting> rb_setting_;
    std::string folder_path_;
    std::list<CLONE_ITEM> cloneItem_list_;

    APKey key_;
};

}

#endif // CLONEDLCOMMAND_H
