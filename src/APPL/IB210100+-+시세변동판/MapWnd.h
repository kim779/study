#pragma once
// MapWnd.h : header file
//

#include "../../control/fx_misc/fxStatic.h"
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
	CWnd*	m_parent;

private:
	BOOL		m_bCreated{};
	BOOL		m_bFont{};
	int			m_nFontSize{};
	std::unique_ptr<CfxStatic> m_panel{};
	std::unique_ptr<CfxStatic> m_panel2{};

	std::unique_ptr<CfxImgButton> m_setupBN{};
	std::unique_ptr<CfxImgButton> m_stopBN{};
	std::unique_ptr<CfxGrid>      m_grid{};

	CFont*		m_font{};
	COLORREF	m_clrBack{};
	COLORREF	m_clrData{};
	COLORREF	m_clrFocus{};

	CString		m_root;
	CString		m_user;
	CString		m_string;
	CString m_strPal;

	CCriticalSection m_sync;
	bool		m_stop{};
	bool		m_on{};

	int		m_row{};
	int		m_rows{};
	int		m_width{};

	std::unique_ptr<class Cconfig> m_config{};

	HBITMAP m_hBmpRoundPanel[4]{};
	HBITMAP m_hBmpRoundContents[4]{};

// Operations
public:
	BOOL	Create();

protected:
	BOOL	create(CWnd* parent);

	void	sendTR(bool clear = false);
	void	dispatch(int key, char* datB, int datL);
	void	alert(CString data);

	void	setup();
	void	setColumns();
	void	loadConfig();
	void	saveConfig();

	void	SetResize();
	COLORREF getColor(int color);
	CFont*	getFont(int point, CString name = _T(""), int bold = FW_NORMAL, bool italic = false);
	HBITMAP	getBitmap(CString bmps);
	long	variant(int key, CString data = _T(""));

	CString GetBuffer(CString sKey);
	void	SaveBuffer(CString sKey, CString sData);
	void	SetBitmap();
	void	Setinsertrow();
	CString	GetPalette();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

