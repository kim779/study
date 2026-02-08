#if !defined(AFX_MAINWND_H__BF49350A_7EE2_470F_B72B_780DCFFD4C41__INCLUDED_)
#define AFX_MAINWND_H__BF49350A_7EE2_470F_B72B_780DCFFD4C41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window


class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	char*		m_pacRegion[MAX_INDEX];
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__BF49350A_7EE2_470F_B72B_780DCFFD4C41__INCLUDED_)
