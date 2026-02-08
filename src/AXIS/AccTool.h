#if !defined(AFX_ACCTOOL_H__2A88F016_C02D_4665_AB48_709F8255021B__INCLUDED_)
#define AFX_ACCTOOL_H__2A88F016_C02D_4665_AB48_709F8255021B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccTool.h : header file
//

#define WM_APPLYACC	WM_USER+4444
////////////////////////////////////////////////////////////////////////////
// CAccTool dialog

class CAccTool : public CDialog
{
// Construction
public:
	CAccTool(CWnd* pParent = NULL);   // standard constructor

	CWnd*	m_parent;
// Dialog Data
	//{{AFX_DATA(CAccTool)
	enum { IDD = IDD_ACCTOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccTool)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccTool)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg LONG OnApplyACC(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCTOOL_H__2A88F016_C02D_4665_AB48_709F8255021B__INCLUDED_)
