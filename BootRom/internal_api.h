#ifndef __INTERNAL_API_H__
#define __INTERNAL_API_H__

#ifdef _WIN32 /*windows -------------------------------------------------------*/

#include <windows.h>
    #define LIBEXPORT __declspec(dllexport)
#else /*linux ----------------------------------------------------------------*/
#if __GNUC__ >= 4
#define LIBEXPORT __attribute__ ((visibility ("default")))
#else
#error  "the compiler is not support!!"
#endif

#endif

LIBEXPORT bool __stdcall FlashTool_Chip_isOldArch(const char* chipname);

#endif
