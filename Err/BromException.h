/*
 * BromException.h
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#ifndef BROMEXCEPTION_H_
#define BROMEXCEPTION_H_

#include "../BootRom/flashtool.h"
#include "BaseException.h"

#include <iostream>

class BromException : public BaseException
{
public:
    BromException(int err_code,unsigned int msp_code=0,
            const char* func="", const char* file="", const int line=0);
    BromException(const BromException& e);
    virtual ~BromException() throw();

    int err_code() const { return err_code_; }
    virtual std::string err_msg() const;

protected:
    int err_code_;
    unsigned int msp_code_;
};

#endif /* BROMEXCEPTION_H_ */
