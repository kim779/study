#pragma once
#pragma pack(1)

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
#define	sinjuCODE		109			// type 0: all, 1: 수익증권(하일리펀드), 2: ELW, 
									//	3: 신주인수권, 4: 수익증권 + 신주인수권

#define	thirdCODE		110
#define	getCODEfuture	111			// FUTURE code
#define	getCODEcall		112			// ATM CALL option
#define	getCODEput		113			// ATM PUT option
#define	ecnCODE			114			// ECN code
#define faoptionCODE	115			// future + option + kospi option
#define all2CODE		116			// kospi + option
#define	usCODE			117			// overseas code
#define	spreadCODE		118			// Spread
#define	kkCODE			119			// kospi + kosdaq

#define	isCODEtype		256			// isCodetype(code, type)

//
//	Wizard Fire Dispatch   ... WM_USER
//	LOWORD(WPARAM)
//			pBytes ... LPARAM
//			nBytes ... HIWORD(WPARAM)
//
#define	getPALETTE		0x01		// PALETTE GetPalette(pBytes)
#define	invokeTRx		0x02		// InvokeTRx(pBytes, nBytes)
						// pBytes = userTH + DATAs, nBytes = DATA size
#define	systemTRx		0x03		// SystemTRx(pBytes, nBytes)
						// pBytes = userTH + DATAs, nBytes = DATA size
#define	viewDLL			0x04		// SetView(int type, CString lParam)
						// HIWORD(wParam) = type
						// lParam = MAPs(8) + OPTIONs + domino DATAs ("/d SYM \t DATA ...")
						// OPTIONs : "/t /S /s /K /k /p /d" (trigger, search, key, pos, domino)
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
#define codeTYPE		0x0c		// GetMarketType(CString strCode)
						// lParam : code
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
						// HIBYTE(HIWORD(wParam))) : ev (11:Click, 12:evChange, 13:DblClk)
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
						// mode   : HIWORD(kind) (0 : account, 1 : userID)

#define mapDLL			0x32		// CString GetNameDLL(int key) 
						// key    : HIWORD(wParam) (0 : main screen)

/////////////////////////////////////////////////////////////////////////
// Add by Mystery (2004.07.08)
#define	NEW_FCODE_CHECK	0x22		// 선물 최근월물 코드값을 얻는다.
#define	ReqCodeBackData	0x23		// 수 시세부 조회데이터 Key값
#define chartOrderSend  0x24		// 수 자동주문 Key값
/////////////////////////////////////////////////////////////////////////


//
//	variantDLL.command
//
#define	titleCC			0x01		// SetWindowText(text)
#define	codeCC			0x02		// SetHistory(text)
						// text : SYM \t CODE
#define	historyCC		0x03		// char* GetHistory(CString text)
						// text : SYM -> history(code + name \t ...)
#define	accountCC		0x04		// char* GetAccounts()
						// accounts(account \t name \n ...)
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

//
//	userTH
//
struct	_userTH	{
	char	trc[8]{};
	char	key{};
	char	stat{};
};
#define	L_userTH	sizeof(struct _userTH)

//
//	userTH.stat
//
#define	US_ENC			0x01
#define	US_OOP			0x02
#define	US_PASS			0x04
#define	US_CA			0x08
#define US_KEY			0x10

//
//	fontR
//
struct	_fontR {
	char*	name{};				// font face name
	int	point{};				// font point
	bool	italic{};				// italic
	int	bold{};				// bold
};

//
//	penR
//
struct	_penR {
	COLORREF clr{};				// pen color
	int	width{};				// pen width
	int	style{};				// pen style
};

//
//	DLL Dispatch helper ID (WM_USER)
//	LOBYTE(LOWORD(WPARAM))
//
#define	DLL_INB			0x01		// char* Read()
#define	DLL_OUB			0x02		// void Write(pBytes, nBytes)
						// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
						// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
#define	DLL_ALERT		0x03		// OnAlert(CString update, int stat)
						// HIWORD(WPARAM) : stat
						// update : if control DLL,  DATAs
						//              otherwise  CODE \t DATAs
#define	DLL_TRIGGER		0x04		// OnTrigger(CString text, bool stat)
						// HIBYTE(LOWORD(WPARAM)) : key
						// HIWORD(WPARAM) : trigger stat
						// trigger stat(true)  -> text : PROC \t PARAM
						//             (false) -> text : SYM \t DATA
#define	DLL_DOMINO		0x05		// SetDomino(CString text)
						// text : SYM \t DATA (\n ... )
#define	DLL_NOTICE		0x06		// OnNotice(CString text)
#define	DLL_DROP		0x07		// OnDrop(CString text)
						// text : point.x \t point.y \n SYM \t DATA (\t ...)
						// LOBYTE(HIWORD(WPARAM)) : source window ID
						// HIBYTE(HIWORD(WPARAM)) : destination window ID
#define	DLL_AXIS		0x08		// OnAxis(key, string)
						// HIBYTE(LOWORD(WPARAM)) : FEV_AXIS.key
						// HIWORD(WPARAM) : window key
#define	DLL_INVOKE		0x0a		// int OnInvoke(int wParam, int lParam)
						// HIWORD(wParam) : invoke helper ID
#define	DLL_SETPAL		0x0b		// void SetPalette()
#define	DLL_SETFONT		0x0c		// void SetFont(int point, CString name)
						// HIWORD(WPARAM) : font point
						// LPARAM : font name, if empty, apply point only
#define	DLL_SETFCB		0x0d		// void SetBlink(bool blink, int flash)
						// HIWORD(WPARAM) == 1, blink otherwise not blink
						// flash : blink color palette index
#define	DLL_SELECT		0x0e		// OnSelect(CString maps)

//
//	DLL_ALERT.stat
//
#define	alert_SCR		0x10
#define	alert_DEIN		0x40


//
//	DLL Controls
//	axCreate(CWnd* parent, void* param)
//
//	void* param
//
struct	_param {
	int		key{};				// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point{};				// font point
	int		style{};				// font style
	DWORD		tRGB{};			// text color index
	DWORD		pRGB{};			// paint color index
	CString		options;		// properties
};

#pragma pack()
