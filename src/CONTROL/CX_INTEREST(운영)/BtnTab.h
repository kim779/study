#pragma once
// BtnTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBtnTab window

class CBtnTab : public CTabCtrl
{
// Construction
public:
	CBtnTab();

// Attributes
public:
	CFont		*m_pFont, *m_pBfont;
	COLORREF	m_bkColor, m_frColor, m_selectedbColor, 
			m_selectedfColor, m_backColor;

	HBITMAP		m_bitmapT_nm, m_bitmapT_dn, m_bitmapT_hv;

private:
	BOOL		m_bHover, m_bTracking;
	int		m_nHover;

// Operations
public:
	void	SetFont(CFont* pFont, bool redraw);
	
	void	SetColor(COLORREF fcolor, COLORREF bcolor, COLORREF sfcolor, COLORREF sbcolor, COLORREF backColor);
	void	OnDraw(CDC *pDC);

private:
	LOGFONT GetLogFont(int fsize, DWORD weight);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtnTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBtnTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBtnTab)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
};