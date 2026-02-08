#if !defined(AFX_MONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
#define AFX_MONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonitorWnd.h : header file
//
#define		MONITER_WRET_READY			 0
#define		MONITER_WRET_RECEIVE		 1
#define		MONITER_WRET_QUIT			-1
#define		MONITER_WRET_TIMEOUT		-2

/////////////////////////////////////////////////////////////////////////////
// CMonitorWnd window

class CMonitorWnd : public CWnd
{
// Construction
public:
	CMonitorWnd();

// Attributes
public:
	HWND m_hATHwnd;
	int  m_nWaitResult;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMonitorWnd();
	BOOL	OpenWindow();
	HWND	GetHandle();
	int		WaitResult();


	// Generated message map functions
protected:
	//{{AFX_MSG(CMonitorWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
