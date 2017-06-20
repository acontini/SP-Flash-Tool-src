#ifndef _RUNTIME_MEMORY_H
#define _RUNTIME_MEMORY_H

#ifdef _WIN32
    #ifdef _DEBUG
    #pragma warning(disable: 4067)
    #pragma warning(disable: 4005)

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

//        #ifndef DEBUG_NEW
//            #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//            #define new DEBUG_NEW
//        #endif

    #endif
#endif

void EnableMemLeakCheck();
void DumpMemoryLeaks();

#endif
