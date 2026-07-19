// Guard.h: interface for the CGuard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARD_H__CA65B76E_FEEC_41F4_A715_B915AEE2E7F6__INCLUDED_)
#define AFX_GUARD_H__CA65B76E_FEEC_41F4_A715_B915AEE2E7F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "../h/axisfire.h"
#include "../dll/lib/axislib.h"
#include "Misc.h"
#include "OleDropEx.h"
#include "encrypt/lz4.h"

#define	WM_GUIDE	WM_USER+1
#define	WM_VERS		WM_USER+2
#define	WM_ANM		WM_USER+3

#define WS_SIGN		0x00000001
#define	WS_TRIX		0x00000002
#define	WS_GUIDE	0x00000004
#define	WS_SELF		0x00000010
#define	WS_FCB		0x00000020
#define	WS_NOMSG	0x00000040

#define	trgGROUP	0x01
#define	trgALL		0x02
#define	trgTHIS		0x03
#define	trgVRS		0x10
#define	trgALLS		0x20

#define	menuXLS		10
#define	menuTXT		11
#define	menuGRID	12
#define	menuINIT	13
#define	menuCOPY	14
#define	menuPRINT	15
#define	menuLINK	16
#define	menuNGROUP	17
#define	menuBASE	100
#define	menuGROUP	200

#define	MASTERs		_T("  ")

class Cvers
{
public:
	bool	m_request = true;
	CString	m_vers{};
	CString	m_path{};
};

// for FDS
#pragma pack(1)

struct	_fdsR {
	char	flag{};		// fds flag
	char	rsvB[1]{};	// reserved
	short	fdsL{};		// fds data length
};
#define	L_FDSR	sizeof(struct _fdsR)

#pragma pack()
//

class CGuard
{
public:
	CGuard();
	virtual ~CGuard();
	CString m_slog;
public:
	CWinApp*	m_app;
	CString		m_keys;			// copyrights/registry keys
	CString		m_root;
	unsigned char	m_term;
	UINT		m_status;		// Axis status

	CWnd*		m_sock;
	CWnd*		m_xecure;
	CWnd*		m_certify;
	COleDropEx	m_drop;

	CAxisPalette*	m_palette;		// Axis palette
	CAxisDraw*	m_draw;
	class CTips*	m_tips;

	CString		m_ips;

	CString		m_modals;
	CString		m_repository;
	int		m_flash;		// flash color index
	int		m_wait;

	CString		m_dragbys;		// text

	class CxSystem*	m_xsystem;
	CCmdTarget*	m_login;

	int		m_vid;
	int		m_font;
	bool		m_resize;
	int		m_CODEs;

	CString		m_stock_accs;		// stock account group
	CString		m_future_accs;		// future account group

// updateXX_20210517
	char		m_encID[32]{};		// encrytion id (form main)

protected:
	CWnd*		m_parent;

	class CDde*	m_dde;
	class CObjects*	m_objects;

	CMap	<int, int, class CWorks*, CWorks*> m_clients;

	bool		m_alive;
	int		m_wtype;
	CMapStringToOb	m_codex;		// codes history
	CString		m_master;		// code master

#ifdef DF_RTM_INDEX
	CMapStringToPtr	m_codeIndex;		// RTM shadow index: code -> CPtrArray* of CScreen*
#endif

	CByteArray	m_major;
	UINT		m_trace;
	HWND		m_hTrace;

private:
	CMapStringToString m_combo;		// combo entry
	CStringArray	m_accno;		// account #
	CMapStringToString m_account;		// account entry
	CMapStringToString m_push;		// user push forms

	unsigned char	m_absS;			// abs security
	unsigned char	m_incS[4];		// inc security
	unsigned char	m_excS[4];		// exc security

	CMap	<int, int, CString, CString &>	m_secmt;
						// security message table
	CMapStringToPtr	m_vers;			// screen MAP version

	CCriticalSection m_sync;
	CCriticalSection m_section;
	CString		m_mapN;
	CString		m_user;
	CString		m_uname;

	CString		m_vtcode;
	CString		m_verpass;
	CString		m_version;
	CString		m_userini;

	HINSTANCE	m_symbol;
	bool		(APIENTRY* axGetCode)(int kind, char* user, char* code, int type, CPoint pt);
	bool		(APIENTRY* axGetName)(int kind, char* code, char* name, int* type);
	bool		(APIENTRY* axGetUpInfo)(char* code, char* bizcode, char* bizname, int* market);
	int		(APIENTRY* axGetInfo)(int kind, char* code, char* info, int* value);

	HINSTANCE	m_loginx;
	void*		(APIENTRY* axLogin)(void* data);
	void*		(APIENTRY* axLoginData)(int id);
	void		(APIENTRY* axLoginLedger)(void* ledger);
	void*		(APIENTRY* axLedger)(void* data);
	void		(APIENTRY* axLedgerEx)(void* ledger, void* data);
	void*		(APIENTRY* axGetLedger)(void* ledger, int pos, int length);
	void		(APIENTRY* axSetLedger)(void* ledger, int pos, int length, void* data);
	void*		(APIENTRY* axGetLedgerEx)(void* ledger, int id);
	void*		(APIENTRY* axGetLedgerData)(void* ledger, void* data, int id);

	HINSTANCE	m_dialog;
	void*		(APIENTRY* axDialog)(int kind, char* root, char* src, char* item, char* def);

	HINSTANCE	m_secure;
	char*		(APIENTRY* axEncrypt)(char* src, char* keys, int mode);
	char*		(APIENTRY* axDecrypt)(char* src, char* keys, int mode);

	HINSTANCE	m_secure2;
	char*		(APIENTRY* axEncryptSHA)(char* src, int srcL);

	CMapStringToPtr	m_interests;

	HINSTANCE	m_fds;
	int		(APIENTRY* axFDSValue)(char* trN, char* srcB, int srcL, char* desB) ;

	BYTE		m_trigger;
	CMapWordToPtr	m_cursors;

	CMapWordToOb	m_rtm;
	struct	_alertR	m_alertR;

// updateXXX_encrypt
	 class CLz4	m_lz4;
	 bool		lz4enc(CString& src);
	 bool		lz4dec(CString& src);
public:
	int		lz4dec(char* inB, int inL, char* outB, int outL);

public:
	int	Initial(CWnd* control);
	void	Startup();
	void	Cleanup();
	CWnd*	GetParent()	{ return m_parent; }

	int	Attach(CWnd *view, int type, int key);
	void	Detach(int key);
	int	GetWorks();
	BOOL	GetClient(int key, class CWorks*& works)
		{ return m_clients.Lookup(key, works); }

	void	SetPush(bool push);
	void	OnPush(CString pushs);
	void	OnAlert(char* pBytes, int nBytes);
	void	OnAlert(CString code, char* pBytes, int nBytes, int stat);
	void	OnNotice(char* pBytes, int nBytes);

	BOOL	GetCombo(char* key, CString& combo, bool parse = false);
	void	SetCombo(CString text);
	COLORREF GetRGB(int index)
		{ return m_palette->GetPaletteRGB(index); }

	void	SetVersion(CString mapN);
	void	UpdateVersion(CString name);
	void	UpdateVers(char *verB, int verL, bool runtime = false);
	bool	isVersion(CString mapN);
	bool	isVersion(CString mapN, CString prefix);
	bool	isLevel(CString mapN, CString prefix);

	void	Sign(int signK, char* signB, int signL, CString& dns, bool shop);
	void	GetSignOn(CString& signN, bool state = true);
	void	GetSignName(CString& name)	{ name = m_uname; }
	void	GetBooth(CString& booth);
	void	GetBranch(CString& branch);
	void	GetTerm(CString& term);
	void	GetHistory(CString name, CString& history, bool tab = false);
	bool	isCodes(CString text)		{ return (m_master.Find(MASTERs+text) != -1) ? true : false; }
	void	GetCode(CString name, CString& text);
	void	GetCode(CMapStringToString& domino, class CClient* client = NULL);
	void	GetCode(CString& domino);

	void	SetAccounts(CString entry);
	void	GetAccounts(CString& account);
	void	GetAcName(CString& account, BOOL full);
	bool	GetAcPass(CString key, CString& pass);
	bool	GetAcName(CString key, CString& name, bool full = false);
	void	xAlert(class CClient* client, class CScreen* screen);

	BOOL	Xecure(int helper, char* pBytes, int& nBytes);
	CString	Secure(CString src, CString keys = _T(""), int mode = 0, bool enc = true);
	CString	Secure2(CString src);
	bool	GetCode(int kind, CString& code, int type, CPoint pt = CPoint(-1,-1), bool valid = false);
	bool	GetName(int kind, CString code, CString& name, int& type);
	bool	GetName(int kind, CString code, CString& name);
	bool	GetBizInfo(CString code, CString& bizcode, CString& bizname, int& market);
	int	GetCodeInfo(int kind, CString code, CString& info, int& value);

	void	LoadHistory();
	CString	PathMAP(CString mapN, bool force = false);
	CString	GetByString(CString& src, char by, bool trim = false);

	void	SetCode(class CScreen* screen, bool recur = false, bool self = true);
	void	SetCode(CString text);
	bool	SetCode(CString name, CString text, bool self = true, bool found = false);
	CString	SetCode(CClient* client);
	void	RemoveCode(CString text, bool reload = true);

	void	PushData(class CfmBase* form, CString keys = _T(""), int row = -1, int column = -1);
	void	PushData(CString text);
	void	PopData(CfmBase* form, CString keys = _T(""), int row = -1, int column = -1);
	void	PopData(CString keys, CString& text);
	bool	GetMapH(CString mapN, char* mapH = NULL);
	bool	GetVirtualTR(char *mapN);
	void	RunHelp(char *helpN);

	void	SetRepository(CClient* client);
	void	SetRepository(class CScreen* screen);
	int	GetRepository(CClient* client);
	int	GetRepository(CScreen* screen);

	void	SetTriggerMode(BOOL vrs, int mode);
	void	OnTrigger(CWorks* works, CString text);
	void	OnTrigger(CWorks* works, char* formN, CString text);
	void	OnProcedure(CString proc, CString param, CString maps, int major = 0, int minor = 0);
	bool	IsPopup(CString mapN);

	bool	RouteTR(CClient* client);
	BOOL	Write(char* pBytes, int nBytes, int key);
	BOOL	Write(int msgK, CString trxC, char* datB, int datL, int key = 0, bool trace = false);
//	BOOL	Login(char* datB, int datL, bool xecure);
	BOOL	Login(int mode, char* datB, int datL, bool xecure);
	BOOL	Service(CScreen* screen, CString trxC, char* datB, int datL, int mode);
	BOOL	Approve(CScreen* screen, int key, CString ip, CString map, char* datB, int datL);
	long	UploadFile(CScreen* screen, CString trxC, char* datB, int datL, int mode, CString fileN, int offset);
	BOOL	DownloadFile(CScreen* screen, CString trxC, char* datB, int datL, int mode, CString fileN);
	BOOL	Invoke(char* pBytes, int nBytes, int key);
	BOOL	Write(char* pBytes, int nBytes, bool trace);

	void	PostAxis(WPARAM wParam, LPARAM lParam = 0)
		{ m_parent->PostMessage(WM_USER, wParam, lParam); }
	int	SendAxis(WPARAM wParam, LPARAM lParam = 0)
		{ return m_parent->SendMessage(WM_USER, wParam, lParam); }
	void	PostAxis(int msg, int key, LPARAM lParam);
	int	SendAxis(int msg, int key, LPARAM lParam);
	void	SetGuide(int guide, int key = 0)
		{ m_parent->SendMessage(WM_GUIDE, MAKEWPARAM(key, guide), 0); }
	void	SetGuide(CString guide, int key = 0, int level = 0)
		{ m_parent->SendMessage(WM_GUIDE, MAKEWPARAM(key, level), (LPARAM)guide.operator LPCTSTR()); }

	bool	CheckSecurity(CString mapN, char* curN);
	int	WorkView(CString maps, int* size);
	bool	GetVersion(CString fileN, CString& vers);
	int	GetCodes(CString name, CString& text, bool self);
	CString	GetCodes()	{ return m_master; }
	void	SetGuide(int type, CString guide, int key);
	int	OpenView(CWorks* works, CString maps, int type, int pos = 0);
	void	SetProfit(CScreen* screen, CfmBase* form, class CProfit*& profit);
	bool	SetCells(CString maps, CString& text);
	void	SetPalette();
	void	SetFCB(bool blink, int flash);
	BOOL	SetFont(int point, bool resize = false, int key = -1);

	BOOL	Certify(BOOL force, BOOL certify, BOOL xcertify = FALSE, BOOL xserver = FALSE);
	int	OnCertify(char* pBytes, int nBytes);
	BOOL	CertifyErr(char* pBytes, int nBytes);
	BOOL	Certify(char* pBytes, int& nBytes, CString maps = _T(""));
	void	CertifyId(char* pBytes, bool retry = false);
	void	GetCetifyInfo(CString& infos);
	int CertifyCloude(LONG gubn);

	void	OnDrag(int key, CString text);
	void	MakeXlsFile(CString format, CString text, bool visible, CString fileN, bool userdefine = false);
	void	MakeXlsFile(CString format, CString text);
	void	MakeXlsFile(CString text, bool visible = true, CString fileN = _T(""), bool userdefine = false);
	BOOL	MakeTxtToXls(CString fileN = _T(""), bool visible = false, bool deleteSrc = false);
	BOOL	MakeXlsToTxt(CString fileN = _T(""), bool visible = false, bool deleteSrc = false);

	bool	CreateObject(CString name, CCmdTarget*& target);
	void	OnDDE(CString text, int key);
	void	OnDDE();
	char*	Modal(int kind, CString text);
	bool	Modal(int kind, CString keys, CString& currents, CString defs);
	CCmdTarget* GetLedger(void* data);
	CString	GetLedger(CCmdTarget* ledger, int pos, int length);
	CString	GetLedger(CCmdTarget* ledger, int id);
	CString	GetLedger(CCmdTarget* ledger, char* data, int id);
	void	SetLedger(CCmdTarget* ledger, int pos, int length, char* data);
	void	SetLedger(CCmdTarget* ledger, CString data);
	void	Ledger(char* ledger);
	CString	GetLoginData(int id);

	int	GetKey(int key);
	void	OnRsm(struct _axisH* axisH, char* datB, int datL);
	BOOL	LoadMenu(CString menus);
	void	isMenu(bool force);
	void	AddRegistry(char* datB, int datL, bool cbonly = false);
	void	OnAccount();
	void	xTRACE(int key, HWND hWnd)		{ m_trace = key; m_hTrace = hWnd; }
	void	xTRACE(int type, char* trace, int size = 0);
	BOOL	OnPrint(CString text, CString name, CString fName, int fPoint);
	void	SetClipboard(CString text);
	bool	GetClipboard(CString& text);
	bool	GetString(int code, CString& text);
	CString	GetFileName(CString fileN, CString filter, CString& ext, bool force = false);
	CString	GetLoginPass();
	CString	GetCAPass();
	int	GetFocus();
	int	GetOwner(int key);
	bool	GetCode(CString name, CString& code, bool up);
	void	SetCursor(int id = 0, CWnd* pWnd = NULL);
	HCURSOR	GetCursor(int id);

	void	ClearFocus(int key);
	void	InternalTrigger(CClient* client, CString procs, CString param, CString maps, BOOL setfocus);

	BOOL	IsWait(CClient* client);

	BOOL	IsPortfolioSymbol(CString name);

	bool	GetFdsValue(CString trN, char* datB, int& datL);
	long	CertifyFull(CString srcB, int srcL, char* desB, int& desL);
	long	CertifyName(char* datB);

private:
	int	AtView(CWorks* works, CString maps, CString domino, bool force);
	void	RequestMAPs(CString mapN, Cvers* vers);
	void	AddRegistry(char* datB, int datL, CString& dns);
	void	RemoveRegistry();
	void	SetString(char* datb, class CdataSet* major, CdataSet* minor, CdataSet* fms);

	bool	CheckSecurity(struct _mapH* mapH);
	void	SecurityErrorMsg(int secNo);
	void	LoadSecuritys();
	void	OnFile(struct _fileH* fileH, char* datB, int datL);

	void	DoRTM(CString code, int stat, class CdataSet* fms, CObArray* obs, CString updates);
	CString	MakeString(class CdataSet* fms, bool newline = true);

#ifdef DF_RTM_INDEX
	// RTM code reverse-index (shadow/validation only, does not affect DoRTM behavior yet).
	// Self-updates from CScreen::OnAlert's existing per-tick field read, so no separate
	// write-path hooks are required. oldCode may be empty (first observation for a screen).
	void	UpdateCodeIndex(class CScreen* screen, CString oldCode, CString newCode);
#endif

	size_t	strcspnx(const char* s, const char reject);
	char*	strtokx(char* str, const char delim, char** start);
};

#endif // !defined(AFX_GUARD_H__CA65B76E_FEEC_41F4_A715_B915AEE2E7F6__INCLUDED_)
