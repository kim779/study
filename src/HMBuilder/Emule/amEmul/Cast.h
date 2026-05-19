#if !defined(AFX_CAST_H__EC1CB579_3A03_4CDD_A869_998A4BF1602D__INCLUDED_)
#define AFX_CAST_H__EC1CB579_3A03_4CDD_A869_998A4BF1602D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cast.h : header file
//
#include "mapping.h"

class CCast : public CWnd
{
public:
	CCast(class CGuard* guard, CWnd* pWnd, char* home);           // protected constructor used by dynamic creation

// Attributes
private:
	CWnd*	m_wizard;
	CGuard*	m_guard;


public:
	bool	m_running;
	CString	m_home;

	CWnd*	m_msgwnd;
	CMapping* m_smap;

	CCriticalSection	m_sync;

// Operations
public:
	virtual ~CCast();
	void	LogMsg(DWORD tick, CString data, bool trade);

	bool	IsRunning() { return m_running; }
	bool	InitialCast();
	void	RegisterRTM(int major, int minor, CString codes);
	void	ReloadFMT(CString home);
	void	FilterRTM(CString filter);
	void	SetInterval(int interval);
	void	ExitCast();
	bool	IsFoptionCode(CString code);

public:
	// Generated message map functions
	//{{AFX_MSG(CTprocDlg)
	//}}AFX_MSG
	afx_msg LRESULT OnQuote(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_CAST_H__EC1CB579_3A03_4CDD_A869_998A4BF1602D__INCLUDED_)
