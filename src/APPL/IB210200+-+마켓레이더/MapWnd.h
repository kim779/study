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
	CWnd* m_parent{};

private:
	int m_nFontSize{};
	std::unique_ptr <CfxStatic>	m_panel;
	std::unique_ptr <CfxStatic>	m_panel2;

	std::unique_ptr <CfxCodeCtrl>	m_code;
	std::unique_ptr <CfxImgButton>	m_searchBN;
	std::unique_ptr <CfxImgButton>	m_stopBN;
	std::unique_ptr <CfxImgButton>	m_inqBN;
	std::unique_ptr <CfxImgButton>	m_setupBN;
	std::unique_ptr <CfxImgButton>	m_nextBN;
	std::unique_ptr <CfxGrid>	m_grid{};
	CWnd*		m_symbol{};
	HINSTANCE	m_inst{};

	CFont*		m_font;
	COLORREF	m_clrBack{}, m_clrBackIn{};

	CString		m_root;
	CString		m_user;
	CString		m_string;
	CString		m_strPal;

	CCriticalSection m_sync;
	bool		m_stop{};

	int			m_rows{};
	CString		m_file;

	bool		m_bCreated{};
	bool		m_next{};
	CString		m_rts;
	DWORD		m_flags[4]{};
	CString		m_state;			// item 제외조건
	CString		m_sound;
	CString		m_keys;
	CMap	< DWORD, DWORD, class Citem*, Citem* > m_items;
	std::vector<std::unique_ptr<Citem>> _vData;

	BOOL		m_chkVol{};
	CString		m_volS;
	double		m_volVal{};
	CString		m_gubnS;

	HBITMAP m_hBmpRoundPanel[4]{};
	HBITMAP m_hBmpRoundContents[4]{};

// Operations
public:
	BOOL	Create();

protected:
	BOOL	create(CWnd* parent);

	
	void	SaveBuffer(CString sKey, CString sData);
	CString	GetBuffer(CString sKey);
	void	LoadBuffer();
	void	CreateControl();

	void	sendTR(bool all = false);
	void	sendTR(CString trN, int key, char* datb, int datl, bool oop = false);
	void	dispatch(int key, char* datb, int datl);
	void	alert(CString data, WPARAM wParam);
	void	parse(CString data);

	void	inquiry(bool all, bool trigger = true);
	void	setup();
	void	next();

	void	loadConfig();
	int	hexs2int(CString hexs);

	int	insertRow(int row);
	void	removeRow(int row);

	void	doWait(BOOL wait = TRUE);
	void	sound();

	COLORREF getColor(int color);
	CFont*	getFont(int point, CString name = _T(""), int bold = FW_NORMAL, bool italic = false);
	HBITMAP	getBitmap(CString bmps);
	long	variant(int key, CString data = _T(""));
	void	openView(CString code, CString maps);
	bool	checkGubn(CString gubn);

	void ReSize();
	void SetResize(int nFontSize);

	void SetBitmap();
	CString GetPalette();
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strCode;
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

