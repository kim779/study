#ifndef _AXMSG_HXX
#define	_AXMSG_HXX

//
//	message
//
#define	WM_AXIS		(WM_USER+500)
#define	WM_STOP		(WM_USER+501)		// connnect dlg
#define	WM_UPDATE	(WM_USER+502)		// data real update : from frame to concludeDlg

#define WM_VIEWHOOK	(WM_USER+4000)
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
#define	axURLx		38	// run mainframe link.
#define	axWRITEIMG	39	// save img
#define	axPRINTIMG	40	// print img
#define	axSHOWHELP	41	// show help
#define axCLOSES	42	// single child close

#define axLOADUSER	43	// GetRoot

#define axCLIP 44    //copy to clipboard
#define axTMenu 45

#define axBONDON 90
#define axBONDOFF 91
#define axINTERSETON 92
#define axINTERSETOFF 93
#define axADDINTERSET 94
#define axINTERSETPT 95
#define axINTERMOVE 96
#define axINTERSDIMOVE 9896

#define axLOGINMSG 9893
#define axSTARTMSG 9894
#define axSLIDEEND 9895 
#define axFITSMNG 9897

#define axDELINTERSET 89
#define axRELOADINTER 88
#define axSETINTERKEY 87
#define axSETBONDWND 86
//#define axSETINTERKEY 87


#define	axALARM		100	// alarm Msg
#define	axDIALOG	101	// duplication notice
#define	axKONG		102	// processfmx

#define axPWDCHK	150	// account password check

// NSocket message
#define	axREPEATSOCK	200	// connect socket
#define	axCONNECTSOCK	201	// connect socket
#define	axRECEIVESOCK	202	// receive socket
#define	axCLOSESOCK	203	// close socket

// caption function button 
#define axCAPTION	300
// child message (CChildFrm, CSChild)
#define	axLINKEDIT	701	// show linkedit

#define axChaserON	9998	// Chaser ON
#define axChaserOFF	9999	// Chaser OFF
//
//	systemTR ID
//
#define	sysACCOUNT	1	// systemTR : account/fund information
#define	sysBASKET	2	// systemTR : Basket information
#define	sysORDER	3	// systemTR : order
#define sysAllCancel	4	// systemTR : All Cancel



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
#define AXI_SHOWHELP		(AXI_BASE + 5)		// caption call(show help)
#define AXI_APPSWITCH		(AXI_BASE + 6)		// app switch
#define	AXI_NEWSLIST		(AXI_BASE + 8)		// ticker bar call
#define	AXI_CHANGEBAR2		(AXI_BASE + 9)		// wParam: 0 - change skin. 1 - change virtual screen
#define AXI_SETTRIGGER		(AXI_BASE + 10)		// child frame call for trigger
#define	AXI_RESTORE		(AXI_BASE + 11)		// child frame call for restor
#define	AXI_HOTKEY		(AXI_BASE + 12)		// key hook for HOTKEY(MDIChild window)
#define AXI_CONSEE		(AXI_BASE + 14)		// view control viewer 
#define	AXI_SMCALL		(AXI_BASE + 15)		// smcall.............
#define	AXI_FUNCKEY		(AXI_BASE + 16)		// main tool bar function key........
#define	AXI_TABCHANGE		(AXI_BASE + 17)		// tabview map change
#define	AXI_SHOWDEAL		(AXI_BASE + 18)		// show conclusion list
#define	AXI_BAR2ACTION		(AXI_BASE + 19)		// bar2 fuction
#define	AXI_SETALL		(AXI_BASE + 20)		// all child set trigger
#define	AXI_GETMAPNUM		(AXI_BASE + 21)		// Get screen Number (4)
#define	AXI_TOOLTIPCHANGE	(AXI_BASE + 22)		// tooltip change
#define	AXI_APPENDALLMAP	(AXI_BASE + 23)		// controbar append all map
#define	AXI_MBLINK		(AXI_BASE + 25)		// main caption link infomation
#define	AXI_SENDTICKINFO	(AXI_BASE + 26)		// ticker infomation sendTR
#define	AXI_INPUTSCREENNO	(AXI_BASE + 27)		// Input screen Number (4)
#define AXI_EDITUSERTOOL	(AXI_BASE + 28)		// Call Edit usertool
#define AXI_TICKSETUP		(AXI_BASE + 29)		// Call setup ticker
#define AXI_GETCAPTIONINFO	(AXI_BASE + 30)		// caption information(group Number, font size)
#define AXI_HIDECAPTIONBUTTON	(AXI_BASE + 31)		// caption button hide
#define AXI_EACHTICKER		(AXI_BASE + 32)		// each ticker setting
#define AXI_LINKOPEN		(AXI_BASE + 33)		// link screen open (include code symbol)
#define AXI_XTR			(AXI_BASE + 34)		// xtr 
#define AXI_CLOSESCREEN		(AXI_BASE + 35)		// close screen	

#define	AXI_BOARD		(AXI_BASE + 37)		// main board msg........
#define	AXI_HOOKTAB		(AXI_BASE + 38)		// ctrl+tab key hook for MDIChild window
#define	AXI_RELOADBB		(AXI_BASE + 39)		// reload board map
#define	AXI_SETBORDER		(AXI_BASE + 40)		// board z-order setting
#define AXI_HIDETICKER		(AXI_BASE + 41)		// hide ticker (1-3:ticker1,2,3, 4:toobar, 5:screenlist)

//////////////////////////////////////////////////////////////////////////
// xTc	define	(SE0000XX)
#define	XTRPOST		2000
#define	REGGROUP	2001
#define	REGCODE		2002
#define	USERSAVE	2003
#define	USERLOAD	2004

//////////////////////////////////////////////////////////////////////////
// WM_CURSORMAP LOWORD(wParam) define
#define	SCM_SETCURSORMAP	1
#define	SCM_DELETECURSORMAP	2
#define	SCM_DELETEALLCURSORMAP	3


//////////////////////////////////////////////////////////////////////////
// WM_USER	wParam define
#define MMSG_GETPASSWORD		0x01
#define MMSG_GETCERTPASSWORD		0x02
#define MMSG_APPLYSETUP			0x03
#define MMSG_LOADXTR			0x04

// CTI	define
#define CTIMSG			1004

// TCPIP
#define TCPIPSERVER		2001
#define TCPIPCLIENT		2002
#define TCPIPSERVERSEND		2003
#define TCPIPSERVERERROR	2004
#define TCPIPRECIVE		2005
#define TCPIPSTANDBY		2006
#define TCPIPSRECONNECT		2007
#define TCPIPCLIENTCLOSE	2008
#define TCPIPSCONNECT		2009
#define TCPIPSNOTCONNECT	2010

#define EXCUTE_OPENMAP		2020
#define REDRAWTITLE		2030
#define CLOSEMAP		2031

#endif
