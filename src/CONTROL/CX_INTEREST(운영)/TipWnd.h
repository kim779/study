#pragma once
// TipWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipWnd window

class CTipWnd : public CWnd
{
// Construction
public:
	CTipWnd(CWnd* pView, CWnd* pWnd, bool bMap = false);

// Attributes
public:
	CWnd*	m_pView;
	CWnd*	m_pWnd;
	CFont	m_font;
	CRect	m_rect;
	CRect	m_rect2;
	CString	m_tips;
	int	m_nHeight;
	class CMapWnd*	m_pChild;
	bool	 m_bMap;
	
	CStringArray		m_arScreen;
	
// Operations
public:
	void	HideTips();
	void	ShowTips(CRect tRc, CRect wRc, CString tips);
	void	ShowMap(CRect tRc, CRect wRc, CString mapname, CString option);
	CRect	FitSize(CRect rect, CPoint pt);
	CString GetTip() { return m_tips; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTipWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTipWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
