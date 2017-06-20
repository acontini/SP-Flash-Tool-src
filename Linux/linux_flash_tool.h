#ifndef    _LINUX_FLASH_TOOL_H_
#define    _LINUX_FLASH_TOOL_H_
#include "WINDEF.H"
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <inttypes.h>
#define __stdcall
#define __fastcall
typedef int far            *LPOVERLAPPED;
typedef const char         *LPCWSTR;  // TODO
typedef HANDLE             HMODULE;
typedef HANDLE             HWND;
typedef char               WCHAR;  // TODO
typedef char               TCHAR;  // TODO

#define CBR_2400 B2400
#define CBR_4800 B4800
#define CBR_9600 B9600
#define CBR_14400 B19200   // no 14400 in linux
#define CBR_19200 B19200
#define CBR_57600 B57600
#define CBR_115200 B115200
#define CBR_230400 B230400

#define GetLastError() errno

// for dwDesiredAccess
const DWORD GENERIC_READ = 1;
const DWORD GENERIC_WRITE = 2;
// dwShareMode
const DWORD FILE_SHARE_READ = 1;
const DWORD FILE_SHARE_WRITE = 2;
// dwCreationDisposition
const DWORD CREATE_NEW = 1;
const DWORD CREATE_ALWAYS = 2;
const DWORD OPEN_EXISTING = 4;
const DWORD OPEN_ALWAYS = 8;
const DWORD TRUNCATE_EXISTING = 16;
// dwFlagsAndAttributes
const DWORD FILE_ATTRIBUTE_NORMAL = 1;

// TODO Need Confrim .
const HANDLE INVALID_HANDLE_VALUE = (HANDLE)-1;
const DWORD INVALID_FILE_SIZE = -1;

// For Seek
const DWORD FILE_BEGIN = 1;
const DWORD FILE_CURRENT = 2;
const DWORD FILE_END = 4;

inline void Sleep(int ms){ ms = ms; };

// FILE OPERATION
HANDLE CreateFile(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPVOID lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    LPVOID hTemplateFile); // FIXME change from HANDLE to LPVOID, avoid warnings about NULL.

BOOL WriteFile(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped);

BOOL ReadFile(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped);

BOOL CloseHandle(HANDLE hObject);
DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);

DWORD SetFilePointer(
    HANDLE hFile,
    LONG lDistanceToMove,
    LPLONG lpDistanceToMoveHigh,
    DWORD dwMoveMethod);

#endif
