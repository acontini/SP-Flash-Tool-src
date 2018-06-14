#include "DLOnlyCommand.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/CoreHandle.h"
#include "../Resource/Handle/DLHandle.h"
#include "../Resource/Handle/RBHandle.h"

#include "../Flow/storhelper.h"
#include "../Flow/dlproxy.h"
#include "../Flow/defaultrom.h"
#include "../Utility/constString.h"
#include "../Public/RomInfoFactory.h"
#include "../Host/Inc/RuntimeMemory.h"
#include "../Logger/Log.h"
#include "../Err/Exception.h"
#include "../Err/FlashToolErrorCodeDef.h"

#include <QtGui>

namespace APCore
{

DLOnlyCommand::DLOnlyCommand(APKey key):
    ICommand(key),
    dl_setting_(NULL),
    key_(key)
{
}

DLOnlyCommand::~DLOnlyCommand()
{

}

bool DLOnlyCommand::CheckPMTLayoutChange(const QSharedPointer<Connection> &conn)
{
    APCore::CoreHandle *core_handle = __POOL_OF_HANDLE(key_);
    if(core_handle == NULL)
    {
        THROW_APP_EXCEPTION(FT_INVALIDE_SESSION_KEY, "");
    }

    APCore::RBHandle *rb_handle = GET_RB_HANDLE(key_);

    CStorHelper storage(conn.data());

    CDLProxy proxy(conn, rb_handle, storage);

    std::list<ROM_INFO> rom_list;

    RomInfoFactory info_factory(key_);
    info_factory.GetRomList(conn, rom_list);

    QWidgetList l = QApplication::allWidgets();
    QTableWidget *tw = NULL;
    foreach (QWidget *w, l) {
        if (w->objectName() == "downloadTableWidget") {
            tw = dynamic_cast<QTableWidget*>(w);
            break;
        }
    }

    int row = 1;
    bool warningWrongFlash = false;

    for(std::list<ROM_INFO>::const_iterator rom_it = rom_list.begin();
        rom_it != rom_list.end(); ++rom_it)
    {
        const PART_INFO *pmt =
                proxy.read_pmt(rom_it->name);

	bool partitionSelected = false;
        if (tw) {
            QTableWidgetItem *twi = tw->item(row, 0);
            if (twi) {
                if (pmt)
                    qDebug() << "Partition Name" << QString::fromLatin1(rom_it->name) << twi->checkState() << QString::fromLatin1(pmt->name);
                else
                    qDebug() << "Partition Name" << rom_it->name << twi->checkState() << "MISSING";

                if (twi->checkState() != 0 && warningWrongFlash)
                    return true;

                if (!pmt)
                    warningWrongFlash = true;

                if (twi->checkState())
                    partitionSelected = true;
            }
            else
                qDebug() << "Done";
        }

        if (rom_it->item_is_visable)
            row++;

        if (pmt != NULL && pmt_is_changed(pmt, *rom_it))
        {

            if((strstr(rom_it->name, "BMTPOOL")||strstr("BMTPOOL", rom_it->name))
                    && ((rom_it->begin_addr&0xFFFF)== 0))
            {
                continue;
            }
            else
            {
                qDebug("PMT changed for <%s>: addr<0x%llx>-->addr<0x%llx>, len<0x%llx>-->len<0x%llx>",
                         rom_it->name, pmt->begin_addr, rom_it->begin_addr, pmt->image_length, rom_it->partition_size);
                LOGD("PMT changed for <%s>: addr<0x%llx>-->addr<0x%llx>, len<0x%llx>-->len<0x%llx>",
                         rom_it->name, pmt->begin_addr, rom_it->begin_addr, pmt->image_length, rom_it->partition_size);

                if (pmt->begin_addr != rom_it->begin_addr && partitionSelected) {
                    qDebug() << "Starting offset differs - cannot proceed";
                    return true;
                }
            }
        }

    }

    return false;
}

void DLOnlyCommand::exec(const QSharedPointer<Connection> &conn)
{
    conn->ConnectDA();

    if(CheckPMTLayoutChange(conn) == true)
    {
        fw_throw_error(ERROR_ROM_MUST_ENABLE);
    }

    if(NULL != dl_setting_)
        dl_setting_->CreateCommand(conn->ap_key())->exec(conn);
}
}
