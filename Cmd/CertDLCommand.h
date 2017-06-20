#ifndef CERTDLCOMMAND_H
#define CERTDLCOMMAND_H
#include "ICommand.h"
#include "../BootRom/brom.h"

namespace APCore
{

class CertDLCommand : public ICommand
{
public:
    CertDLCommand(APKey key);
    virtual ~CertDLCommand();

    virtual void exec(const QSharedPointer<Connection>&conn);

private:
    void ArgFlashToolDLCert(SCERT_ARG_S &cdl_arg, void* m_cb_in_brom_state_arg);

public:
    static int __stdcall cb_in_brom_stage(BROM_HANDLE_T brom_handle,
                                          HANDLE hCom, void *usr_arg);

    static int __stdcall cb_com_init_stage(HANDLE hCom, void *usr_arg);
};
}

#endif // CERTDLCOMMAND_H
