#pragma once
// TitleTip.h : header file
//

#define TITLETIP_CLASSNAME _T("ZTitleTip")

/////////////////////////////////////////////////////////////////////////////
// CTitleTip window

class CTitleTip : public CWnd
{
// Construction
public:
	CTitleTip();
	virtual ~CTitleTip();
	virtual BOOL Create(CWnd *parentWnd);

// Attributes
public:

// Operations
public:
	void Show(CRect rectTitle, LPCTSTR lpszTitleText, 
			int xoffset = 0, LPRECT lpHoverRect = nullptr, LPLOGFONT lpLogFont = nullptr);
	void Hide();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleTip)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	CWnd* m_parentWnd{};
	CRect	m_rectTitle;
	CRect	m_rectHover;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTitleTip)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

