/*
 * Log.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: MTK81019
 */

#include "Log.h"

#ifdef USE_QTDEBUG

#include <iostream>
#include "../Utility/Utils.h"

#ifdef _WIN32
static const char kSep= '\\';
#else
static const char kSep= '/';
#endif

void QDebugLogFunc::operator()(const char * format,...)
{
    std::string log_str(format);
    va_list params;
	static char msg[1024] = {0};
#ifndef _WIN32
    log_str = Utils::ReplaceAllSubstring(log_str, WIN_HEX_FORMAT, LINUX_HEX_FORMAT);
#endif
    va_start( params, format );
    vsnprintf( msg, 1020, log_str.c_str(), params );
    va_end( params );
    std::string whole_name(m_file);
    std::string::size_type index = whole_name.find_last_of(kSep);
    if(index != whole_name.npos)
    {
        m_file += index+1;
    }
#ifdef _WIN32
    qDebug()<<m_func<<"():"<<msg<<"("<<m_file<<","<<m_line<<")";
#endif
    //also write to the log file.
    Logger::GetLogger()<<Logger::GetLogger().ContextInfo()
            <<m_func<<"():"
            <<msg<<"("<<m_file<<","<<m_line<<")"<<std::endl;
}
#endif
