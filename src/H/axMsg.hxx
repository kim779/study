#pragma once

//
//	message
//
#define	WM_AXIS		(WM_USER+500)
#define	WM_STOP		(WM_USER+501)		// connnect dlg

#define	WM_UPDATE	(WM_USER+502)		// data real update : from frame to concludeDlg
//
//	message : WM_AXIS
//	LOWORD(wParam)
//
#define	axSTART		1
#define	axRUN		2
#define	axAXIS		3	// axis running
#define	axSIGNON	4	// signon

#define	axCHANGE	5	// change current view : combo
#define	axCLONE		6	// clone current view
#define	axCLOSEV	7	// close view
#define	axCLOSEP	8	// close popup
#define	axRELOAD	9	// combo

#define	axTOOLBAR	10	// toolbar
#define	axSELCLIST	11	//
#define axGROUP		12	// toolbar group change

#define	axCURSOR	13	// combo
#define axFOCUS		14	// wsevent
#define	axGUIDE		15	// set guide
#define	axFULLMENU	16	// get menu full string
#define	axNEWS		17	// ticker news
#define	axRESULT	18	// conclusion

#define	axEXTEND	19	// sb extend click : sbx->child->mainframe
#define	axMOVE		20	// move child
#define	axTRIGGER	21	// trigger change  : sbx->child->mainframe

#define	axHOTKEY	22	// orderTR by hotkey
#define	axHOTKEY2	23	// hotkey onTR
#define	axHOTKEY3	24	// show hotkey information

#define	axSCREEN	25	// save user screen
				// HIWORD(wParam) 0 : add, 1 : remove
#define	axMOVEX		26	// move user screen
				// HIWORD(wParam) 0 : is enable?, others : move group#
#define	axMODAL		27	// modal dialog attach & drawing
#define	axKEYS		28	// act VK_XXX (VK_HOME, VK_ESCAPE)

#define	axLINK		29	// create link map
#define	axMSG		30	// show full message

#define	axPRINT		31	// print childframe
#define	axDEVICE	32	// ms read done

#define	axXTR		33	// Xtr
#define	axSETVIEW	34	// get device view
#define axCLOSEX	35	// modalx closex setting
#define axWAIT		36	// cursor change
#define	axURL		37	// open window ie.

#define	axALARM		100	// alarm Msg
//
//	systemTR ID
//
#define	sysBASKET	2	// systemTR : Basket information
#define	sysORDER	3	// systemTR : order



//
//	save, restore position keyword
//
#define	snPOSITION		"POSITION"
#define	posFRAME		"Frame"
#define	posTOOLBAR		"ToolBar_"
#define	posSTATUSBAR		"StatusBar_"
#define	posHOTKEYBAR		"HotkeyBar"
#define	posTIME			"TimeWindow"

#define	snSTATUS		"STATUSBAR"
#define	nINDEX			"sbIndex_"

//////////////////////////////////////////////////////////////////////////
// callback interface	(main)

#define	AXI_BASE		21000
#define	AXI_CHANGEVIEW		(AXI_BASE + 1)		// toolbar call
#define	AXI_SELCLIST		(AXI_BASE + 2)		// toolbar call
#define AXI_CHANGEFOCUS		(AXI_BASE + 3)		// child frame call
#define AXI_ACTIVEMAP		(AXI_BASE + 4)		// tool bar call(for HSBCtrl)
#define AXI_ADDTOOL		(AXI_BASE + 5)		// tool bar call(for HSBCtrl)
#define AXI_DELTOOL		(AXI_BASE + 6)		// tool bar call(for HSBCtrl)
#define AXI_SHOWHELP		(AXI_BASE + 7)		// caption call(show help)
#define	AXI_CHANGETICKER	(AXI_BASE + 8)		// ticker bar call
#define	AXI_SHOWTICKER		(AXI_BASE + 9)		// ticker bar call
#define	AXI_NEWSLIST		(AXI_BASE + 10)		// ticker bar call
#define	AXI_CHANGEBAR2		(AXI_BASE + 11)		// wParam: 0 - change skin. 1 - change virtual screen
#define AXI_SETTRIGGER		(AXI_BASE + 12)		// child frame call for trigger
#define	AXI_RESTORE		(AXI_BASE + 13)		// child frame call for restor
#define	AXI_HOOKTAB		(AXI_BASE + 14)		// ctrl+tab key hook for MDIChild window
#define	AXI_ONLYPIN		(AXI_BASE + 15)		// keep only push pin 
#define AXI_XTR			(AXI_BASE + 16)		// xtr 
#define AXI_CONSEE		(AXI_BASE + 17)		// view control viewer 
#define	AXI_BOARD		(AXI_BASE + 18)		// main board msg........
#define	AXI_FUNCKEY		(AXI_BASE + 19)		// main tool bar function key........
#define	AXI_TABCHANGE		(AXI_BASE + 20)		// tabview map change
#define	AXI_SHOWCONCLUSION	(AXI_BASE + 21)		// show conclusion list
#define	AXI_CHANGECONCLUSION	(AXI_BASE + 22)		// change conclusion setting
#define	AXI_RELOADBB		(AXI_BASE + 23)		// reload board map
#define	AXI_SETBORDER		(AXI_BASE + 24)		// board z-order setting
#define	AXI_CLOSECHILD		(AXI_BASE + 25)		// close child
#define	AXI_SETALL		(AXI_BASE + 26)		// all child set trigger
//////////////////////////////////////////////////////////////////////////
// xTc	define	(SE0000XX)
#define	REGGROUP	2001
#define	REGCODE		2002
#define	USERSAVE	2003
#define	USERLOAD	2004

