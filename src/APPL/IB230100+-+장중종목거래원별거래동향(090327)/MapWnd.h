#pragma once
// MapWnd.h : header file
//

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_grid/fxgrid.h"

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
	// Construction
public:
	CMapWnd(CWnd* parent);

	// Attributes
public:
	CWnd* m_parent;

private:
	std::unique_ptr<CfxStatic> m_panel{};
	std::unique_ptr<CfxStatic> m_contentspanel{};
	std::unique_ptr<CfxStatic> m_left{};
	std::unique_ptr<CfxStatic> m_top{};
	std::unique_ptr<CfxStatic> m_right{};
	std::unique_ptr<CfxStatic> m_bottom{};
	std::unique_ptr<CfxStatic> m_center{};

	std::unique_ptr<CfxCodeCtrl> m_code{};
	std::unique_ptr<CfxImgButton> m_searchBN{};
	std::unique_ptr<CfxImgButton> m_stopBN{};
	std::unique_ptr<CfxImgButton> m_inqBN{};

	std::unique_ptr<CfxGrid> m_tbl1{};
	std::unique_ptr<CfxGrid> m_tbl2{};
	std::unique_ptr<CfxStatic> m_label{};
	std::unique_ptr<CfxStatic> m_schg{};
	std::unique_ptr<CfxStatic> m_svol{};
	std::unique_ptr<CfxStatic> m_bvol{};
	std::unique_ptr<CfxStatic> m_bchg{};

	std::unique_ptr <CfxGrid> m_grid{};
	CWnd* m_symbol{};
	HINSTANCE	m_inst;

	CFont* m_font{}, * m_fontb{};
	CFont* m_sfont{};
	COLORREF	m_clrBackIn{}, m_clrContentsBackIn{};
	COLORREF	m_clrBack{};

	CString		m_keys;
	CString		m_keyx;
	CString		m_root;
	CString		m_string;

	CString		m_fsvol;
	CString		m_fbvol;

	CCriticalSection m_sync;
	bool		m_stop{};

	CMapStringToPtr	m_rts;

	int		m_rows{};
	int		m_sort{};
	bool		m_descend{};
	HBITMAP		m_hRoundBitmap[4]{};
	HBITMAP m_hBmpRoundPanel[4]{};
	HBITMAP m_hBmpRoundContents[4]{};

	//2011.11.22 KSJ
	bool	m_bFirst{};	//dispatch에 처음들어간지 여부
	CString	m_strCode;	//현재 코드

	class CContainerWnd* m_pMarketpicker{};
// Operations
public:
	BOOL	Create();
	CString		Parser(CString& srcstr, CString substr);

protected:
	BOOL	create(CWnd* parent);

	void	sendTR();
	void	sendTR(CString trN, int key, char* datb, int datl, bool oop = false);
	void	dispatch(int key, char* datb, int datl);
	void	alert(CString data, WPARAM wParam);
	void	alertx(WPARAM wParam, LPARAM lParam);	//2012.02.15 KSJ
	void	inquiry(bool trigger = true);

	//2011.11.23 KSJ
	void	FirstSearch(int key, char* datb, int datl);
	void	AlertSearch(int key, char* datb, int datl);
	// KSJ

	int	parse(CString text, CStringArray& arr);
	int	parse(CString text, CMapStringToString& maps);
	CString	setPercent(CString data);

	void	doWait(BOOL wait = TRUE);
	void	setStaticColor(CfxStatic* pWnd, COLORREF bgclr, COLORREF fgclr, COLORREF bdclr);

	COLORREF getColor(int color);
	CFont*	getFont(int point, CString name = _T(""), int bold = FW_NORMAL, bool italic = false);
	HBITMAP	getBitmap(CString bmps);
	long	variant(int key, CString data = _T(""));
	void	openView(CString maps, CString data);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBitmap();
	CString m_strPal;
	CString GetPalette();
	virtual ~CMapWnd();

	// Generated message map functions
	CString m_sMarket{};
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnMsgMKDLL(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

