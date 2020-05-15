/*
 * BromException.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#include "BromException.h"
#include "ErrorLookup.h"

BromException::BromException(int err_code,unsigned int msp_code,
        const char* func, const char* file, int line)
        :BaseException(func,file,line), err_code_(err_code),msp_code_(msp_code)
{
}
BromException::BromException(const BromException& e):
    BaseException("", "", 0)
{
    this->err_code_ = e.err_code_;
    this->msp_code_ = e.msp_code_;
}
BromException::~BromException() throw()
{
}

std::string BromException::err_msg() const
{
    char buff[512] = {0};
    switch(err_code_)
    {
    case S_STORAGE_NOT_MATCH:
    case S_CHIP_TYPE_NOT_MATCH:
    case STATUS_STORAGE_MISMATCH:
    case STATUS_SCATTER_HW_CHIP_ID_MISMATCH:
    case S_DL_LOAD_REGION_IS_OVERLAP:
    case S_DL_SCAT_IS_DOWNLOAD_PROP_ERR:
        FlashTool_GetLastErrorMessage(NULL, buff);
        return std::string(buff);
    default:
        return ErrorLookup::BromErrorMessage(static_cast<STATUS_E>(err_code_),msp_code_);
    }
}



