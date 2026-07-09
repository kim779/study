 //*******************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2003
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : appl.h	application common header
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-05	Initial version
//		 02.00  2008-02 Upgrade version (for EU2S)
//		 02.10	2009-06 Customize version (for KATS)
//*****************************************************************************
#pragma once

//
// confirm dialog : type option
//
#define	CFD_EDIT	0x00000001		// edit enable(1개인 경우)
#define	CFD_FUND	0x00000002		// fund 주문

#define	CFD_ONE		0x00000010		// 1개 주문
#define	CFD_TWO		0x00000020		// 2개 주문
#define	CFD_THREE	0x00000040		// 3개 주문
#define	CFD_OTHER	0x00000080		// 4개 이상
#define	CFD_FOUR	0x00000100		// 4개 주문
#define	CFD_HTWO	0x00000200		// 4개 주문

#define	CFD_LTPOS	0x000001000		// left top
#define	CFD_MTPOS	0x000002000		// middle top
#define	CFD_RTPOS	0x000004000		// right top
#define	CFD_LBPOS	0x000008000		// left bottom
#define	CFD_MBPOS	0x000010000		// middle bottom
#define	CFD_RBPOS	0x000020000		// right bottom
#define	CFD_WCPOS	0x000040000		// window center
#define	CFD_SCPOS	0x000080000		// screen center
#define	CFD_USPOS	0x000100000		// user define pos : CRect(x, y, 0, 0)

//
// childframe message (to m_parent->GetParent())
//
#define	WM_AXCHILD	(WM_USER + 500)

//
// message : WM_AXCHILD, LOWORD(wparam)
//
#define	apROOTDIR	0x51		// char* return : root 
#define	apLOGINID	0x52		// char* return : loginID
#define	apLOGINIDx	0x53		// char* return : loginID|desc
#define	apGUIDE1	0x54		// lparam : message
#define	apGUIDE2	0x55		// lparam : message, HIWORD(wparam) : level(1, 2, 3)
					//                                    black, red, blue
#define	apGUIDE3	0x56		// lparam : message, HIWORD(wparam) : level(1, 2, 3)
#define	apFIXEDA	0x57		// lparam : LOWORD(lparam) = width, HIWORD(lparam) = height
#define	apFIXEDW	0x58
#define	apFIXEDH	0x59
#define	apHOOKED	0x60		// HIWORD(wparam) 1: hooked

#define apPINCH		0x61		// 
#define	apSETCONFIG	0x62		//
#define apSETTG		0x63



// 환경설정 INI
#define	CONFIGINI	"config.ini"	// config file
#define	ORDER		"ORDER"		// SECTION:주문
#define	CONFIRM		"CONFIRM"	// key 주문확인창 1:TRUE, 0:FALSE
#define	PRICE		"PRICE"		// key 0:상대호가, 1:현재가, 2:사용안함
#define	DISPLAY		"DISPLAY"	// SECTION
#define	FLASH		"FLASH"		// key Blink사용여부 1:TREU, 0:FALSE



