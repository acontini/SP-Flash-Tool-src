#include "ResourceManager.h"
#include "../Host/Inc/RuntimeMemory.h"

#include "CoreHandle.h"
#include <QtGlobal>

#if 0
//a way of hash to generate integer according to the string
static int HashByString(const std::string & str)
{
    int key = 0;
    const char *buffer = str.c_str();
    unsigned int len = str.length();

    const unsigned int int_size = len/sizeof(int);
    int *int_ptr = (int*)(buffer);

    for(int i=0; i< int_size; i++)
    {
        key ^= int_ptr[i];
    }

    //convert remained bytes to int
    unsigned int int_rem  = len%sizeof(int);
    char temp[sizeof(int)] = {0};

    memcpy(temp, buffer+int_size*sizeof(int), int_rem);
    int *int_ptr2 = (int*)(temp);
    key ^= *int_ptr2;

    return key;
}
#endif

static int GenKey()
{
    //TODO: generate a session id.
    return qrand();
}

namespace APCore
{

QSharedPointer<ResourceManager> ResourceManager::instance_;

ResourceManager *ResourceManager::instance()
{
    if(instance_ == NULL)
    {
        instance_ = QSharedPointer<ResourceManager>(new ResourceManager());
    }
    return instance_.data();
}

ResourceManager::ResourceManager()
{
}

APKey ResourceManager::NewAPKey()
{
    APKey new_key = GenKey();
    handles_map.insert(std::pair<APKey, CoreHandle*>(new_key, new CoreHandle()));
    return new_key;
}

CoreHandle *ResourceManager::GetHandle(const APKey key)
{
    if(handles_map.find(key) != handles_map.end())
    {
        return handles_map[key];
    }
    return NULL;
}

void ResourceManager::FreeHandle(const APKey key)
{
    if(handles_map.find(key) != handles_map.end())
    {
        delete handles_map[key];
        handles_map.erase(key);
    }
}

void ResourceManager::FreeAll()
{
    for(std::map<int, CoreHandle*>::const_iterator it
        = handles_map.begin(); it!= handles_map.end(); ++it)
    {
        delete it->second;
    }
    handles_map.clear();
}


}
