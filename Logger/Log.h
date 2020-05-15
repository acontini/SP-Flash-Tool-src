/*
 * Log.h
 *
 *  Created on: Aug 27, 2011
 *      Author: MTK81019
 */

#ifndef LOG_H_
#define LOG_H_

#include "Logger.h"
#include <QDebug>

#ifdef _DEBUG
#define USE_QTDEBUG
#endif

#ifdef USE_QTDEBUG

class QDebugLogFunc{
public:
    const char * m_func;
    const char * m_file;
    int m_line;

    QDebugLogFunc(const char * func, const char * file, int line)
    {
        m_func = func;
        m_file = file;
        m_line = line;
    }
    void operator ()(const char * format,...);
};

#define LOG (QDebugLogFunc(__FUNCTION__, __FILE__, __LINE__))
#define LOGV (QDebugLogFunc(__FUNCTION__, __FILE__, __LINE__))
#define LOGD (QDebugLogFunc(__FUNCTION__, __FILE__, __LINE__))
#define LOGI (QDebugLogFunc(__FUNCTION__, __FILE__, __LINE__))
#define LOGW (QDebugLogFunc(__FUNCTION__, __FILE__, __LINE__))
#define LOGE (QDebugLogFunc(__FUNCTION__, __FILE__, __LINE__))
#else
#define LOG (LogFunc(Logger::Debug, __FUNCTION__, __FILE__, __LINE__))
#define LOGV (LogFunc(Logger::Verbose, __FUNCTION__, __FILE__, __LINE__))
#define LOGD (LogFunc(Logger::Debug, __FUNCTION__, __FILE__, __LINE__))
#define LOGI (LogFunc(Logger::Info, __FUNCTION__, __FILE__, __LINE__))
#define LOGW (LogFunc(Logger::Warn, __FUNCTION__, __FILE__, __LINE__))
#define LOGE (LogFunc(Logger::Error, __FUNCTION__, __FILE__, __LINE__))
#endif


#endif /* LOG_H_ */
