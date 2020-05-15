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
 *  mtk_mcu.h
 *
 * Project:
 * --------
 *  BootRom Library
 *
 * Description:
 * ------------
 *  The definition of MediaTek chip types and external clock arguments.
 *
 * Author:
 * -------
 *  Amos Hsu (mtk00539)
 *
 *******************************************************************************/
 
#ifndef	_MTK_MCU_H_
#define	_MTK_MCU_H_

typedef enum {
	EXT_13M = 1,
	EXT_26M = 2,
	EXT_39M = 3,
	EXT_52M = 4,
	EXT_CLOCK_END,
	AUTO_DETECT_EXT_CLOCK = 254,
	UNKNOWN_EXT_CLOCK = 255
} EXT_CLOCK;

typedef	enum {
	MT6205	= 0,
	MT6205B	= 1,
	MT6218	= 2,
	MT6218B	= 4,
	MT6219  = 5,
	MT6217  = 6,
	MT6228  = 7,
	MT6227  = 8,
	MT6229  = 9,
	MT6226  = 10,
	MT6226M  = 11,
	MT6230  = 12,
	MT6225  = 13,
	MT6268T  = 14,
	MT6223  = 15,
	MT6227D  = 16,
	MT6226D  = 17,
	MT6223P  = 18,
	MT6238	= 19,
	MT6235	= 20,
	MT6235B	= 21,
	TK6516_MD  = 22,
	TK6516_AP  = 23,
	MT6268A	= 24,
	MT6516_MD  = 25,
	MT6516_AP  = 26,
	MT6239	= 27,
	MT6251T	= 28,
	MT6253T  = 29,
	MT6268B  = 30,
	MT6253  = 32,
	MT6253D  = 33,
	MT6236  = 34,
	MT6270A  = 35,

	MT6276  = 128,
	MT6251  = 129,
	MT6255  = 130,
	MT6573  = 131,
	MT6575  = 132,
	MT6577  = 133,
	MT6589  = 134,
	MT6582  = 135,
    MT6572  = 136,
	MT8135  = 137,
	MT6592  = 138,
	MT6571  = 139,
	MT6595  = 140,
    MT8127  = 141,
    MT8173  = 142,
    MT6752  = 143,
    MT2601  = 144,
	MT8590	= 145,
	MT7623  = 146,
    MT7683  = 147,
	MT8591  = 148,
	MT8592  = 149,
	MT8531  = 150,
	MT7863  = 151,
	MT2701  = 152,
	MT8521  = 153,	
	MT6795  = 154,
	MT6574  = MT6582,
	MT6735  = 155,
    MT6580  = 156,
	MT6735M = 157,
	MT6753  = 158,
	MT6737T = 159,
	MT6737M = 160,	
	MT8163  = 161,

	//follow is new arch chip, if add old arch chip, please add after MT8163 and before MT6755 !!!
	MT6755 = 180,
	MT6797 = 181,
	MT6799 = 182,	//Whitney
	MT0571 = 183,
	MT6750 = 184,
	ELBRUS = 185, //MT0598
	MT6757 = 186, //kibo, olympus
	MT6757D = 187, //kibo+
	MT6759 = 188, //Alaska
	MT8167 = 189,
	MT6570 = 190, //rushmore
	MT6763 = 191, //Bianco
	MT6758 = 192, //vinson
	MT6739 = 193, //zion
	MT8695 = 194,
	MT0713 = 195, //Cannon
	BBCHIP_TYPE_END,
	AUTO_DETECT_BBCHIP = 254,
	UNKNOWN_BBCHIP_TYPE = 255
} BBCHIP_TYPE;


// FIXME
typedef BBCHIP_TYPE ChipName;


#endif
