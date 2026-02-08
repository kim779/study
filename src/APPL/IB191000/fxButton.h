#pragma once

class CfxButton : public CButton
{
public:
	CfxButton(CFont* pFont = NULL);
	virtual ~CfxButton();
	
	//{{AFX_VIRTUAL(CfxButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	BOOL	Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	Init(DWORD style, CString text = _T(""), CString images = _T(""),
				COLORREF clrBACK = GetSysColor(COLOR_3DFACE), 
				COLORREF clrFORE = GetSysColor(COLOR_BTNTEXT),
				COLORREF clrTOGGLE = GetSysColor(COLOR_BTNTEXT));
	void	SetStyle(DWORD style, bool redraw = true);
	void	SetText(CString text, bool redraw = true);
	void	SetBitmap(CString path, bool redraw = true);
	void	SetBkColor(COLORREF clrBACK, bool redraw = true);
	void	SetFgColor(COLORREF clrFORE, bool redraw = true);
	void	SetDisableBkColor(COLORREF clrDISBACK, bool redraw = true);
	void	SetDisableFgColor(COLORREF clrDISFORE, bool redraw = true);
	void	SetToggleColor(COLORREF clrTOGGLE, bool redraw = true);
	void	SetFont(CFont* pFont, bool redraw = true);
	void	SetToggleStatus(bool bPress, bool redraw = true);
	bool	GetToggleStatus() { return m_bPress;	}
	void	ModifyBtnStyle(DWORD remove, DWORD add, bool redraw = true);
private:
	void	drawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor);

protected:
	CBitmap* m_bitmap;
	COLORREF m_clrBACK;
	COLORREF m_clrFORE;
	COLORREF m_clrTOGGLE;
	COLORREF m_clrDISBACK;
	COLORREF m_clrDISFORE;
	DWORD	m_style;
	CString	m_text;
	CFont*	m_pFont;

	bool	m_bPushed;
	CWnd*	m_parent;
	bool	m_bPress;	// use toggle
	

protected:
	//{{AFX_MSG(CfxButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

