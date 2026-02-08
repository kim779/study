#if !defined(AFX_BASEMP_H__8ADCCF20_BFF4_4937_9468_04091783E4FD__INCLUDED_)
#define AFX_BASEMP_H__8ADCCF20_BFF4_4937_9468_04091783E4FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseMP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseMP window

class CBaseMP : public CWnd
{
// Construction
public:
	CBaseMP();
	bool m_bDll;
// Attributes
protected:
	bool	m_protect;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseMP)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseMP();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseMP)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEMP_H__8ADCCF20_BFF4_4937_9468_04091783E4FD__INCLUDED_)
