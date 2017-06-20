#ifndef SUPER_HANDLE_H
#define SUPER_HANDLE_H

#include <stdio.h>

namespace APCore {

template <typename _Tx>
class SuperHandle
{
public:
    typedef _Tx HANDLE_T;

public:
    SuperHandle() : handle_t_(NULL)
    {
    }

    HANDLE_T handle_t() const
    {
        return (NULL==this) ? NULL : handle_t_;
    }

protected:
    HANDLE_T handle_t_;
};

}
#endif // SUPER_HANDLE_H
