#if !defined(AFX_BCTRL1_H__FC2E81EC_37DB_4608_ABDA_3BB06350889D__INCLUDED_)
#define AFX_BCTRL1_H__FC2E81EC_37DB_4608_ABDA_3BB06350889D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCtrl1.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CBCtrl1 window

class CBCtrl1 : public CWnd
{
// Construction
public:
	CBCtrl1(CString home, COLORREF bk = RGB(255, 255, 255));

// Attributes
public:
protected:
	int		m_key;
	CString		m_home;
	COLORREF	m_bk;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCtrl1)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	set_Key(int key) { m_key = key; }
	int	get_Key() { return m_key; }
	virtual ~CBCtrl1();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCtrl1)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCTRL1_H__FC2E81EC_37DB_4608_ABDA_3BB06350889D__INCLUDED_)
