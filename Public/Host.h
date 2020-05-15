/*
 * Host.h
 *
 *  Created on: Aug 26, 2011
 *      Author: MTK81019
 */

#ifndef HOST_H_
#define HOST_H_

typedef int APKey; //the only identification for GUI.

const int AP_STOP = 9876;


#ifdef _WIN32
#define DEFAULT_LOG_FILE "\\QT_FLASH_TOOL.log"
#else
#define DEFAULT_LOG_FILE "/QT_FLASH_TOOL.log"
#endif


#endif /* HOST_H_ */
