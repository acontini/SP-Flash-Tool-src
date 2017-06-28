#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "../Public/Host.h"
#include "../Resource/CoreHandle.h"

#include <map>
#include <qsharedpointer.h>

#define __POOL_OF_HANDLE(key)        \
    (APCore::ResourceManager::instance()->GetHandle(key))
#define _HANDLE_OF_POOL(key, tp)    \
    (__POOL_OF_HANDLE(key)->tp##_handle())

#define GET_RB_HANDLE(key)    \
    _HANDLE_OF_POOL(key, rb)

#define _HANDLE_OF_POOL_T(key, tp)    \
    (_HANDLE_OF_POOL(key, tp)->handle_t())

#define GET_DL_HANDLE_T(key)    \
    _HANDLE_OF_POOL_T(key, dl)
#define GET_DA_HANDLE_T(key)    \
    _HANDLE_OF_POOL_T(key, da)
#define GET_AUTH_HANDLE_T(key)    \
    _HANDLE_OF_POOL_T(key, auth)
#define GET_CERT_HANDLE_T(key)    \
    _HANDLE_OF_POOL_T(key, sc)

namespace APCore
{
class CoreHandle;
class ResourceManager
{
public:
    static ResourceManager *instance();

    APKey NewAPKey();

    CoreHandle* GetHandle(const APKey key);

    void FreeHandle(const APKey key);

    unsigned int GetHandleCount() const { return handles_map.size(); }

    void FreeAll();

private:
    std::map<APKey, CoreHandle*> handles_map;

    //singleton
    ResourceManager();
    static QSharedPointer<ResourceManager> instance_;
};

}

#endif // RESOURCEMANAGER_H
