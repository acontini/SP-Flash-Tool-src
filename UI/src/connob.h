#ifndef CONNOB_H
#define CONNOB_H

#include "../../Conn/Connection.h"
#include "MainController.h"

namespace APCore
{
class ConnOb : public ConnObserver
{
public:
    ConnOb(MainController *control);
    ~ConnOb();

    virtual void OnBromConnected(const BOOT_RESULT &, const std::string &);
    virtual void OnDAConnected(const DA_REPORT_T &, const std::string &, const int);

private:
    MainController *control_;
};

}

#endif // CONNOB_H
