
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2005
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : chaser.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2005-06	Initial version
// *****************************************************************************

#define	WM_AXIS		WM_USER+500
#define WM_RECEIVE	WM_USER+200
//
//	void OnTrace(WPARAM wParam, LPARAM lParam)
//		HIWORD(wParam) : DATAs type
//		LOWORD(wParam) : DATAs size
//		lParam : DATAs...
#define	x_STRs			0x00		// STRINGs with '\0'
#define	x_RTMs			0x01		// RTMs with '\0'
#define	x_SNDs			0x02		// SND DATAs
#define	x_RCVs			0x03		// RCV DATAs
#define x_CONs			0x09		// CONs DATAs
//	setting	trace
#define axChaserON	9998	// Chaser ON
#define axChaserOFF	9999	// Chaser OFF

//////////////////////////////////////////////////////////////////////////
/// AXIS Registry Key

#define	AXISKEY		"AXIS"
#define	AXISNAME	"Axis Workstation V03.00"
#define	ENVIRONMENT	"Environment"
#define	WORKSTATION	"Workstation"
#define	INFORMATION	"Information"
#define TRACESETTING	"TraceSetting"

#define	BKCOLOR		"bkcolor"
#define SAMECOLOR	"samecolor"
#define ALLCOLOR	"allcolor"
#define SNRCOLOR	"snrcolor"
#define RTMCOLOR	"rtmcolor"
#define SYSCOLOR	"syscolor"
#define RPTCOLOR	"rptcolor"

#define COLOR_BLACK	RGB(  0,   0,   0)
#define COLOR_RED	RGB(255,   0,   0)
#define COLOR_BLUE	RGB(  0,   0, 255)

#define IDX_ALL		0
#define IDX_SNR		1
#define IDX_RTM		2
#define IDX_SYS		3
#define IDX_RPT		4
#define IDX_MAX		5

#define DEFAULTFONT	"Arial"
#define SAMEFONT	"samefont"
#define ALLFONT		"allfont"
#define SNRFONT		"snrfont"
#define RTMFONT		"rtmfont"
#define SYSFONT		"sysfont"
#define RPTFONT		"rptfont"

#define DEFAULTFONTN	10
#define	ALLSIZE		"allsize"
#define	SNRSIZE		"snrsize"
#define	RTMSIZE		"rtmsize"
#define SYSSIZE		"syssize"
#define RPTSIZE		"prtsize"