/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2004
 *
 ******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  host.h
 *
 * Project:
 * --------
 *  Flash Download/Readback/Format Library.
 *
 * Description:
 * ------------
 *  host dependent declaration
 *
 * Author:
 * -------
 *  
 *
 *******************************************************************************/
#ifndef	_HOST_H_
#define	_HOST_H_

/*******************************************************************************
 * data type
 *******************************************************************************/
typedef unsigned char       U8;
typedef signed char         S8;
typedef unsigned short      U16;
typedef signed short        S16;
typedef unsigned int        U32;
typedef signed int          S32;
typedef unsigned char*      P_U8;
typedef unsigned short*     P_U16;
typedef unsigned int*       P_U32;

#ifndef UINT32_STYLE_DEFINE
#define UINT32_STYLE_DEFINE
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
#endif

typedef signed int          intx;
typedef unsigned int        uintx;

#define MAX_UINT32 0xFFFFFFFF
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF

#define UNUSED(x)    (void)x

#ifdef _WIN32 /*windows -------------------------------------------------------*/

#include <windows.h>

typedef unsigned int BROM_HANDLE_T;
typedef unsigned __int64  U64;      //!<: VC6 can not recognize long long as 64bit 
typedef signed   __int64  S64;      //!<: VC6 can not recognize long long as 64bit 
typedef unsigned __int64* p_U64;    //!<: VC6 can not recognize long long as 64bit 
typedef unsigned __int64  uint64;   //!<: VC6 can not recognize long long as 64bit 

//#define META_SPEC_OUTPUT

#if ! defined(META_SPEC_OUTPUT)
    #define LIBEXPORT __declspec(dllexport)
#else
    #define LIBEXPORT
#endif

//#define LIBEXPORT
#define C_SEP_CHR   '\\'
#define C_SEP_STR   "\\"

typedef ULONG THR_RET;

#ifdef _CRT_DEPRECATE_TEXT
#define _DEPRECATED_API LIBEXPORT _CRT_DEPRECATE_TEXT("The function is deprecated, and not suggested to use.")
#else
#define _DEPRECATED_API LIBEXPORT
#endif


#else /*linux ----------------------------------------------------------------*/

#if __GNUC__ >= 4
#define LIBEXPORT __attribute__ ((visibility ("default")))
#else
#error  "the compiler is not support!!"
#endif

#define __stdcall
#define __cdecl
#define FAR
#define HINSTANCE unsigned int *
#define FreeLibrary(HINSTANCE)
#define LoadLibrary(dll_name) NULL
#define GetProcAddress(HINSTANCE,str) NULL

#define C_SEP_CHR   '/'
#define C_SEP_STR   "/"
typedef unsigned long       BROM_HANDLE_T;
typedef unsigned int        DWORD;  //!<: MSDN: unsigned 32-bit integer, The range is 0 through 4294967295 decimal.
                                    //!<:       it should be defined as unsigned int instead of unsigned long
typedef unsigned short      WORD;   //!<: MSDN: unsigned 16-bit integer
typedef unsigned int        UINT;   //!<: MSDN: An unsigned INT. The range is 0 through 4294967295 decimal.
typedef unsigned char       BYTE;   //!<: MSDN: A byte (8 bits).
typedef void *              HANDLE;
typedef HANDLE              HWND;

typedef unsigned long long  U64;
typedef signed long long    S64;
typedef unsigned long long* p_U64;
typedef unsigned long long  uint64;

#ifndef _countof
#define _countof(ary) (sizeof(ary)/sizeof(ary[0]))
#endif

#define _DEPRECATED_API LIBEXPORT __attribute__((deprecated))

#endif

#ifdef ENABLE_64BIT_PROTOCOL
typedef U64 UPTR;
#else
typedef U32 UPTR;
#endif

/*******************************************************************************
 * function declaration
 *******************************************************************************/
#endif
