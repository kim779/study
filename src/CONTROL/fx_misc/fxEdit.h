#pragma once
// fxEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxEdit window

class AFX_EXT_CLASS CfxEdit : public CEdit
{
// Construction
public:
	CfxEdit(CFont* pFont = nullptr);

// Attributes
protected:
	DWORD	m_style;
	CString	m_text;
	COLORREF m_clrBACK;
	COLORREF m_clrFORE;
	CBrush	m_brBACK;
	BYTE	m_alignment;

	CFont*	m_pFont;

	DWORD	m_attr;
	CWnd*	m_parent;
	UINT	m_nID;


// Operations
public:
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	Init(DWORD style, DWORD attr, CString text = _T(""), COLORREF clrBACK = GetSysColor(COLOR_3DFACE),
					COLORREF clrFORE = GetSysColor(COLOR_BTNTEXT), BYTE alignment = 0 /*alCENTER*/);
	void	SetStyle(DWORD style);
	void	SetText(CString text, bool redraw = true);
	CString	GetText();
	void	SetBkColor(COLORREF clrBACK, bool redraw = true);
	void	SetFgColor(COLORREF clrFORE, bool redraw = true);
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetAlignment(BYTE alignment, bool redraw = true);
	void	ModifyAttribute(DWORD attrRM, DWORD attrADD);
	
private:
	void	formatComma(CString& text);
	void	commaText();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEditKillFocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

