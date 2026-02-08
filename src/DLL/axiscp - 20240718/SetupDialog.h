#if !defined(AFX_SETUPDIALOG_H__8D959627_FFC3_4573_B97B_8CB1AFBB438F__INCLUDED_)
#define AFX_SETUPDIALOG_H__8D959627_FFC3_4573_B97B_8CB1AFBB438F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupDialog.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSetupDialog dialog

class CSetupDialog : public CDialog
{
// Construction
public:
	CSetupDialog(UINT idResource, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDialog() {}
// Dialog Data
	//{{AFX_DATA(CSetupDialog)
	enum { IDD = IDD_SETUPDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	UINT	m_resourceID;
	CString m_caption;
	virtual void DefaultSetup();
	virtual void ApplySetup();
	//virtual void Reue
	CWnd* m_pMain{};
	virtual void  SearchGroupList();
protected:
	void	ChangeHangulMode(HWND hWnd);		// 한글 모드로
	void	ChangeEnglishMode(HWND hWnd);		// 영문 모드로

protected:
	//CControlChanger* m_changer{};
	std::unique_ptr< CControlChanger> m_changer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPDIALOG_H__8D959627_FFC3_4573_B97B_8CB1AFBB438F__INCLUDED_)
