
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : axisanm.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

//
//	AXIS alert notice message format
//

#pragma once
#pragma pack(1)
struct	_anmH {
	unsigned char	anmK{};		// the kind of alert msg
	unsigned char	anmF{};		//
	char	datL[5]{};		// data length
};

#define	L_anmH	sizeof(struct _anmH)

//
//	_anmH.anmK ... UDP alert
//
#define	anmK_ALIVE	0x01		// BP alive notice
					// anmH + ips
#define	anmK_PUSH	0x02		// push real-time data


struct	_rtmH {
	unsigned char	rtmK{};		// the kind of RTM
	unsigned char	stat{};		// stat
	unsigned char	datK{};		// the kind of RTM data
	char	datL[3]{};		// data length
};

#define	L_rtmH	sizeof(struct _rtmH)

//
//	_rtmH.rtmK
//
#define	rtmK_INFO	'I'		// RTM information
#define	rtmK_DATA	'D'		// RTM data

//
//	_rtmH.stat
//
#define	stat_TICK	0x0f		// index data (KOSPI, KOSDAQ, K200, FUTURE, etc)
#define	stat_SCR	0x10		// scroll data (grid, graph)
#define	stat_SET	0x20		// default set
#define	stat_DEIN	0x40		// tick delete/insert
#pragma pack()