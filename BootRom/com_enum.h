/*******************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2003
 *
 ******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  com_enum.h
 *
 * Project:
 * --------
 *  BootRom Library
 *
 * Description:
 * ------------
 *  Enumerate serial ports.
 *
 * Author:
 * -------
 *  Amos Hsu (mtk00539)
 *
 *******************************************************************************/
 
#ifndef _COM_ENUM_H_
#define _COM_ENUM_H_

#include "host.h"

#define	COM_ENUM_OK						0
#define	COM_ENUM_INVALID_ARGUMENT		1
#define	COM_ENUM_NO_COM_PORT_FOUND		2
#define	COM_ENUM_COM_ARRAY_TOO_SMALL	3
#define	COM_ENUM_ERROR					4
#define C_MAX_COM_NAME_LEN              260

typedef enum {
	COM_ENUM_BY_CREATEFILE = 0,
	COM_ENUM_BY_REGISTRY,
} COM_ENUM_METHOD;

typedef struct {
    char name[C_MAX_COM_NAME_LEN];
} COM_ENUM_PORT;

#define C_MAX_PREFIX_NUM 8
typedef struct {
    COM_ENUM_METHOD method;
    char *prefix[C_MAX_PREFIX_NUM]; 
    bool bUSBOnly;
} COM_ENUM_ARG;


typedef enum {
    WHITE_LIST = 0,
    BLACK_LIST
} FILTER_TYPE_E;

typedef struct 
{
    unsigned int m_uCount;
    FILTER_TYPE_E m_eType;
    char ** m_ppFilterID;
	bool *m_bInterface;
} COM_FILTER_LIST_S;

typedef struct
{
    int m_iFilterIndex;
    unsigned int m_uNumber;
    char m_rFriendly[512];
    char m_rInstanceID[512];
    char m_rSymbolic[512];
    bool m_bInterface;
} COM_PROPERTY_S;



#ifdef	__cplusplus
extern "C" {
#endif

LIBEXPORT int __stdcall ComPortEnumerate(unsigned short *p_com_port_array, unsigned short *p_com_port_count);
LIBEXPORT int __stdcall ComPortEnumerateWithPrefix(unsigned short *p_com_port_array, unsigned short *p_com_port_count, const char *registryValuePrefix);
LIBEXPORT int __stdcall ComPortEnumerateEx(COM_ENUM_METHOD  method, unsigned short *p_com_port_array, unsigned short *p_com_port_count);
LIBEXPORT int __stdcall UsbComPortEnumerate(unsigned short *p_com_port_array, unsigned short *p_com_port_count);
LIBEXPORT int __stdcall ComPortEnumerateByArg(COM_ENUM_ARG *p_arg, COM_ENUM_PORT* p_com_port_array, unsigned short *p_com_port_count);

//SP new APIs for searching COM port
LIBEXPORT int __stdcall GetCurrentCOMPortNumberWithFilter(COM_FILTER_LIST_S * pCOMFilter, const void * pGuid, bool bDeviceInterface, int * pNum);
LIBEXPORT int __stdcall GetCurrentCOMPortInfoWithFilter(COM_FILTER_LIST_S * pCOMFilter, const void * pGuid, bool bDeviceInterface, COM_PROPERTY_S * pCOMPorperty, int * pNum);
LIBEXPORT int __stdcall GetIncrementCOMPortWithFilter(COM_FILTER_LIST_S * pCOMFilter, COM_PROPERTY_S * pCOMPorperty, const void * pGuid, bool bDeviceInterface, int * pStopFlag, double dTimeout);
LIBEXPORT void __stdcall GetAllInterfaceInfo(const void * pGuid, bool bEnableDriverInfo);
LIBEXPORT int __stdcall GetCOMPortWithFilter(COM_FILTER_LIST_S * pCOMFilter, COM_PROPERTY_S * pCOMPorperty, int * pStopFlag, double dTimeout);
LIBEXPORT int __stdcall GetSpecialCOMPortWithFilter(COM_FILTER_LIST_S * pCOMFilter, int dPreferComPort, COM_PROPERTY_S * pCOMPorperty,
								   int * pStopFlag, double dTimeout);
#ifdef	__cplusplus
}
#endif

#endif
