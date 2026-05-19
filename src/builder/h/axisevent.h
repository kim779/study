
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2001
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : axisevent.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2014-09	Initial version
// *****************************************************************************

#pragma once

#pragma	pack(1)

//////////////////////////////////////////////////////////////////////////////////
//	Mainframe -> Wizard
//
// __declspec(dllexport) BOOL APIENTRY axRunAxis(long mode, void* pBytes, long nBytes, long session);	// DI_RUN
// __declspec(dllexport) BOOL APIENTRY axInvokeTR(void* data);						// DI_TRX
// __declspec(dllexport) long APIENTRY axAttachForm(CWnd* view, long type, long key);			// DI_ATTACH
// __declspec(dllexport) void APIENTRY axDetachForm(long key);						// DI_DETACH
// __declspec(dllexport) CString APIENTRY axGetFormName(long key);					// DI_FORMN
// __declspec(dllexport) long APIENTRY axGetFormInfo(CString maps, long& width, long& height);		// DI_FORMI
// __declspec(dllexport) BOOL APIENTRY axLoadForm(long key, CString maps, long width, long height, BOOL only);// DI_FORMS
// __declspec(dllexport) long APIENTRY axWizard(long kind, long value, CString& string);		// DI_WIZARD
// __declspec(dllexport) long APIENTRY axInvoke(long key, long kind, long variant);			// DI_INVOKE

//
//	axRunAxis
//	mode
#define	createWizard		0x01		// pBytes : CWnd* mainframe
#define	loginAXIS		0x02		// ips, port#, session
						// port#(-1) -> close
#define	signUSER		0x03		// user information, information size, session
#define	listenUDP		0x04		// ips, port#
						// port#(-1) -> close
#define	loginSHOP		0x05		// ips, port#
						// port#(-1) -> close
#define	loginAXISx		0x06		// ips, port#, session
						// port#(-1) -> close
#define	signUSERx		0x07		// the signon process pass... P/F startup
#define	loginDONE		0x08		// multi connecttion process done

//
//	axInvokeTR.data : _trxH + data

//
//	axAttachForm.type
//	axGetFormInfo.return
//
#define	vtypeMSK		0xf0		// mask
#define	vtypeSCR		0x10		// fixed view type : scroll
#define	vtypeFIX		0x20		// fixed view type : fixed
#define	vtypeRSZ		0x30		// resize-fixed view type
#define	vtypeWND		0x40		// static window
#define	vtypeFEX		0x50		// fixed view type : non-scroll
#define	vtypeREX		0x60		// resize-fixed vR view type

#define	vtypeERR		0x00
#define	vtypeNRM		0x01
#define	vtypeVBX		0x02
#define	vtypeDLL		0x03
#define	vtypeGRX		0x04
#define	vtypeHTM		0x05

//
//	axAttachForm.key
//
#define	WK_NORM			0x20
#define	WK_POPUP		0x80
#define	WK_RMN			0xfd		// remain dll window key
#define	WK_NONE			0xff

__declspec(dllexport) void APIENTRY axDetachForm(long key);						// DI_DETACH
__declspec(dllexport) CString APIENTRY axGetFormName(long key);						// DI_FORMN
__declspec(dllexport) long APIENTRY axGetFormInfo(CString maps, long& width, long& height);		// DI_FORMI
__declspec(dllexport) BOOL APIENTRY axLoadForm(long key, CString maps, long width, long height, BOOL only);// DI_FORMS
__declspec(dllexport) long APIENTRY axWizard(long kind, long value1, long value2, CString& valstr);	// DI_WIZARD
__declspec(dllexport) long APIENTRY axInvoke(long key, long kind, long variant);			// DI_INVOKE

/*
//
//	axWizard.kind
//
#define	loadACC			0x01
#define	saveACC			0x02		// set account
						// valstr : account information
#define	setGID			0x03		// set group ## (trigger)
						// key : value1, ## : value2
#define	getFDC			0x04		// key : value1, string : FORM symbol \t procedure name
						// if symbol is "", return cursor symbol in valstr
#define	setFDC			0x05		// key : value1, valstr : SYMs \t DATAs
						// key & 0xff00, apply all
						// if valstr is "DATAs", set DATAs at cursor symbol
#define	setFCB			0x06		// set flash color blink, value1 : true / false
#define	setPAL			0x07		// set palette theme
						// if value1 = 1, reload whole palette
#define	devEV			0x08		// device event
						// key : value1
#define	homeEV			0x09		// set cursor at home
						// key : value1
#define	setVID			0x0a		// set virtual screen id ##
						// ## : value1
#define	setTRG			0x0b		// set trigger type
						// value1 = 0, set group type
						// value2 = 0 : group, 1 : all, 2 : current
						// value1 = 1, set virtial screen type
						// value2 = 0 : virtual screen, 1 : all
#define	setFONT			0x0c		// set font size (not resize screen)
						// point : value1, 9 or 10
#define	encPASS			0x0d		// encrypt password
						// valstr : password
#define	setSCN			0x0e		// set screen number
						// key : value1, valstr : "screen number"
#define	setFONTx		0x0f		// set font size (resize screen)
						// key : value1, key == -1, apply all
						// point : value2
#define	applyACC		0x10		// apply account information to VIEW 
#define	getCA			0x11		// get certify information
#define	getLEDGER		0x12		// get ledger information
#define	keyEV			0x13		// key event
						// key : value1, key value : value2
#define	getFOCUS		0x14		// get focus key
#define	setACCG			0x15		// set account group information
						// key : value1, 0 = stock, 1 = future
						// valstr : group_id1 \t group_name1 \n group_id2 \t group_name2 \n ...
#define	setACCB			0x16		// set account combo
						// size : value1, data : valstr
#define	setNOMSG		0x17		// set no msgbox, value1 : true / false
#define	getRTS			0x18		// get data count of rts queue
#define	getNDC			0x19		// get named data (equal to pop)
						// valstr : name
#define	setNDC			0x1a		// set named data (equal to push)
						// valstr : name \t data
#define	setFLOAT		0x1b		// set floating view
						// key : value1, value2 : float = 1
#define	setLINK			0x1c		// set link mode(create new linked child), value1 : true / false
#define	getDLL			0x1d		// get DLL window (CWnd *)
						// key : value1
#define	getHISTORY		0x1e		// get code history
						// valstr : name
#define	setHISTORY		0x1f		// set code history
						// valstr : name \t data
#define	delHISTORY		0x20		// remove history
						// valstr : name \t data
						// if data is empty, remove all
#define	loadCODE		0x21		// load code file
#define	getCODEx		0x22		// char* GetCode(_codeH*)
#define	getUSER			0x23		// char* GetUserID()
#define	getUSERN		0x24		// char* GetUserName()
#define	ddeON			0x25		// void SetDDE()
#define	setMAC			0x26		// set mac address
						// valstr : mac-address

#define	setTRACE		0x99		// set TRACE mode
						// value1 = TRACE message ID
						// value2 : HWND TRACE window handle (NULL = TRACE mode off)
						// OnTrace(WPARAM wParam, LPARAM lParam)

//
//	void OnTrace(WPARAM wParam, LPARAM lParam)
//		HIWORD(wParam) : DATAs type
//		LOWORD(wParam) : DATAs size
//		lParam : DATAs...
#define	x_STRs			0x00		// STRINGs with '\0'
#define	x_RTMs			0x01		// RTMs with '\0'
#define	x_SNDs			0x02		// SND DATAs
#define	x_RCVs			0x03		// RCV DATAs
*/


//////////////////////////////////////////////////////////////////////////////////
//	fmObject -> Wizard
//
//	msg	: WM_USER
//	wParam	: sender form object
//	lParam	: use Cmsg class for various arguments
//

//
//	Cmsg.what
//

//	run script
//
#define	evOnClick		0x0001		// run onClick script
#define	evOnDoubleClick		0x0002		// run onDoubleClick script
#define	evOnMouseEnter		0x0003		// run onMouseEnter script
#define	evOnMouseLeave		0x0004		// run onMouseLeave script
#define	evOnSetFocus		0x0005		// run onSetFocus script
#define	evOnChange		0x0006		// run onChange script
#define	evOnCharChange		0x0007		// run onCharChange script
#define	evOnKey			0x0008		// run onKey script


//	get / set information
//
#define	evinvokeTRx		0x0080		// Invoke TR
						// Cmsg.args[0] = char*	trxH + DATAs
#define evCreateView		0x0081		// create view
						// Cmsg.args[0] = char*	mapName
						// Cmsg.args[1] = int	view type
						// Cmsg.args[3] = char*	option : "/c /t /S /s /K /k /p /d" (change, trigger, search, key, pos, domino)
						// Cmsg.args[4] = char*	domino data : "/d SYM \t DATA ..."
						// return int key of new view
#define	evRunProc		0x0082		// run procedure in owner screen
						// Cmsg.args[0] = char*	procedure-name
						// Cmsg.args[1] = char*	arguments
#define	evRunProc2		0x0083		// run procedure
						// Cmsg.args[0] = int	work key
						// Cmsg.args[1] = char*	procedure-name
						// Cmsg.args[2] = char*	arguments
#define	evMouse			0x0084		// mouse action
						// Cmsg.args[0] = int	lbutton(1) or rbutton(0)
						// Cmsg.args[1] = char*	mapN
						// Cmsg.args[2] = int	pt.x
						// Cmsg.args[3] = int	pt.y
						// Cmsg.args[4] = char*	symbol \t data

#define	evAddAccount		0x0085		// onAccount
						// Cmsg.args[0] = char*	account data
						// Cmsg.args[1] = int	data size
#define	evSaveAccount		0x0086		// set accounts
						// Cmsg.args[0] = char*	account information
#define	evApplyAccount		0x0087		// apply account information to view

#define	evCodeDlg		0x0090		// code dialog
						// Cmsg.args[0] = int	kind of CODE
						// Cmsg.args[1] = int	type of CODE
						// Cmsg.args[2] = int	pt.x
						// Cmsg.args[3] = int	pt.y
						// return char* code
#define	evCodeDlg2		0x0091		// code dialog
						// Cmsg.args[0] = int	kind of CODE
						// Cmsg.args[1] = int	type of CODE
						// Cmsg.args[2] = char*	code
						// Cmsg.args[3] = int	pt.x
						// Cmsg.args[4] = int	pt.y
						// return char* code
#define	evCodeName		0x0092		// get code name
						// Cmsg.args[0] = char*	code
						// return char* code name
#define	evCodeType		0x0093		// get code type (whichTYPE)
						// Cmsg.args[0] = char*	code
						// return int code type
#define	evFindCode		0x0094		// find code with code or name
						// Cmsg.args[0] = char*	code or name
						// return char* code
#define	evCodeUnit		0x0095		// get trading unit
						// Cmsg.args[0] = char*	code
						// return int unit
#define	evCodeInfo		0x0096		// get code info
						// Cmsg.args[0] = char*	code
						// return char* info
#define	evStockCode		0x0097		// is stock code (serach mode in fmEDIT)
						// Cmsg.args[0] = char* code
						// return BOOL
#define	evElwBASIS		0x0098		// get elw basis
						// Cmsg.args[0] = char*	code
						// return BOOL
#define	evListStock		0x0099		// get listedstock
						// Cmsg.args[0] = char*	code
						// return int
#define	evCodeR			0x009a		// get codes _codeH*
						// Cmsg.args[0] = _codeH*
						// return char* code list
#define	evCloseView		0x00a0		// close view
#define	evSizeView		0x00a1		// set size
						// Cmsg.args[0] = int	width
						// Cmsg.args[1] = int	height
#define	evAutoView		0x00a2		// code change automatically 
						// Cmsg.args[0] = int	count
						// Cmsg.args[1] = char*	(code \t name) * count
#define	evGetViewName		0x00a3		// get name of view
						// Cmsg.args[0] = int	key
						// return char* mapname
#define	evMenuView		0x00a4		// menu search dialog
						// return char*	mapname
#define	evLinkView		0x00a5		// link menu dialog
						// Cmsg.args[0] = char*	mapname
#define	evGetLedger		0x00a6		// get ledger
						// Cmsg.args[0]	= char*	ledgerH
						// set ledger-information in ledgerH
#define	evGetCursor		0x00a7		// get cursorHCURSOR GetCursor(int kind)
						// Cmsg.args[0]	= int	kind (numeric value)
						// return HCURSOR
#define	evEncPass		0x00a8		// encryptCString Encrypt(string)
						// Cmsg.args[0]	= char*	string (password)
						// return char* encrypted string
#define	evSetData		0x00a9		// set data
						// Cmsg.args[0] = int	key, if -1, set all form in current view
						// Cmsg.args[1] = int	symbol
						// Cmsg.args[2] = int	data
#define	evTabstop		0x00aa		// set tapstop
						// void SetTabStop(int key, BOOL tabstop, (char *)name)
						// Cmsg.args[0] = int	tabstop (1 or 0)
						// Cmsg.args[1] = char*	form-object name
#define	evSetRect		0x00ab		// set rect
						// Cmsg.args[0] = CRect*
#define	evRadioGroup		0x00ac		// move focus in radio-group
						// Cmsg.args[0] = int	next (1 or 0)
#define	evFilter		0x00ad		// set grid rts filter
						// Cmsg.args[0] = int	operator (equal, not-equal,..etc)
						// Cmsg.args[1] = int	absolute value (1 or 0)
						// Cmsg.args[2] = char*	value
#define	evGetMain		0x00ae		// get mainWnd
						// return CWnd*
#define	evGetAuth		0x00af		// get authority of map
						// Cmsg.args[0] = char*	mapName
						// return int
#define	evCheckCA		0x00b0		// check certification passwd
						// Cmsg.args[0] = char*	passwd
						// return BOOL

#define	evPrint			0x00e0		// print
#define	evText			0x00e1		// save text file
						// Cmsg.args[0] = int	include-head (1 or 0)
						// Cmsg.args[1] = int	append (1 or 0)
						// Cmsg.args[2] = char*	path (save path)
#define	evExcel			0x00e2		// save excel file(CString text, bool visible, bool has_format)
						// Cmsg.args[0] = int	run (1 or 0)
						// Cmsg.args[1] = char*	format
						// Cmsg.args[2] = char*	excel DATAs
#define	evExcel2		0x00e3		// save excel file(CString text, bool visible, bool has_format)
						// Cmsg.args[0] = int	run (1 or 0)
						// Cmsg.args[1] = char*	format
						// Cmsg.args[2] = char*	excel DATAs
#define	evExport		0x00e4		// save file

#define	evVariant		0x00f0		// onVariant
						// Cmsg.args[0] = int	command

//
//	evVariant.command = Cmsg.args[0]
//
#define	ccTitle			0x01		// setWindowText
						// Cmsg.args[1] = char*	text
#define	ccAddHistory		0x02		// set history
						// Cmsg.args[1] = char*	symbol
						// Cmsg.args[2] = char*	code
#define	ccDelHistory		0x03		// remove history
						// Cmsg.args[1] = char*	symbol
						// Cmsg.args[2] = char*	code, if code is empty, remove all codes in symbol
#define	ccGetHistory		0x04		// get history
						// Cmsg.args[1] = char*	symbol
						// return char* history (code + name \t ...)
#define	ccTrigger		0x05		// trigger
						// Cmsg.args[1] = char*	symbol
						// Cmsg.args[2] = char*	data
#define	ccTriggerw		0x06		// wide trigger(CString text)
						// Cmsg.args[1] = char*	symbol
						// Cmsg.args[2] = char*	data

#define	ccTriggerGroup		0x07		// get trigger group of view
						// return int
#define	ccGuide			0x08		// set guide
						// Cmsg.args[1] = char*	guide
#define	ccPush			0x09		// push data
						// Cmsg.args[1] = char*	name
						// Cmsg.args[2] = char*	data
#define	ccPop			0x0a		// pop data
						// Cmsg.args[1] = char*	name
						// return char* data
#define	ccFull			0x0b		// input is full
						// wrap if form-object has faWRAP
						// return BOOL
#define	ccMajorKey		0x0c		// get major key
						// return int
#define	ccMinorKey		0x0d		// get minor key
						// return int
#define	ccMacAddress		0x0e		// get mac-address
						// return char*
#define	ccLoginPass		0x0f		// get login password
						// return char*
#define	ccIgnore		0x10		// get change-ignore flag
						// return BOOL
#define	ccSystemID		0x30		// get System-ID
						// return char* "H" or "F", "H" : HTS, "F" : FO only
#define	ccLoginDept		0x31		// get login-dept
						// return char*
#define	ccLoginDate		0x32		// get login-date
						// return char*
#define	ccOrderEnable		0x33		// IsOrderEnable
						// return BOOL
#define	ccOrderConfirm		0x34		// IsOrderConfirmEnable
						// return BOOL
#define	ccProhibit		0x35		// IsProhibit
						// return BOOL 
#define	ccCallCenter		0x36		// IsCallCenter

#define	ccGetAccount		0x50		// get account
						// Cmsg.args[1] = int	full (1 or 0)
						// return if full, accounts (account \t pass \t \name \t ....\n ...)
						// otherwise accounts(account \t name \n ...)
#define	ccGetAccountPass	0x51		// get account passwd
						// Cmsg.args[1] = char*	account
						// return char* passwd
#define	ccGetAccountCB		0x52		// get combo-account information
						// Cmsg.args[1] = symbol
						// return char*	default item \t (item \t display \n) * n
#define	ccSetAccount		0x53		// set combo-account information
						// Cmsg.args[1] = char*	symbol
						// Cmsg.args[1] = char*	data
#define	ccGetAccountName	0x54		// get account name
						// Cmsg.args[1] = acount
#define	ccGetAccountInfo	0x55		// get account information
						// Cmsg.args[1] = acount
						// return char* "pass \t name \t ..."
#define	ccSetAccountInfo	0x56		// set account information
						// Cmsg.args[1] = char*	acount
						// Cmsg.args[1] = char*	info (pass \t name \t...)
#define	ccGetAccountGroup	0x57		// GetAccountGroup(int key)
						// Cmsg.args[1] = int	key (1 or 0)
						// return char*	group_id1 \t group_name1 \n group_id2 \t group_name2 \n ...
						// if key = 0, return stock account group
						// key = 1, return future account group

//
//	trxH
//
struct	_trxH	{
	char	trc[8];
	BYTE	key;
	BYTE	ukey;				// user-defined key (form object only)
	BYTE	path;				// destination server id

	WORD	stat;
	int	size;
};
#define	L_trxH	sizeof(struct _trxH)

//
//	trxH.stat
//
#define	TS_ENC			0x0001
#define	TS_OOP			0x0002
#define	TS_PASS			0x0004
#define	TS_CA			0x0008
#define	TS_KEY			0x0010		// ukey is valid (form object only)
#define	TS_CAx			0x0020		// input certification passwd automatically
#define	TS_XRTM			0x0080		// no RTM

//
//	trxH.path
//
#define	ssMASTER		0		// routing main Server (AXIS)


//
//	codeH
//
struct	_codeH {
	int	kind;
	DWORD	type;
	char	code[32];
};

//
//	codeH.kind
//
#define	CDKIND_ALL			100
#define	CDKIND_KOSPI			101
#define	CDKIND_KOSDAQ			102
#define	CDKIND_KRX			103
#define	CDKIND_SUBSIDIARY		104
#define	CDKIND_THEME			105
#define	CDKIND_FREEBOARD		106
#define	CDKIND_KOSPI200			107
#define	CDKIND_KOSTAR 			108
#define	CDKIND_FILTER			109 
#define	CDKIND_ETF			110
#define	CDKIND_ELW			200
#define	CDKIND_ELW_BASE			201
#define	CDKIND_ELW_PB			202
#define	CDKIND_ELW_ALL			203
#define	CDKIND_ELW_MONTH		204 
#define	CDKIND_INDEX			300
#define	CDKIND_INDEX_KOSPI		301
#define	CDKIND_INDEX_KOSDAQ		302
#define	CDKIND_INDEX_KRX		303
#define	CDKIND_INDEX_K200		304
#define	CDKIND_INDEX_K100		305
#define	CDKIND_INDEX_KOSTAR		306
#define	CDKIND_INDEX_FREEBOARD		307
#define	CDKIND_INDEX_CODECHANGE		308
#define	CDKIND_INDEX_ALL		309
#define	CDKIND_FO			400
#define	CDKIND_FUTURE_K200		401
#define	CDKIND_OPTION_K200		402
#define	CDKIND_SFUTURE			403
#define	CDKIND_SOPTION			404
#define	CDKIND_FUTURE_STAR		405
#define	CDKIND_FUTURE_MONTH		406
#define	CDKIND_OPTION_MONTH		407
#define	CDKIND_OPTION_HSGA		408
#define	CDKIND_OPTION			409
#define	CDKIND_SFUTURE_ALL		410 
#define	CDKIND_SOPTION_ALL		411 
#define	CDKIND_FOREIGN_ALL		500
#define	CDKIND_FOREIGN_MAJOR		501
#define	CDKIND_FOREIGN_INDEX		502
#define	CDKIND_FOREIGN_ITEM		503
#define	CDKIND_FOREIGN_FUTURE		504
#define	CDKIND_FOREIGN_RATE		505
#define	CDKIND_FOREIGN_DEBT		506
#define	CDKIND_FOREIGN_EXCHANGE		507
#define	CDKIND_HISTORY			600
#define	CDKIND_KOFEX			700
#define	CDKIND_KOFEX_FUTURE		701
#define	CDKIND_KOFEX_OPTION		702
#define	CDKIND_FOINDEX_ALL		800
#define	CDKIND_FOINDEX_MAJOR		801
#define	CDKIND_FOINDEX_FO_MONTH		802
#define	CDKIND_FOINDEX_FO		803
#define	CDKIND_FOINDEX_FI		804
#define	CDKIND_FOINDEX_HISTORY		805
#define	CDKIND_FOINDEX_FUTURE		806
#define	CDKIND_FOINDEX_MARKET		807
#define	CDKIND_GFUTURE_ALL		900
#define	CDKIND_GFUTURE_CURRENCY		901
#define	CDKIND_GFUTURE_INTEREST		902
#define	CDKIND_GFUTURE_INDEX		903
#define	CDKIND_GFUTURE_COMMODITY	904
#define	CDKIND_GFUTURE_METALS		905
#define	CDKIND_GFUTURE_ENERGY		906 
#define	CDKIND_GFUTURE_BOND		907
#define	CDKIND_GFUTURE_USE		908
#define	CDKIND_GFX_INDEX		920

//
//	codeH.type
//
#define	CDTYPE_CODE		0x00000001
#define	CDTYPE_NAME		0x00000002
#define	CDTYPE_RCODE		0x00000004
#define	CDTYPE_ENAME		0x00000008
#define	CDTYPE_MONTH		0x00000010
#define	CDTYPE_HSGA		0x00000020 


#pragma	pack()
