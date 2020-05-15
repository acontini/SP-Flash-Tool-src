/*
 * DAHandle.h
 *
 *  Created on: Aug 26, 2011
 *      Author: MTK81019
 */

#ifndef DAHANDLE_H_
#define DAHANDLE_H_

#include "../../BootRom/flashtool_handle.h"
#include "SuperHandle.h"
#include <string>

namespace APCore {

class DAHandle : public SuperHandle<DA_HANDLE_T>
{
public:
    DAHandle();
    ~DAHandle();

    void LoadDA(const std::string &da_file);

    void GetDAInfo(DA_INFO *da_info);

    bool IsReady();
};

} /* namespace APCore */
#endif /* DAHANDLE_H_ */
