/*
 * Exception.h
 *
 *  Created on: Sep 2, 2011
 *      Author: MTK81019
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include "AppException.h"
#include "BromException.h"
#include "UserStopException.h"

#define THROW_APP_EXCEPTION(x, y) \
                do{ throw AppException(x, y,  __FUNCTION__, __FILE__, __LINE__); } while(0);

#define THROW_BROM_EXCEPTION(x, y) \
        do{ throw BromException(x, y, __FUNCTION__, __FILE__, __LINE__); } while(0);

#define THROW_BROM_EXCEPTION_EX(e) \
    do{ throw BromException(e); } while(0);

#define THROW_USER_STOP \
        do{ throw UserStopException(__FUNCTION__, __FILE__, __LINE__); } while(0);


#endif /* EXCEPTION_H_ */
