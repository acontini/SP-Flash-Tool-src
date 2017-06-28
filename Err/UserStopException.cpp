#include "UserStopException.h"



UserStopException::UserStopException(
        const char* func, const char* file, int line)
    :BaseException(func,file,line)
{

}

UserStopException::~UserStopException() throw()
{
}
