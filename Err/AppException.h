/*
 * AppException.h
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#ifndef APPEXCEPTION_H_
#define APPEXCEPTION_H_

#include "BaseException.h"

#include <iostream>

class AppException : public BaseException
{
public:
    AppException(int err_code, const std::string& message, const char* func="", const char* file="", int line=0);
    virtual ~AppException() throw();

    virtual std::string err_msg() const;
    int err_code() const { return err_code_; }

private:
    std::string err_msg_;
    int err_code_;
};

#endif /* APPEXCEPTION_H_ */
