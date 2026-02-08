#pragma once
// fxImgButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxImgButton window

class CfxImgButton : public CWnd
{
// Construction
public:
	CfxImgButton(CFont* pFont = NULL);
	virtual ~CfxImgButton();

// Attributes
public:	
	bool	m_capture, m_bChecked, m_bCheck, m_bStretch, m_bFocus, m_bDrawFocus;
	BOOL	m_bHover, m_bTracking;

	CBitmap*	m_Bitmap;
	CBitmap*	m_Bitmapdn;
	CBitmap*	m_Bitmaphv;

	BOOL	m_bLClick;

protected:
	CFont	*m_pFont;
	std::unique_ptr<CToolTipCtrl>	m_pTipCtrl;

	HCURSOR	m_hCursor;

private:
	BOOL	m_bText;
	COLORREF m_clText;
	CPoint	m_posLClick;

// Operations
public:
	BOOL	Create(LPCTSTR caption, const RECT& rect, CWnd* parent, UINT nID, bool bCheck = FALSE, bool bStretch = FALSE);
	void	SetCursor(HCURSOR hcursor) { m_hCursor = hcursor; }
//	void	SetImgBitmap(HBITMAP hBitmap, HBITMAP hBitmap_dn, HBITMAP hBitmap_hv);
	void	SetImgBitmap(CBitmap* pBitmap, CBitmap* pBitmap_dn, CBitmap* pBitmap_hv);
	void	SetFont(CFont* pFont, BOOL bRedraw = true);
	void	SetCheck(bool bCheck = true);
	void	SetDrawFocus(bool bDraw = true);

	bool	IsChecked();

protected:
	CString	Parser(CString &srcstr, CString substr);
	CSize	BitmapSize(HBITMAP hBitmap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxImgButton)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetText(LPCTSTR caption, LPCTSTR tooltip);
	BOOL	IsShowText();
	void	ShowText(BOOL bShowText);
	void	CreateTooltip();
	void	DrawStretchByMask(CDC *pDC, CBitmap *pBitmap, CRect bRc, COLORREF maskcolor);

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxImgButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};