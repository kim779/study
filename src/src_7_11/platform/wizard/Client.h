// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__42AA262B_5FCB_47F5_831F_BFE969586BC1__INCLUDED_)
#define AFX_CLIENT_H__42AA262B_5FCB_47F5_831F_BFE969586BC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Works.h"
#include "Screen.h"
#include "Misc.h"

#define	TM_RTM			1		// real-time
#define	TM_WAIT			2		// wait timeout
#define	TM_REPBN		3		// repeat-mouse
#define	TM_REPTR		100		// repeat-TR
#define	TM_VB			200		// script timer
#define	TMI_RTM			250
#define	TMI_REPTR		3000

struct	_state	{
	int	key;				// F1 - F12
	int	shift;				// key shift
	int	button;				// mouse button
	int	scroll;				// scroll button
};

//
//	_state.shift
//
#define	SS_NS			0		// none
#define	SS_SS			1		// shift
#define	SS_CS			2		// control
#define	SS_CSS			3		// control+shift

//
//	_state.button
//
#define	MS_NB			0		// none
#define	MS_LB			1		// left button
#define	MS_MB			2		// middle
#define	MS_RB			3		// right

//
//	_state.scroll
//
#define	SB_NS			0		// none
#define	SB_UP			1		// up
#define	SB_DN			2		// down
#define	SB_IM			3		// item
#define	SB_HD			4		// head
#define	SB_PU			5		// page up
#define	SB_PD			6		// page down
#define	SB_FR			7		// first button
#define	SB_FD			8		// tree folder


class CClient : public CWorks  
{
public:
	CClient(CGuard* guard, CWnd* view, int key, int type);
	virtual ~CClient();

public:
	class CStream*	m_stream;
	class CScript*	m_vm;
	class CKey*	m_keyx;
	class CMouse*	m_mouse;

	BOOL		m_focus;
	int		m_magic;
	CSize		m_offset;

	CCaret		m_default;		// default Caret
	CCaret		m_current;		// current Caret
	CCaret		m_cbox;			// control box
						// combo, resize bar, shift keys form
	CCaret		m_src2box;		// caret save for cbox

	CString		m_string;		// system.string
	int		m_value;		// system.value
	bool		m_bool;			// system.bool

	struct	_state	m_vms;			// VM client state
	CWnd*		m_child;

	HBITMAP		m_bitmap;
	COLORREF	m_bkground;
	CCriticalSection	m_sync;
	float		m_hR, m_vR;

protected:
	CMap	<int, int, CScreen*, CScreen*>	m_screen;

	CMapStringToString	m_dominos;
	CMapStringToPtr		m_control;

private:
	CMap	<int, int, CfmBase*, CfmBase*>	m_tips;
	CMap	<int, int, int, int&>		m_keys;
	CString		m_next;
	CString		m_codes;
	CCaret		m_form;
	int		m_cell;

// Operations
public:
	bool	Attach(CString maps, bool only, bool fix = false);
	void	OnAxis(struct _axisH* axisH, char* pBytes, int nBytes);
	void	OnTrigger(CString name, CString text, int key);
	void	OnProcedure(CString proc, CString param, int key = 0);
	bool	isWorks()	{ return true; }
	BOOL	GetAt(CString name, CString& text);
	void	OnDomino(CString domino);
	void	OnDrop(CPoint point, CString text, CWnd* pWnd = NULL);

	void	SetPalette(bool force = false);
	void	SetBK(char *bitmapN, bool update = false);
	void	SetBK(COLORREF rgb, bool update = false);
	void	SetBK();

	bool	SetAtScreen(CScreen*& screen, CRect rect, char* mapN, int key = -1, int parent = -1);
	bool	SetObject(int key, CScreen*& screen, CRect fmRect, char* mapN);
	bool	SetObject(CScreen* main, int key, char *mapN);
	BOOL	GetAtScreen(CScreen*& screen, int key = 0)
		{ return m_screen.Lookup(key, screen); }
	BOOL	GetScreen(CScreen*& screen)
		{ return GetAtScreen(screen, m_current.m_key); }
	void	RemoveAtScreen(int key);

	void	GetKeyObject(CScreen* screen, CfmBase*& form);
	int	GetKeyObject(CScreen* screen);
	void	GetPrevScreen(CScreen*& screen, int& key);
	void	GetNextScreen(CScreen*& screen, int& key);

	void	OnScroll();
	void	OnResize(int Width, int Height);
	void	OnFocus(BOOL focus, HWND hWnd = NULL);
	void	OnControl(HWND hWnd);
	bool	OnControl();
	void	OnFocus(CScreen* screen, int idx, bool event);
	void	OnChange(CScreen* screen);
	void	OnDraw(CDC* pDC);
	void	OnPaint(CDC* pDC, bool isPrint);
	bool	isVisible(CScreen* screen, CfmBase* form);
	bool	isVisible(int key, int idx);
	void	OnPrint(CDC* pDC);

	void	SetCPos(bool event);
	void	SetCPos(CScreen *screen, int pos, bool event = true);

	bool	isPush(CString& pushN);
	void	OnPush(CString pushs);
	void	OnAlert(CString code, class CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR = NULL);
	void	OnNotice(class CdataSet& major, CdataSet& minor, CdataSet& fms, CString text);
	void	OnClear();
	void	SetBlink();

	int	xAlert(CScreen* screen, char* keys);
	void	WhereAmi(CScreen* screen, bool& cursor, bool& focus);
	void	SetRepository(CScreen* screen);
	void	GetRepository(CScreen* screen);
	void	SaveDomino(CString domino);
	void	SaveDomino();
	int	RestoreDomino(bool force = false);
	int	RestoreDomino(CScreen* screen, CMapStringToString& dominos, bool force = false);
	void	CodeDomino(CScreen* screen, CMapStringToString& dominos);

	bool	OnTRAN(bool byKey);
	bool	isKeyButton(WPARAM wParam);
	void	WaitState(CScreen* screen, bool timeout);
	void	WaitDone(CScreen* screen, bool timeout, bool wait = true);
	void	RepeatTR(CScreen* screen);
	void	OnRepeat(int key);

	void	ChangeIME(CScreen* screen, int idx);
	void	ChangeIME(bool ime);

	bool	RunMenu(CScreen* screen, CfmBase* form, CPoint point);
	void	ClearShift();

	void	History(CfmBase* form, int mode);
	void	History(bool apply);
	void	SelfHistory(CfmBase* form);
	void	AutoHistory(CfmBase* form);
	void	CheckJumpByRule(CScreen* screen = NULL);

	void	GetNext(CString& next);
	BOOL	isNext()		{ return !m_next.IsEmpty(); }
	bool	isNext(CScreen* screen, int key);
	void	SetNext();
	void	OnSend(CScreen* screen);

	void	MakeFile(CScreen* screen, CfmBase* form, CString path = _T(""), bool head = true, bool append = false);
	void	MakeXlsFile(CScreen* screen, CfmBase* form, bool visible, CString path = _T(""), bool head = true);
	void	Export(CScreen* screen, CfmBase* form, bool visible, CString data);
	void	OnPrint(CScreen* screen, CfmBase* form, bool print);
	bool	SetCaption(CString maps);

	bool	FindScreen(int parent, CString& name, CScreen*& screen, bool only = false);
	CCmdTarget* GetControl(CString name);
	bool	isChild(int parent, CScreen* screen);
	void	SetFCB(bool blink, int flash);
	void	SetControl(int key, CString text);
	BOOL	SetFont(int point, bool resize, bool attach = false);
	void	OnDevice();
	void	SetLedger(CString maps);
	bool	GetCode(CMapStringToString& domino);

	void	AddTips();
	int	LookTips(CPoint point, CString& text);
	BOOL	isTips()	{ return !m_tips.IsEmpty(); }
	void	OnAccount();
	void	EditColumns(CScreen* screen, CfmBase* form);

	int	GetKey(int skey, int ukey, int index);
	int	GetScreenKey(int key, int& index);
	bool	isPurge();

// Implementation
protected:
	void	OnWrite(CScreen* screen, CfmBase* form, CString text);
	void	FreeBK(bool reset = true);
	void	SetInitial(bool reset = true);
	void	SetCode(CString& codes);
	bool	SetCode(CScreen* screen, CfmBase* form, CString& codes, bool force);
	bool	GetDragCell(CScreen* screen, CfmBase* form, int& col, bool force);
	void	OnStart(CScreen* screen);
	int	GetCodes(CString syms, CString& items, bool self);
	void	GetCodes(CString& codes);
	CString	GetFormat(CScreen* screen, CfmBase* form);
};

LRESULT CALLBACK CallProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // !defined(AFX_CLIENT_H__42AA262B_5FCB_47F5_831F_BFE969586BC1__INCLUDED_)
