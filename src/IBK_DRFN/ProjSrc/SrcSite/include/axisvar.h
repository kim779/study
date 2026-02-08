
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
//  Components   : axisvar.h	AXIS common constant variable
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#pragma once

#define	L_VER		8			// 02.10.00
#define	_AXVER_		"02.1"			// major version
#define	_AXUS_		"US"			// axisus design map

#define	L_TRXC		8			// transaction code
#define	L_MAPN		8			// MAP name
#define	L_SGID		2			// MAP group/S
#define	L_TGID		3			// MAP group/T
#define	L_SELC		4			// MAP slection code

#define	RUNDIR		"exe"
#define TABDIR		"tab"
#define	AXISDIR		"axis"
#define	MAPDIR		"map"
#define	DEVDIR		"dev"
#define	LISTDIR		"list"
#define	HELPDIR		"help"
#define	IMAGEDIR	"image"
#define	MTBLDIR		"mtbl"
#define	DATDIR		"dat"
#define	ROPDIR		"rop"

#define	JNLDIR		"jnl"
#define	USRDIR		"user"
#define	GEXDIR		"gex"

#define SECMSG		"secmsg"
#define	HJCODE		"hjcode.dat"
#define	FJCODE		"fjcode.dat"
#define	OJCODE		"opcode.dat"
#define	CJCODE		"cjcode.dat"
#define	UPCODE		"upcode.dat"
#define	KFCODE		"qjcode.dat"
#define	AXISMSG		"axismsg.dat"
#define	VTCODE		"vtcode.ini"
#define	PJCODE		"pjcode.dat"
#define	ELWCODE		"elwcod.dat"
#define CFCODE		"cfcode.dat"
#define SFCODEDAT	"sfcode.dat"
#define WOJCODE		"wocode.dat"
#define HJCODEx		"hjcode3.dat"
#define MFCODE		"mfcode.dat"
#define MOCODE		"mocode.dat"

//////////////////////////////////////////////////////////////////////////
///	AXIS Registry Key

#define	AXISKEY		"AXIS"
#define	AXISNAME	"Axis Workstation V03.00"
#define	ENVIRONMENT	"Environment"
#define	WORKSTATION	"Workstation"
#define	INFORMATION	"Information"

//------------------------------------------------------//
//		ENVIRONMENT SECTION			//
//------------------------------------------------------//
#define	COPYRIGHT	"CopyRight"		// string
#define	ROOTDIR		"RootDir"		// string
#define	STATUS		"Status"		// dword
#define	stTBIcon	0x00000001		// toolbar preference :	icon
#define	stTBText	0x00000002		//			text
#define	stTBIconText	0x00000003		//			icon + text

#define	stPopupIgnore	0x00000008
#define	stIsIcon	0x00000010		// frame size
#define	stIsMax		0x00000020
#define	stStatusBar	0x00000100		// show status bar
#define	stBlackPrint	0x00001000		// black print
#define	stTrigger	0x00002000		// trigger script run
#define	stConfirm	0x00004000		// order confirm window
#define	stPopup		0x00008000		// popup order window
#define	stPassword	0x00010000		// passwd dialog

#define	stDocBar	0x00010000		// docuemntbar show (docbar)
#define	stViewBar	0x00020000		// viewbar show (orderbar)
#define	stManager	0x00040000		// manager show (treeview)

#define FRAMEPOS	"FramePos"		// string
#define TOOLBAR		"ToolBar_"		// string
#define	USER		"User"			// string
						// "CompanyName | UserName"

#define	WNDINFO		"WindowInfo"		// dword(HI:ViewType, LO:MaxWindows)
#define	szMAP		0x0000
#define	szFIXED		0x0001
#define	szRESIZABLE	0x0002

#define	CHILDPOS	"ChildPos"		// dword
						// (HI:child position, LO:Msg position)
#define	cpDEFAULT	0			// 0 : by Windows
#define	cpTOP		1			// 1 : TOP
#define	cpCENTER	2			// 2 : CENTER
#define	cpMAX		3			// 3 : MAXIMIZE

#define	mpCENTER	0			// 0 : CENTER
#define	mpTOP		1			// 1 : TOP
#define	mpBOTTOM	2			// 2 : BOTTOM

#define	CHOICEKEY	"ChoiceKey"		// dword
#define	CLEARKEY	"ClearKey"		// dword
#define	CYCLECOUNT	"CycleCount"		// dword
#define	TICKERS		"Tickers"		// dword

#define	VERIPS		"VersionIPs"		// string
#define	SRCVIEWER	"SourceViewer"		// string
#define	SHOPIPS		"WorkshopIPs"		// string
#define	BOOTH		"Booth"			// string
						// "BoothID|BoothName"
#define	TERMTYPE	"TerminalType"		// terminal type 0:TCP, 1:HTS

#define	entryINST1	"Interest_1"		// interest_1 config
#define	entryINST2	"Interest_2"		// interest_2 config
#define	entryJISU1	"JsIndex_1"		// jisu_1 config
#define	entryJISU2	"JsIndex_2"		// jisu_1 config
#define	entryJISU3	"JsIndex_3"		// jisu_1 config
#define	entryJISU4	"JsIndex_4"		// jisu_1 config

#define	ORDERTYPE	"OrderType"		// AV9. G|EB 0: AV=D, 1: <190, 2:DZ=:4Z<190
#define	FIRSTRUN	"FirstRun"		// running first
#define	MESSENGER	"Messenger"		// messenger 1b5?

//------------------------------------------------------//
//		WORKSTATION SECTION			//
//------------------------------------------------------//
#define	KEYTM		"KeyTime"		// string
#define	USERMAP		"UserMap"		// string
#define	TERMNAME	"TerminalName"		// string
#define	INITMAP		"InitMap"		// string
#define	TRANTMO		"TranTimeout"		// dword
#define	USAGETMO	"UsageTimeout"		// dword
#define	IDLETMO		"IdleTimeout"		// dword
#define	COMPANY		"Company"		// dword
#define	BRANCH		"Branch"		// dword
#define	APTERMIP	"ApTermIP"		// string
#define	SIGNONID	"SignOnID"		// string
						// "SignOnID | SignOnDesc"
#define	SIGNONTM	"SignOnTime"		// string
#define	HOMEPAGE	"HomePage"		// string
#define	SERVICEID	"ServiceID"		// string
#define	SITEID		"SiteID"		// string

//------------------------------------------------------//
//		INFORMATION SECTION			//
//------------------------------------------------------//
#define	CONNECTBY	"ConnectBy"
#define	byNONE		0
#define	byTCP		1
#define	byINET		2	
#define	byMODEM		3
#define	byPPP		4
#define	byINET2		21
#define	cmDIRECT	31
#define	cmMODEM		32
#define	cmPSDN		33

#define	byURL		"URL"			// string
						// internet | ppp
#define	inPPPs		"PPP"			// string
						// "Number | Dest | ID | PASSWD"
#define	byTCPs		"Server"		// string
#define	PROXY		"Proxy"			// value


//////////////////////////////////////////////////////////////////////////
///	AXIS INI Repository

//
//	menu
//
#define	MENUDAT		"axismenu.dat"
#define	AXISMENU	"axis.menu"
#define	MENUSN		"Menu"

//
//	user
//
#define	AXISUSER	"axisuser.ini"
#define	SYSTEMSN	"System"
#define	SAVECODE	"Codes"

//
//	palette
//
#define	PALETTE		"palette.ini"
#define	GENERALSN	"General"
#define	FLASHSN		"Flash"


//////////////////////////////////////////////////////////////////////////
///	AXISLOGIN id

#define	getOK		-1
#define	getSIZE		0
#define	getMsg		1
#define	getEtype	2
#define	getEcode	3
#define	getTimeR	4

