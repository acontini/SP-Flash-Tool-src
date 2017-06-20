#include "../Inc/RuntimeMemory.h"
#include <crtdbg.h>
#ifdef _DEBUG
#define new MYDEBUG_NEW
#endif

void EnableMemLeakCheck()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
}

void DumpMemoryLeaks()
{
    _CrtDumpMemoryLeaks();
    _CrtMemDumpAllObjectsSince(NULL);
}

