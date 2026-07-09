#if !defined(AFX_BCTRL3_H__5B77352F_618A_40EA_A495_7E90AB58FFA9__INCLUDED_)
#define AFX_BCTRL3_H__5B77352F_618A_40EA_A495_7E90AB58FFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCtrl3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCtrl3 window

class CBCtrl3 : public CWnd
{
// Construction
public:
	CBCtrl3(CString home);

// Attributes
public:
protected:
	int		m_key;
	CString		m_home;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCtrl3)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	set_Key(int key) { m_key = key; }
	int	get_Key() { return m_key; }
	virtual ~CBCtrl3();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCtrl3)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCTRL3_H__5B77352F_618A_40EA_A495_7E90AB58FFA9__INCLUDED_)
