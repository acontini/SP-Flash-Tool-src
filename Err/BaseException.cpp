/*
 * BaseException.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#include "BaseException.h"
#include <stdio.h>

BaseException::BaseException(const char* func, const char* file, int line)
    : func_(func), file_(file), line_(line)
{
}

BaseException::~BaseException() throw()
{
}

std::string BaseException::context() const
{
    char buffer[512] = {0};
    sprintf(buffer,"(%s,%s,%d)",func_,file_,line_);
    return std::string(buffer);
}



