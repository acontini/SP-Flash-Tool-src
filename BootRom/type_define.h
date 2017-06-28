#ifndef	_TYPE_DEFINE_H_
#define	_TYPE_DEFINE_H_

#ifndef UINT32_STYLE_DEFINE
#define UINT32_STYLE_DEFINE
   typedef unsigned char         uint8;
   typedef signed char           int8;
   typedef unsigned short        uint16;
   typedef signed short          int16;
   typedef unsigned int          uint32;
   typedef signed int            int32;
#endif

typedef char                  char8;
typedef short                 char16;
typedef int                   char32;

typedef unsigned long         ulong32;
typedef long                  long32;
typedef unsigned long long    uint64;
typedef signed long long      int64;
typedef void*                 pvoid;

#ifndef BOOL
typedef int                   BOOL;
#endif
typedef int                   status_t;

#ifndef TRUE
   #define TRUE 1
   #define FALSE 0
#endif

typedef long HSESSION;
#define INVALID_HSESSION_VALUE 0

#endif
