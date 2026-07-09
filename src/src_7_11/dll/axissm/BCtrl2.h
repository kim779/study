#if !defined(AFX_BCTRL2_H__6B28CD6F_BCC5_401D_92B2_A70675E08D70__INCLUDED_)
#define AFX_BCTRL2_H__6B28CD6F_BCC5_401D_92B2_A70675E08D70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCtrl2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCtrl2 window

class CBCtrl2 : public CWnd
{
// Construction
public:
	CBCtrl2(CString home);

// Attributes
public:
protected:
	int		m_key;
	CString		m_home;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCtrl2)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	set_Key(int key) { m_key = key; }
	int	get_Key() { return m_key; }
	virtual ~CBCtrl2();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCtrl2)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCTRL2_H__6B28CD6F_BCC5_401D_92B2_A70675E08D70__INCLUDED_)
