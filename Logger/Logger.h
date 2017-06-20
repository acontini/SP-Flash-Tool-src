/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2009
*
*****************************************************************************/

#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>

class LoggerImpl;

class Logger {
    public:
    enum Level {
        Verbose = 0,
        Debug,
        Info,  //default for console mode
        Warn,
        Error,
        Off
        };

    void SetBaseLevel(Level baselevel);
    void AddLogHandle(const std::string& fileName, Logger::Level level);
    void AddLogHandle(std::ostream* stdStream, Logger::Level level);
    void RemoveAllLogHandle();


    static void SetSPFlashToolLogFolder();
    static std::string GetSPFlashToolLogFolder();
    static void SetSPFlashToolDumpFileFolder();
    static std::string GetSPFlashToolDumpFileFolder();

    std::string DumpFileWithTimeStamp(void) const;

    std::string sp_flash_tool_log_path;
    std::string dump_files_path;

    std::string ContextInfo() const; //get time,logger name, pid, tid
    void Log(Level level,const char *pattern, ...);

    Logger& operator<<(Level curlevel);

    Logger& operator<< (bool val );
    Logger& operator<< (short val );
    Logger& operator<< (unsigned short val );
    Logger& operator<< (int val );
    Logger& operator<< (unsigned int val );
    Logger& operator<< (long val );
    Logger& operator<< (unsigned long val );
    Logger& operator<< (float val );
    Logger& operator<< (double val );
    Logger& operator<< (long double val );
    Logger& operator<< (const void* val );
    Logger& operator<< (char val);
    Logger& operator<< (const char* val);
    Logger& operator<< (const wchar_t* val);
    Logger& operator<< (std::streambuf* sb );
    Logger& operator<< (const std::string& sb );

    Logger& operator<<(std::ostream& (*pf)(std::ostream&));
    Logger& operator<< (std::ios& ( *pf )(std::ios&));
    Logger& operator<<(std::ios_base& (*pf)(std::ios_base&));

    ~Logger();
private:
    LoggerImpl *pimpl;
    Logger(const std::string& loggerName, Level baseLevel = Warn);

public:
    static Logger& GetLogger();
    static void DebugOn();
    static void DebugOff();
};

class LogFunc{
public:
    Logger::Level m_level;
    const char * m_func;
    const char * m_file;
    int m_line;

    LogFunc(Logger::Level level, const char * func, const char * file,int line)
    {
        m_level = level;
        m_func = func;
        m_file = file;
        m_line = line;
    }
    void operator ()(const char * format,...);
};

//#define LOGX (LogFunc(Logger::Debug, __FUNC__, __FILE__, __LINE__))

#endif // _LOGGER_H
