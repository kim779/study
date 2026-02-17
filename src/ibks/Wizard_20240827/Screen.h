// Screen.h: interface for the CScreen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCREEN_H__F38BF95F_1AE9_4A37_8068_ACFCF31D4C95__INCLUDED_)
#define AFX_SCREEN_H__F38BF95F_1AE9_4A37_8068_ACFCF31D4C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "../dll/form/axform.h"
#include "../dll/form/fmBase.h"

#define	waitSN			0x00000001
#define	reptxSN			0x00000002

struct	_flashK {
	int	index;
	int	color;
	int	key;
	ULONGLONG	time;
};

struct	_blink {
	class CfmBase*	form;
	ULONGLONG	time;
	int		key;
	int		color;
	int		rgb;
};

#define	floatSUM		0x10000

class CProfit
{
public:
	CProfit()		{}
	virtual ~CProfit()	{ m_sum.RemoveAll(); m_cell.RemoveAll(); }

	CString		m_name = _T("");

	double		m_fee = 0;
	double		m_tax = 0;
	double		m_money = 0;
	bool		m_charge = 0;

	CfmBase*	m_fees = nullptr;
	CfmBase*	m_taxs = nullptr;
	CfmBase*	m_moneys = nullptr;
	CfmBase*	m_charges = nullptr;

	CMap	<char, char, int, int&>	m_cell;
	CMap	<int, int, class CfmBase*, CfmBase*> m_sum;
};

class CtranItem
{
public:
	CtranItem();
	virtual	~CtranItem();

	class	CfmBase* m_form;	// for form
	int	m_size;
	int	m_type;
	int	m_col;			// for column
};


class CScreen : public CAxisForm
{
public:
	CScreen(class CClient* client, CRect rect);
	virtual ~CScreen();

public:
	class CClient*	m_client;
	class CGuard*	m_guard;
	int		m_parent;
	int		m_object;
	int		m_trxK;
	unsigned char	m_trxS;

	UINT		m_state;
	int		m_tabL;
	int*		m_tabR;

	bool		m_skip;
	CMapStringToOb	m_cells;		// CPtrArray()

	int		m_ioL;
	int*		m_ioR;

	CCmdTarget*	m_ledger;
	int		m_ledgerL;
	class CScriptEngine*	m_vbe;
	class	CxScreen*	m_xscreen;

	CPtrArray	m_ins;
	CPtrArray	m_outs;

	CMapStringToOb	m_icells;
	CMapStringToOb	m_ocells;

protected:
	CMapWordToOb	m_flashObs;		// flash FORMs
	CMap	<int, int, _flashK, _flashK &>	m_revObs;
	CMap	<int, int, _blink, _blink &>	m_blink;

	int		m_row;
	CfmBase*	m_notice;
	CObArray	m_notices;		// FM_CONTROL : receive notice data
	CProfit*	m_profit;
	CfmBase*	m_sales;
	CfmBase*	m_push;

	CCriticalSection m_sync;

private:
	ULONGLONG	m_tick;
	bool		m_color;
	CMapStringToPtr	m_objects;

// Operations
public:
	BOOL	isMain()	{ return !m_key; }
	BOOL	isUob()		{ return (m_mapH->trxC[0] == '.' || m_mapH->trxC[0] == '*'); }
	BOOL	isFlash()	{ return (m_mapH->options & OP_FLASH); }
	bool	isNotice()	{ return (m_notice || m_notices.GetSize() > 0) ? true : false; }

	bool	Attach(CString mapN);
	bool	Parse(bool resize = false, bool fix = false);
	void	ReAttach(CRect rect);
	void	OnVirtual(char* mapN, int& tab);

	void	SetForm(int idx, bool focus);
	bool	SetObject(int key, char *mapN, bool force = false);

	bool	isPush()	{ return (m_push < 0) ? false : true; }
	bool	isPush(CString& pushN);
	void	OnPush(CString pushs);

	bool	OnAlert(CString code, CString update, class CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR = NULL);
	bool	OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString notices);
	void	OnProfit(CfmBase* form);
	void	Blink();
	void	Blink(CfmBase* form, BOOL enable, int color);
	void	Blink(CfmBase* form, int key, BOOL enable, int color);
	void	OnClear();

	void	ChangeTR(char *trxC);
	void	ChangeTR(int toPath)	{ m_mapH->toPath = toPath; }

	bool	isKeyButton(WPARAM wParam);
	bool	OnTRAN(bool byKey);
	bool	OnChange(int idx, bool byKey);
	void	OnClick(CfmBase* form, int result);
	void	SetCells(CfmBase* form, CString text, bool update = true, bool grid_ini = false);
	void	Reset();
	void	GuideLedger();
	void	SetLedger(char* ledger = NULL);
	void	SetLedger(int pos, int length, CString item);
	void	GetLedger(char* ledger);
	CString	GetLedger(int pos, int length);
	void	EnableForm(bool enable = true);
	void	SetPalette();
	void	SetControl(CString data);
	void	SetFCB(bool blink, int flash);
	void	OnFile(BOOL upload, char* datB, int datL);

	void	DoEvent(int ev, CString syms);
	long	DoSomething(int type, CfmBase* form, WPARAM wParam = 0, LPARAM lParam = 0, int wpL = 0, int lpL = 0);
	int	DoComboBox(CfmBase* form);
	void	DoProc(CString procs, CfmBase* form, CString data, int count = -1);
	bool	GetHistory(CString name, CString& codes, bool up = true);
	BOOL	IsResizable();
	int	GetHeight();
	int	GetWidth();

	bool	OnControl(HWND hWnd, int& key, bool force = false);
	int	ParseRCC(CfmBase* form, char* datB, int index);
	bool	isPurge();

	bool	GetTranInfo(CString trx);
	void	ClearTranInfo();

	int	CreateObject(CString name, CCmdTarget*& target);

// Implementation
protected:
	void	UpdateRTM(int key, CString code, CString update, class CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR);
	void	ScrollRTM(CObArray* obs);
	bool	FlashGrid(int key, CString code, CdataSet *fms);
	bool	FlashSemi(int key, CdataSet *fms, CString code = _T(""));

	int	GetFlashBaseKey(int index);
	bool	isFlash(int isEdit, CString string, CString text);
	bool	GetValue(int isEdit, CString text, double &value);
	void	ResetRTM(char* name);
	void	Order(CfmBase* form, int order);

	void	GetTranInfo(CString path, CPtrArray& forms, CMapStringToOb& cells, bool input);
};

#endif // !defined(AFX_SCREEN_H__F38BF95F_1AE9_4A37_8068_ACFCF31D4C95__INCLUDED_)
