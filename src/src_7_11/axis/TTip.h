#pragma once
// TTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTip window

class CTTip : public CWnd
{
// Construction
public:
	CTTip(CWnd* mainframe);

#define	XXXX 9

// Attributes
public:
	CWnd*	m_mainframe;
	CFont	m_font;
	CRect	m_rect;
	CString	m_tips;
// Operations
public:
	void	HideTips();
	void	ShowTips(CRect tRc, CString tips);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTTip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
