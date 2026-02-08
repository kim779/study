#if !defined(AFX_SETUP_H__8BCB100B_A807_444A_AB71_D9978B3853A4__INCLUDED_)
#define AFX_SETUP_H__8BCB100B_A807_444A_AB71_D9978B3853A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setup.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSetup dialog
#include "useMaindefine.h"
class CSetup : public CDialog
{
// Construction
public:
	CSetup(CWnd* pParent = NULL);   // standard constructor
	~CSetup();

	CString	m_Port;
protected:

//	class CNSocket*	m_sock;
// Dialog Data
	//{{AFX_DATA(CSetup)
	enum { IDD = IDD_SETUP };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CControlChanger> m_changer;
#else
	CControlChanger* m_changer{};
#endif
	void	Init();
	void	Save();
	// Generated message map functions
	//{{AFX_MSG(CSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnAos();
	afx_msg void OnKeyprotect();
	afx_msg void OnPcfirewall();
	//}}AFX_MSG
	afx_msg	LONG OnAXIS(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_allResourceDownload;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUP_H__8BCB100B_A807_444A_AB71_D9978B3853A4__INCLUDED_)
