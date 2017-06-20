#include "connob.h"

#include "../../BootRom/flashtool.h"

namespace APCore
{
ConnOb::ConnOb(MainController *control):
    control_(control)
{
}

ConnOb::~ConnOb()
{

}

void ConnOb::OnBromConnected(const BOOT_RESULT &boot_result,
                             const std::string &friendly_name)
{
    control_->get_brom_result(boot_result, friendly_name);
}

void ConnOb::OnDAConnected(const DA_REPORT_T &da_report,
                           const std::string &friendly_name,
                           const int usb_status)
{
    control_->get_da_report(da_report, friendly_name, usb_status);
}

}
