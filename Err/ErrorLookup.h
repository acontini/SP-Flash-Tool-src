/*
 * ErrorLookup.h
 *
 *  Created on: Jan 6, 2011
 *      Author: MTK81019
 */

#ifndef ERRORLOOKUP_H_
#define ERRORLOOKUP_H_

#include <iostream>

#include "../BootRom/flashtool.h"
#include "FlashToolErrorCodeDef.h"

#define ENUM_TO_STRING(case_id) \
    case case_id:\
        return #case_id;

class ErrorLookup{
public:
    static std::string BromErrorMessage(STATUS_E error_code, unsigned int msp_code);
    static std::string FlashToolErrorMessage(FLASH_TOOL_RESULT error_code);
    static std::string FTStatusToString(FLASH_TOOL_RESULT err_code);
private:
    ErrorLookup();
};

#endif /* ERRORLOOKUP_H_ */
