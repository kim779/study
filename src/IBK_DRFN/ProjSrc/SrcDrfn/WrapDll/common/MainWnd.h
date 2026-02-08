#if !defined(AFX_MAINWND_H__69E9D28E_BCC2_4B14_95F9_0B70FA394DD0__INCLUDED_)
#define AFX_MAINWND_H__69E9D28E_BCC2_4B14_95F9_0B70FA394DD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//
#include "../../../SrcSite/include/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
//class CBaseNetworkModule;
//class CHistoricalVolateWnd;
	
class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pWnd);
// Attributes
public:
	CWnd*		m_pParent;

//	CBaseNetworkModule* m_pwndBaseNetwork;
//	CHistoricalVolateWnd* m_pHistoricalWnd;

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
	CPCTrMngHelper	m_PctrHelper;
	IDebugManager*	m_pIDebugMng;

	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__69E9D28E_BCC2_4B14_95F9_0B70FA394DD0__INCLUDED_)
