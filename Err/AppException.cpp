/*
 * AppException.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#include "AppException.h"
#include "ErrorLookup.h"

AppException::AppException(
        int err_code,
        const std::string& message,
        const char* func, const char* file, int line)
    :BaseException(func,file,line),
      err_msg_(message),
      err_code_(err_code)
{

}

AppException::~AppException() throw()
{
}

std::string AppException::err_msg() const
{
    if(err_msg_.length() <= 0)
    {
        return ErrorLookup::FlashToolErrorMessage(static_cast<FLASH_TOOL_RESULT>(err_code_));
    }

    return err_msg_;
}



