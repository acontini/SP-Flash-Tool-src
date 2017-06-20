#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "Core/Common.h"

template<typename T>
class Concrete_HandleManager
{
public:
    typedef int  (__stdcall *HandleCreator)(T handle);
    typedef int  (__stdcall *HandleDestoryer)(T handle);

    explicit Concrete_HandleManager(T handle, HandleCreator creator, HandleDestoryer destoryer, int success_flag)
        : handle_(handle),
          creator_(creator),
          destoryer_(destoryer),
          success_flag_(success_flag) {
        CreateHandle();
    }
    ~Concrete_HandleManager() {
        DestroyHandle();
    }

    bool is_handle_ready() const {
        return is_handle_ready_;
    }

private:
    int CreateHandle() {
        int ret = creator_(handle_);
        bool is_ready = (success_flag_ == ret);
        set_is_handle_ready(is_ready);
//        LOG("Concrete_HandleManager(): CreateHandle(): is_handle_ready_(%s).",
//            (is_handle_ready() ? "Yes" : "No"));
        return ret;
    }
    int DestroyHandle() {
        int ret = -1;
        if( is_handle_ready() ) {
            ret = destoryer_(handle_);
            bool is_destory = (success_flag_ == ret);
//            LOG("Concrete_HandleManager(): DestroyHandle() is %s.",
//                is_destory ? "sucess" : "failure");
        } else {
//            LOG("Concrete_HandleManager(): DestroyHandle() falied!");
        }
        return ret;
    }

    void set_is_handle_ready(bool is_ready) {
        this->is_handle_ready_ = is_ready;
    }

    DISALLOW_COPY_AND_ASSIGN(Concrete_HandleManager);

private:
    const T handle_;
    const int success_flag_;
    HandleCreator creator_;
    HandleDestoryer destoryer_;
    bool is_handle_ready_;
};


#endif // HANDLERMANAGER_H
