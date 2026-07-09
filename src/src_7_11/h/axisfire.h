
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
//  Components   : axisfire.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2001-07	Initial version
// *****************************************************************************

#ifndef	_AXISFIRE_H_
#define	_AXISFIRE_H_

#pragma	pack(1)

////////////////////////////////////////////////////////////////////////////////
//	Sock Control ID_FIRE = 1
//	void OnFireEvent(int type, char* pBytes, int nBytes)
//		type   : event type
//		pBytes : data
//		nBytes : detail information(length, code, flag, etc)
//
//	Wizard Control ID_FIRE = 1
//	void OnFireEvent(int type, WPARAM wParam, LPARAM lParam)
//		type   : event type
//		wParam : detail information
//		lParam : DATAs...

#define	FEV_OPEN		0		// axis connected
						// zero  : success
						// other : fail code
#define	FEV_CLOSE		1		// axis closed
#define	FEV_RUN			2		// axis/workstation start
						// true  : axis update
						// false : axis start
#define	FEV_SIZE		3		// file size information
#define	FEV_VERS		4		// screen version table

#define	FEV_ANM			5		// alert message(tick, flash)
#define	FEV_AXIS		6		// axis message
						// LOWORD(wParam) : key
						// HIWORD(wParam) : ...
#define	FEV_STAT		7		// progress stat
						// resource name, progress stat
#define	FEV_PUSH		8		// push message

#define	FEV_ERROR		9		// error, string
						// LOWORD(wParam) : key
						// HIWORD(wParam) : level
#define	FEV_GUIDE		10		// guide, code
						// LOWORD(wParam) : key
#define FEV_SIGNR		15

////////////////////////////////////////////////////////////////////////////////
//	AXIS/Frame TRx Control ID_FIRE = 1
#define	FEV_FMX			20		// request AXIS/Frame TRx
						// LOWORD(nBytes), LOWORD(wParam) : key
						// HIWORD(nBytes), HIWORD(wParam) : size

////////////////////////////////////////////////////////////////////////////////
//	Certify Control ID_FIRE = 1
#define	FEV_CA			30		// certify message
						// LOWORD(wParam) : key
						// HIWORD(wParam) : ...

//
//	FEV_AXIS.key
//	LOWORD(wParam)
//
#define	loginERR		0x99		// type : HIWORD(wParam)
#define	runAXIS			0x00		//
#define	newVIEW			0x01		// lParam : userWH + DATAs, current key : HIWORD(wParam)
#define	renVIEW			0x02		// key : HIWORD(wParam), map    : lParam
#define	delVIEW			0x03		// key : HIWORD(wParam)
#define	htmlVIEW		0x04		// key : HIWORD(wParam), html   : lParam
#define	titleVIEW		0x05		// key : HIWORD(wParam), title  : lParam
#define	autoVIEW		0x06		// ### : HIWORD(wParam), items  : lParam
#define	mapNAME			0x07		// key : HIWORD(wParam), map    : lParam
#define	waitPAN			0x08		// key : HIWORD(wParam), wait   : lParam
#define	hangulPAN		0x09		// key : HIWORD(wParam), hangul : lParam
#define	doublePAN		0x0a		// key : HIWORD(wParam), double : lParam
#define	alarmPAN		0x0b		// dialog ? : HIWORD(wParam), text : lParam
#define	noticePAN		0x0c		// text : lParam
#define	dialogPAN		0x0d		// type : HIWORD(wParam), data  : lParam
#define	axisPAN			0x0e		// type : HIWORD(wParam), data  : lParam
#define	sizeVIEW		0x0f		// key : HIWORD(wParam),  width : LOWORD(lParam), height : HIWORD(lParam)
#define	userINFO		0x10		// ### : HIWORD(wParam),  data  : lParam
#define	printVIEW		0x11		// key : HIWORD(wParam),  data  : lParam
#define	menuAXIS		0x12		// load menu
#define	linkVIEW		0x13		// data : lParam
#define	closeAXIS		0x14		// terminate AXIS
						// reboot : HIWORD(wParam)
#define	nameVIEW		0x15		// key : HIWORD(wParam), bug : lParam , get active mapname
#define	accnINFO		0x16		// apply account information to VIEW ?
#define	userPASS		0x17		// get login passwd, passwd : lParam
#define	userPASSx		0x18		// set login passwd, passwd : lParam
#define	reloadACCN		0x19		// key : HIWORD(wParam), username    : lParam

//
//	FEV_AXIS.loginERR
//	HIWORD(wParam)
//
#define	axisERR			0x00		// General ERROR
#define	signERR			0x01		// SignOn ERROR
#define	secureERR		0x02		// Secure ERROR
#define	certifyERR		0x03		// certify ERROR

//
//	FEV_CA.key
//	LOWORD(wParam)
//
#define	invokeCA		0x01		// invokeTR "AXISENCA" (pBytes, nBytes)
#define encryptCA		0x02		// CString Encrypt(string, mode)
						// lParam : string (password \t keys (account or userID))
						// mode   : HIWORD(wParam) (0 : account, 1 : userID)
#define	guideCA			0x03		// SetGuide(CString text)
#define	closeCA			0x04		// terminate AXIS
#define	htmlCA			0x05		// html view


//
//	userWH
//
struct	_userWH	{
	char		type;			// window type
	unsigned char	key;			// window key
	char		group;			// trigger group
	POINT		pos;			// window position
						// pos.x == -1. pos.y : pos value
	char		maps[8];		// MAPs(8) + domino DATAs
};
#define	L_userWH	sizeof(struct _userWH)


//
//	userWH.type
//
#define	typeVIEW		0x01
#define	typeMODAL		0x02
#define	typePOPUP		0x03
#define	typePOPUPX		0x04		// activate on focusing

//
//	userWH.pos
//	if pos.x == -1, pos.y
//
#define	LtopPOS			1
#define	RtopPOS			2
#define	LbottomPOS		3
#define	RbottomPOS		4
#define	CenterPOS		5
#define LCenterPOS		6
#define RCenterPOS		7
#define	LtopLup			11
#define	LtopRup			12
#define	LtopLdown		13
#define	LtopRdown		14
#define	RtopLup			21
#define	RtopRup			22
#define	RtopLdown		23
#define	RtopRdown		24
#define	LbottomLup		31
#define	LbottomRup		32
#define	LbottomLdown		33
#define	LbottomRdown		34
#define	RbottomLup		41
#define	RbottomRup		42
#define	RbottomLdown		43
#define	RbottomRdown		44
#define	CenterView		99


//////////////////////////////////////////////////////////////////////////////////
//	Dispatch Helper IDs
//	Wizard Dispatch ID
//
#define	DI_RUN			0x01		// BOOL RunAxis(int mode, char* pBytes, int nBytes)
#define	DI_TRX			0x02		// BOOL axTR(char* pBytes, int nBytes)
						// pBytes = userTH + DATAs, nBytes = DATA size
#define	DI_ATTACH		0x03		// int axAttach(CWnd* view, int type, int key)
#define	DI_DETACH		0x04		// void axDetach(int key)
#define	DI_FORMS		0x05		// BOOL axWorkShop(int key, CString maps, int size, bool only)
						// maps = MAP(8) [domino DATAs...]
#define	DI_FORMN		0x06		// char* axWorkName(int key)
#define	DI_FORMI		0x07		// int axWorkView(CString mapN, int* size)
#define	DI_WIZARD		0x08		// LONG_PTR axWizard(int kind, LONG_PTR variant)
#define	DI_INVOKE		0x09		// int axInvoke(int wParam, int lParam)

//
//	DI_ATTACH.type
//	DI_FORMI.return
//
#define	vtypeMSK		0xf0		// mask
#define	vtypeSCR		0x10		// fixed view type : scroll
#define	vtypeFIX		0x20		// fixed view type : fixed
#define	vtypeRSZ		0x30		// resize-fixed view type
#define	vtypeWND		0x40		// static window
#define	vtypeFEX		0x50		// fixed view type : non-scroll

#define	vtypeERR		0x00
#define	vtypeNRM		0x01
#define	vtypeVBX		0x02
#define	vtypeDLL		0x03
#define	vtypeGRX		0x04
#define	vtypeHTM		0x05

//
//	DI_ATTACH.key
//
#define	WK_NORM			0x20
#define	WK_POPUP		0x80
#define	WK_RMN			0xfd		// remain dll window key
#define	WK_NONE			0xff

//
//	DI_RUN.mode
//
#define	loginAXIS		0x01		// ips, port#
						// port#(-1) -> close
#define	signUSER		0x02		// user information, information size
#define	listenUDP		0x03		// ips, port#
						// port#(-1) -> close
#define	loginSHOP		0x04		// ips, port#
						// port#(-1) -> close
#define	loginAXISx		0x05		// ips, port#
						// port#(-1) -> close
#define	signUSERx		0x06		// the signon process pass... P/F startup
#define	listenCAST		0x07		// port#

//
//	DI_WIZARD.kind
//	LOWORD(kind)
//
#define	loadACC			0x01
#define	saveACC			0x02		// set account
						// HIWORD(kind) 1 : all, 0 : one, variant : account information
#define	setGID			0x03		// set group ## (trigger)
						// key : LOWORD(variant), ## : HIWORD(variant)
#define	getFDC			0x04		// key : HIWORD(kind), variant : FORM symbol
						// if symbol is "", return cursor symbol
#define	setFDC			0x05		// key : HIWORD(kind), variant : SYMs \t DATAs
						// key & 0xff00, apply all
						// if variant is "DATAs", set DATAs at cursor symbol
#define	setFCB			0x06		// set flash color blink, variant : true / false
#define	setPAL			0x07		// set palette
						// if HIWORD(kind) = 1, reload whole palette
#define	devEV			0x08		// device event
						// key : HIWORD(kind)
#define	homeEV			0x09		// set cursor at home
						// key : HIWORD(kind)
#define	setVID			0x0a		// set virtual screen id ##
						// ## : HIWORD(variant), key : LOWORD(variant)
#define	setTRG			0x0b		// set trigger type
						// HIWORD(kind) = 0, set group type
						// HIWORD(variant) 0 : group, 1 : all, 2 : current
						// HIWORD(kind) = 1, set virtial screen type
						// HIWORD(variant) 0 : virtual screen, 1 : all
#define	setFONT			0x0c		// set font size (not resize screen)
						// key : HIWORD(kind), key == -1, apply all
						// point : HIWORD(variant), 9 or 10
#define	encPASS			0x0d		// encrypt password
						// variant : password \t keys (account or userID)
						// HIWORD(kind) = 0 : account, 1 : userID
#define	setSCN			0x0e		// set screen number
						// key : HIWORD(kind), variant : "screen number"
#define	setFONTx		0x0f		// set font size (resize screen)
						// key : HIWORD(kind), key == -1, apply all
						// point : HIWORD(variant)
#define	applyACC		0x10		// apply account information to VIEW 
#define	getCA			0x11		// get certify information
#define	setFLOAT		0x12		// set floating view
						// key : HIWORD(kind), variant : float = 1
#define	setCAv			0x13		// set certify version
						// vers : HIWORD(kind)
#define	setLINK			0x14		// set link mode(create new linked child), variant : true / false
#define	optSOCK			0x15		// set socket parsing mode, variant : true / false
#define	filterRTM		0x16		// filter realtime, variant : rtm_type \t rmt_type
#define	setINTERVAL		0x17		// set filtering interval, variant : interval value in millisecond
						// if variant = 0, do not apply filtering
#define	setGOPTION		0x18		// set grid refresh option
						// key : HIWORD(kind), key == -1, apply all
#define	setNDC			0x19		// set named data (equla to push)
						// variant : name \t data
#define reloadMASTER		0x20
#define	setTRACE		0x99		// set TRACE mode
						// HIWORD(kind) = TRACE message ID
						// variant : HWND TRACE window handle (NULL = TRACE mode off)
						// OnTrace(WPARAM wParam, LPARAM lParam)
//
//	setCAv : Certify Version
//
#define	caVER1			1		// Certify(char* pBytes, int* nBytes)
#define	caVER2			2		// Certify(char* pBytes, int* nBytes, char* infos)


//
//	void OnTrace(WPARAM wParam, LPARAM lParam)
//		HIWORD(wParam) : DATAs type
//		LOWORD(wParam) : DATAs size
//		lParam : DATAs...
#define	x_STRs			0x00		// STRINGs with '\0'
#define	x_RTMs			0x01		// RTMs with '\0'
#define	x_SNDs			0x02		// SND DATAs
#define	x_RCVs			0x03		// RCV DATAs


//
//	Sock Dispatch ID
//
#define	DI_OPEN			0x01		// BOOL OpenSock(CString home, CString ip, int port)
#define	DI_CLOSE		0x02		// void CloseSock()
#define	DI_WRITE		0x03		// BOOL WriteStream(char* pBytes, int nBytes)
#define	DI_DOPEN		0x04		// BOOL OpenDgram(int port)
#define	DI_DCLOSE		0x05		// void CloseDgram()
#define	DI_DWRITE		0x06		// BOOL WriteDgram(CString ip, char* pBytes, int nBytes, BOOL route)
#define	DI_AXIS			0x07		// BOOL StartAxis(char* keys, int mode)
#define	DI_ENUM			0x08		// int  GetEnum(char* pBytes, int nBytes, int mode)


//
//	DI_AXIS.mode
//
#define	modeWM			0		// workshop manager
#define	modeWS			-1		// workstation mode
#define	modeRM			-2		// resource/version control manager


//
//	DI_ENUM.mode
//
#define	enumQUEUE		0		// get queuing data count


//
//	Xecure Dispatch ID
//
#define	DI_ENC			0x01		// BOOL Encrypt(char* pBytes, int* nBytes)
#define	DI_DEC			0x02		// BOOL Decrypt(char* pBytes, int* nBytes)
#define	DI_XEC			0x03		// int Xecure(char* pBytes, int nBytes)


//
//	Certify Dispatch ID
//
#define	DI_ONCA			0x01		// char* OnCertify(char* pBytes, int* nBytes)
#define	DI_CA			0x02		// BOOL  Certify(char* pBytes, int* nBytes)
#define	DI_CAERR		0x03		// BOOL  CertifyErr(char* pBytes, int nBytes)
#define	DI_CAID			0x04		// void  CertifyID(char* pBytes)
#define	DI_CAEX			0x05		// BOOL  CertifyEx()
#define	DI_CAX			0x06		// BOOL  Certify(char* pBytes, int* nBytes, char* infos)


//
//	AxisCode.dll
//	bool axGetCode(int kind, char* code, int type)
//
#define	allCODE			99
#define	kospiCODE		100
#define	futureCODE		101
#define	optionCODE		102
#define	foptionCODE		103
#define	indexCODE		104
#define	bondCODE		105
#define	kofexCODE		106
#define	kospi2CODE		107
#define	koptionCODE		108
#define	sinjuCODE		109
#define	thirdCODE		110
#define	getCODEfuture		111			// FUTURE code
#define	getCODEcall		112			// ATM CALL option
#define	getCODEput		113			// ATM PUT option
#define	ecnCODE			114			// ECN code
#define faoptionCODE		115			// future + option + kospi option
#define all2CODE		116			// kospi + option
#define	usCODE			117			// overseas code
#define	spreadCODE		118			// Spread
#define	kkCODE			119			// kospi + kosdaq
#define	fdCODE			120			// fund
#define	kkelwCODE		121			// kospi + kosdaq + elw (except etf)
#define kfutureCODE		122			// kospi + FUTURE 
#define etfCODE			123			// etf
#define kospi50CODE		124			// kospi 50
#define elwCODE			125			// elw
#define commdtCODE		126			// commodity future
#define	UNbondCODE		127
#define	AgencyCODE		128

#define	isCODEtype		256			// isCodetype(code, type)

//
//	AxisCode.dll
//	bool axGetName(int kind, CString code, char* name, int* type)
//
#define	codeNAME		100
#define	futureNAME		101
#define	optionNAME		102
#define	indexNAME		104
#define bondNAME		105
#define	kofexNAME		106
#define kospi2NAME		107
#define	koptionNAME		108
#define sinjuNAME		109
#define thirdNAME		110
#define	fdNAME			111
#define ecnNAME			114
#define	whichNAME		115
#define	whichTYPE		116
#define whichKIND		117			// get code kind : future, option, koption 
#define kfutureNAME		122			// 주식선물
#define codeSTYPE		123
#define commdtNAME		126			// commodity future
#define agname			129
//
//	AxisCode.dll
//	bool axCodeList(int kind, CString& list)
//
#define	futureLIST		100		// {code \t name \t}
#define	callLIST		101		// {월물 \t ATM \t {code \t name \t 행사가 \t}\n}
#define	putLIST			102		// {월물 \t 행사가 \t code \t name \n}
// 추가
#define pcodeLIST		103		// 주식옵션 {거래대상종목구분 \t 거래대상종목명 \t}
#define pcallLIST		104		// 주식옵션 {월물 \t ATM \t {code \t name \t 행사가 \t}\n}
#define pputLIST		105		// 주식옵션 {월물 \t 행사가 \t code \t name \n}
#define kfutureLIST		106		// 스타지수 선물 {code \t name \t}
// 추가 KRX100선물 2006.05.23 _KSH
#define xfutureLIST		107		// KRX100 선물	{code \t name \t}
// 추가 주식선물 2006.10.27
#define sfutureLIST		108		// 주식선물	{code \t name \t}
#define commdtLIST		109		

//#define	codeENAME		120
//#define	futureENAME		121
//#define	optionENAME		122

#define	elwJS			255			// ELW 기초자산 code
#define	whichORDER		256			// getCodeOrderable(code)


//
//	AxisModal.dll
//	char* axModal(int kind. CString keys, CString currents)
//
#define	columnED		100
#define	tabED			101
#define	linkED			102


//
//	Wizard Fire Dispatch   ... WM_USER
//	LOWORD(WPARAM)
//			pBytes ... LPARAM
//			nBytes ... HIWORD(WPARAM)
//
#define	getPALETTE		0x01		// PALETTE GetPalette(pBytes)
#define	invokeTRx		0x02		// InvokeTRx(pBytes, nBytes)
						// if FM_CONTROL, pBytes = SYM \t userTH + DATAs, nBytes = DATA size
						// else pBytes = userTH + DATAs, nBytes = DATA size
#define	viewDLL			0x04		// SetView(int type, CString lParam)
						// HIWORD(wParam) = type
						// lParam = MAPs(8) + OPTIONs + domino DATAs ("/d SYM \t DATA ...")
						// OPTIONs : "/c /t /S /s /K /k /p /d" (change, trigger, search, key, pos, domino)
#define	dropDLL			0x05		// Register(HWND hWnd, bool dropWnd)
						// HIWORD(wParam) = drop target is DllMainWnd(false)
						// if hWnd is NULL, current view register.
#define	dragDLL			0x06		// DoDragDrop(CString text)
						// text : SYM \t DATA (\t DATA ...)
#define	revokeDLL		0x07		// Revoke(HWND hWnd)
#define	alertDLLx		0x08		// xAlert(int key)
						// if key is -1, all unit unregister key.
#define	clipDLL			0x09		// SetClipboard(CString lParam)
						// CString GetClipboard() ... lParam is NULL
#define	procDLL			0x0a		// Procedure(CString procs, int major, int minor)
						// lParam = PROCS \t PARAM
						// LOBYTE(HIWORD(wParam)) : major
						// HIBYTE(HIWORD(wParam)) : minor
#define	mouseDLL		0x0b		// OnMouse(lParam, lbutton)
						// HIWORD(wParam) : lbutton(True) or rbutton(False)
						// lParam : mapN \n pt.x \t pt.y \n SYM \t DATA \n
						// if grid, lParam : mapN \t \n pt.x \t pt.y \n SYM \t DATA \n
#define	codeTYPE		0x0c		// GetCodeType(CString code) : whichTYPE
#define	accountDLL		0x0d		// OnAccount(pBytes, nBytes)
#define	variantDLL		0x10		// OnVariant(CString lParam, int wParam)
						// HIWORD(wParam) : command
#define	codeDLL			0x11		// CString GetCode(int kind, int type, CPoint pt)
						// LOBYTE(HIWORD(wParam)) : kind of CODE
						// HIBYTE(HIWORD(wParam)) : type of CODE
						// LOWORD(lParam) : pt.x
						// HIWORD(lParam) : pt.y
#define	nameDLL			0x12		// CString GetName(CString lParam)
						// lParam[0] : return the type of CODE
#define	columnDLL		0x13		// CString GetColumn(CString lParam)
#define	excelDLL		0x14		// SaveExcel(CString text, bool visible)
						// lParam = excel DATAs
						// HIWORD(wParam) = visible
#define	closeDLL		0x15		// CloseView()
#define	sizeDLL			0x16		// SetSize(lParam)
						// LOWORD(lParam) : width
						// HIWORD(lParam) : height
#define	formDLL			0x17		// SetData(int key, CString data)
						// HIWORD(wParam) : key
						// lParam : data, SYM \t DATA
#define	eventDLL		0x18		// DoEvent(int key, int ev, CString data)
						// LOBYTE(HIWORD(wParam))) : key
						// HIBYTE(HIWORD(wParam))) : ev
						// lParam : data, SYM
#define	autoDLL			0x19		// Auto(char* data, int count)
						// HIWORD(wParam) : count
						// lParam : data, (CODE \t NAME) * count

#define	sizeMAP			0x1a		// int SizeMap(char* maps)
#define	attachMAP		0x20		// int AttachMap(CWnd* pWnd, int type)
						// lParam : pWnd, HIWORD(wParam) : type, return value : key
#define	detachMAP		0x21		// void DetachMap(int key)
						// HIWORD(wParam) : key
#define	loadMAP			0x22		// BOOL LoadMap(int key, char* maps)
						// HIWORD(wParam) : key
						// lParam : maps(8) + domino DATAs ("SYM \t DATA ...")
#define	dominoMAP		0x23		// void DominoMap(int key, char* data)
						// HIWORD(wParam) : key. key == WK_NONE, apply ALL
						// lParam : domino DATA ("SYM \t DATA")
#define	ledgerDLL		0x24		// GetLedger(char* ledger)
						// lParam : ledger
#define	typeDLL			0x25		// int GetViewType(CString mapN)
#define	waitDLL			0x26		// BOOL IsWait(int key)
						// HIWORD(wParam)

#define	getFONT			0x27		// CFont* GetFont(_fontR*)
						// lParam : _fontR*
#define	getPEN			0x28		// CPen* GetPen(_penR*)
						// lParam : _penR
#define	getBRUSH		0x29		// CBrush* GetBrush(COLORREF clr)
						// lParam : clr
#define	getBITMAP		0x30		// CBitmap* GetBitmap(char* fileN)
						// lParam : full path file name

#define	encPASSx		0x31		// CString Encrypt(string, mode)
						// lParam : string (password \t keys (account or userID))
						// mode   : HIWORD(wParam)
#define	mapDLL			0x32		// CString GetNameDLL(int key)
						// key    : HIWORD(wParam) (0 : main screen)
#define	regCODE			0x33		// void cast->registerRTM("CODE1\tCODE2....")

#define	remainDLL		0x34		// void SetRemain(CString data)
						// lParam : data

//
//	variantDLL.command
//
#define	titleCC			0x01		// SetWindowText(text)
#define	codeCC			0x02		// SetHistory(text)
						// text : SYM \t CODE
#define	historyCC		0x03		// char* GetHistory(CString text)
						// text : SYM -> history(code + name \t ...)
#define	accountCC		0x04		// char* GetAccounts(BOOL full)
						// if full, accounts (account \t pass \t \name \t ....\n ...)
						// otherwise accounts(account \t name \n ...)
#define	passCC			0x05		// char* GetPassword(CString account)
#define	triggerCC		0x06		// SetTrigger(CString text)
						// text : SYM \t DATA
						// proc : $PROC \t PARAM
#define	guideCC			0x07		// SetGuide(CString text)
#define	pushCC			0x08		// PushData(CString text)
						// text : SYM \t DATA
#define	popCC			0x09		// char* PopData(CString keys)
#define	homeCC			0x0a		// char* GetHome()
#define	userCC			0x0b		// char* GetUserID()
#define	nameCC			0x0c		// char* GetUserName()
#define	orderCC			0x0d		// bool IsOrderEnable()
#define	confirmCC		0x0e		// bool IsOrderConfirmEnable()
#define	whichCC			0x10		// bool WhichCode(CString code)
#define	prohibitCC		0x11		// bool IsProhibit()
#define	majorCC			0x13		// int GetMajorKey()
#define	minorCC			0x14		// int GetMinorKey()
#define	accountCCx		0x15		// char* GetAccount(CString symbol)
						// default item \t (item \t display \n) * N
#define	accountCCs		0x16		// SetAccount(CString text)
						// text : symbol \t data
#define	acnameCC		0x17		// char* GetAccountName(CString accs)
#define	passCCx			0x18		// char* GetLoginPassword()
#define	systemCC		0x19		// char* GetSystemID()
						// return "N" or "M"
#define	codeCCx			0x1e		// RemoveHistory(text)
//
//	eventDLL.ev
//
#define	evOnClick		11		// OnClick()
#define	evOnChange		12		// OnChange()
#define	evOnDblClk		13		// OnDblClk()

//
//	encPASSx.mode
//
#define	modeACN			0		// encrypt data(password...) by account no.
#define	modeUID			1		// encrypt data(password...) by user ID

//
//	userTH
//
struct	_userTH	{
	char	trc[8];
	char	key;
	char	stat;
};
#define	L_userTH	sizeof(struct _userTH)

//
//	userTH.stat
//
#define	US_ENC			0x01
#define	US_OOP			0x02
#define	US_PASS			0x04
#define	US_CA			0x08
#define	US_KEY			0x10		// userTH + DATAs[0] ... DATAs[0] -> user TRx key

//
//	_extTH
//
struct	_extTH {
	char	key;				// user-defined key
						// DLL : userTH.key
						// CONTROL : US_KEY, SYM[0]
	int	size;				// DATA size
};
#define	L_extTH		sizeof(struct _extTH)

//
//	fontR
//
struct	_fontR {
	char*	name;				// font face name
	INT_PTR	point;				// font point
	bool	italic;				// italic
	INT_PTR	bold;				// bold
};

//
//	penR
//
struct	_penR {
	COLORREF clr;				// pen color
	INT_PTR	width;				// pen width
	INT_PTR	style;				// pen style
};

//
//	DLL Dispatch helper ID (WM_USER)
//	LOBYTE(LOWORD(WPARAM))
//
#define	DLL_INB			0x01		// char* Read() for DLL I/O controls
#define	DLL_OUB			0x02		// void Write(pBytes, nBytes)
						// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
						// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
#define	DLL_OUBx		0x12		// void Write(pBytes)
						// pBytes : _extTH + DATA
#define	DLL_ALERT		0x03		// OnAlert(CString update, int stat)
						// HIWORD(WPARAM) : stat
						// update : if control DLL,  DATAs
						//              otherwise  CODE \t DATAs
#define	DLL_ALERTx		0x13		// OnAlert(void* data)
						// data : struct _alertR*

#define	DLL_TRIGGER		0x04		// OnTrigger(CString text, bool stat)
						// HIBYTE(LOWORD(WPARAM)) : key
						// HIWORD(WPARAM) : trigger stat
						// trigger stat(true)  -> text : PROC \t PARAM
						//             (false) -> text : SYM \t DATA
#define	DLL_DOMINO		0x05		// SetDomino(CString text, bool stat)
						// LPARAM : text, SYM \t DATA (\n ... )
						// HIWORD(WPARAM) : domino stat
#define	DLL_NOTICE		0x06		// OnNotice(CString text)
#define	DLL_DROP		0x07		// OnDrop(CString text)
						// text : point.x \t point.y \n SYM \t DATA (\t ...)
						// LOBYTE(HIWORD(WPARAM)) : source window ID
						// HIBYTE(HIWORD(WPARAM)) : destination window ID
#define	DLL_AXIS		0x08		// OnAxis(key, string)
						// HIBYTE(LOWORD(WPARAM)) : FEV_AXIS.key
						// HIWORD(WPARAM) : window key
#define	DLL_EXCEL		0x09		// make excel data
#define	DLL_INVOKE		0x0a		// int OnInvoke(int wParam, int lParam)
						// HIWORD(wParam) : invoke helper ID
#define	DLL_SETPAL		0x0b		// void SetPalette()
#define	DLL_SETFONT		0x0c		// void SetFont(int point, CString name)
						// set font only. not resize screen
						// HIWORD(WPARAM) : font point
						// LPARAM : font name, if empty, apply point only
#define	DLL_SETFCB		0x0d		// void SetBlink(bool blink, int flash)
						// HIWORD(WPARAM) == true, blink otherwise not blink
						// flash : blink color palette index
#define	DLL_SELECT		0x0e		// OnSelect(CString maps)
#define	DLL_SETFONTx		0x0f		// void SetFont(int point, CString name)
						// set font & resize screen
						// HIWORD(WPARAM) : font point
						// LPARAM : font name, if empty, apply point only
#define	DLL_ACCOUNT		0x10		// update account combo items
#define	DLL_GUIDE		0x11		// BOOL SetGuide(CString guide)


//
//	DLL_ALERT.stat, DLL_ALERTx.stat
//
#define	alert_TICK		0x0f		// tick index data (KOSPI, KOSDAQ, K200, FUTURE, etc)
#define	alert_SCR		0x10
#define	alert_DEIN		0x40

#define	maxREC			256
#define	maxSYMBOL		1000

//
//	DLL_ALERTx
//
struct	_alertR	{
	CString	code;
	int	stat;				// DLL_ALERT.stat
	int	size;				// data record count
	DWORD_PTR ptr[maxREC];			// data record array
};
#define	L_alertR	sizeof(struct _alertR)

//
//	DLL_DOMINO.stat
//
#define	ddNormal		0		// normal domino data (history...)
#define	ddPopup			1		// OnPopupEvent... special window (special data)

//
//	DLL Controls
//	axCreate(CWnd* parent, void* param)
//
//	void* param
//
struct	_param {
	INT_PTR		key;			// LOWORD(key) : screen key, HIWORD(key) : input size
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int			point;			// font point
	int			style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

#pragma	pack()

#endif