/*
 * BaseException.h
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#ifndef BASEEXCEPTION_H_
#define BASEEXCEPTION_H_

#include <iostream>

class BaseException : public std::exception
{
public:
    BaseException(const char* func, const char* file, int line);
    virtual ~BaseException() throw();

    std::string context() const;

    virtual int err_code() const = 0;
    virtual std::string err_msg() const = 0;

protected:
    const char* func_;
    const char* file_;
    int line_;
};

#endif /* BASEEXCEPTION_H_ */
