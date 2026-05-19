// Screen.h: interface for the CScreen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCREEN_H__F38BF95F_1AE9_4A37_8068_ACFCF31D4C95__INCLUDED_)
#define AFX_SCREEN_H__F38BF95F_1AE9_4A37_8068_ACFCF31D4C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../dll/form/amform.h"
#include "../dll/form/ambase.h"

#define	waitSN			0x00000001
#define	reptxSN			0x00000002

struct	_flashK {
	int	index;
	int	color;
	int	key;
	DWORD	time;
};

struct	_blink {
	class CamBase*	form;
	DWORD	time;
	int	color;
};

#define	floatSUM		0x10000

class CProfit
{
public:
	CProfit()		{}
	virtual ~CProfit()	{ m_sum.RemoveAll(); m_cell.RemoveAll(); }

	CString		m_name;

	double		m_fee;
	double		m_tax;
	double		m_money;
	bool		m_charge;

	CamBase*	m_fees;
	CamBase*	m_taxs;
	CamBase*	m_moneys;
	CamBase*	m_charges;

	CMap	<char, char, int, int&>	m_cell;
	CMap	<int, int, class CamBase*, CamBase*> m_sum;
};


class CScreen : public CAmForm
{
public:
	CScreen(class CClient* client, CRect rect);
	virtual ~CScreen();

public:
	class CClient*	m_client;
	class CGuard*	m_guard;
	CRect			m_rect;
	int				m_parent;
	int				m_object;
	int				m_trxK;
	unsigned char	m_trxS;

	UINT			m_state;
	int				m_tabL;
	int*			m_tabR;

	bool			m_skip;
	CMapStringToOb	m_cells;		// CPtrArray()

	int		m_ioL;
	int*		m_ioR;
	map<string, string>			m_MLEN;
	map<string, CamBase*>			m_MTR;
	vector<string>				m_vINFO;
	vector<string>				m_vOUT;
	vector<int>				m_vGridLen;

	class CLedger*	m_ledger;
	int		m_ledgerL;
	class CxScreen*	m_xscreen;
	class CScriptEngine*	m_vbe;

protected:
	CMapWordToOb	m_flashObs;		// flash FORMs
	CMap	<int, int, _flashK, _flashK &>	m_revObs;
	CMap	<int, int, _blink, _blink &>	m_blink;

	int		m_row;
	CamBase*	m_notice;
	CObArray	m_notices;		// FM_CONTROL : receive notice data
	CProfit*	m_profit;
	CamBase*	m_sales;
	CamBase*	m_push;

	CCriticalSection	m_sync;

private:
	DWORD		m_tick;
	bool		m_color;

// Operations
public:
	// LUA Method
	// -- FORM
	void	LuaSend(long target, CString strTR);
	lua_State* LuaGetObj();
	void	setFormName(CString sFormName);
	void	setFormMain(CScreen* pScreen);
	void	setParentObj(lua_State* L);
	// -- SYSTEM
	void	SetPopData(CString data);

	BOOL	isMain()	{ return !m_key; }
	BOOL	isUob()		{ return (m_mapH->trxC[0] == '.' || m_mapH->trxC[0] == '*'); }
	BOOL	isFlash()	{ return (m_mapH->options & OP_FLASH); }
	bool	isNotice()	{ return (m_notice || m_notices.GetSize() > 0) ? true : false; }

	bool	Attach(CString mapN);
	bool	Parse(bool resize = false);
	void	ReAttach(CRect rect);
	void	OnVirtual(char* mapN, int& tab);

	void	SetForm(int idx, bool focus);
	bool	SetObject(int key, char *mapN);

	bool	isPush()	{ return (m_push < 0) ? false : true; }
	bool	isPush(CString& pushN);
	void	OnPush(CString pushs);

	bool	OnAlert(CString code, class CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR = NULL);
	bool	OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString notices);
	void	OnProfit(CamBase* form);
	void	Blink();
	void	Blink(CamBase* form, BOOL enable, int color);
	void	OnClear();

	void	ChangeTR(char *trxC);
	void	ChangeTR(int toPath)	{ m_mapH->toPath = toPath; }

	bool	isKeyButton(WPARAM wParam);
	bool	OnTRAN(bool byKey);
	bool	OnChange(int idx, bool byKey);
	void	OnClick(CamBase* form, int result);
	void	SetCells(CamBase* form, CString text, bool update = true);
	void	GuideLedger();
	void	SetLedger(char* ledger = NULL);
	void	SetLedger(int pos, int length, CString item);
	void	GetLedger(char* ledger);
	CString	GetLedger(int pos, int length);
	void	EnableForm(bool enable = true);
	void	SetPalette(bool force = false);
	void	SetControl(CString data);
	void	SetFCB(bool blink, int flash);
	void	OnFile(BOOL upload, char* datB, int datL);

	void	DoEvent(int ev, CString syms);
	long	DoSomething(int type, CamBase* form, WPARAM wParam = 0, LPARAM lParam = 0);
	int		DoComboBox(CamBase* form);
	void	DoProc(CString procs, CamBase* form, CString data, int count = -1);
	bool	OnControl(HWND hWnd, int& key);
	bool	isPurge();
	bool	GetFileExist(CString sName);


// Implementation
protected:
	void	UpdateRTM(int key, CString code, class CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR);
	void	ScrollRTM(CObArray* obs);
	bool	FlashGrid(int key, CString code, CdataSet *fms);
	bool	FlashSemi(int key, CdataSet *fms, CString code = _T(""));

	int		GetFlashBaseKey(int index);
	bool	isFlash(int isEdit, CString string, CString text);
	bool	GetValue(int isEdit, CString text, double &value);
	void	ResetRTM(CString name);
	void	Order(CamBase* form, int order);
};

#endif // !defined(AFX_SCREEN_H__F38BF95F_1AE9_4A37_8068_ACFCF31D4C95__INCLUDED_)
