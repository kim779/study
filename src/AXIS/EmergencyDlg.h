#if !defined(AFX_EMERGENCYDLG_H__CC601A30_963E_4648_A62F_50FB6854F38D__INCLUDED_)
#define AFX_EMERGENCYDLG_H__CC601A30_963E_4648_A62F_50FB6854F38D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmergencyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmergencyDlg dialog

class CEmergencyDlg : public CDialog
{
// Construction
public:
	CEmergencyDlg(CWnd* pParent = NULL);   // standard constructor
	UINT	GetVal() { return m_val; }
// Dialog Data
	//{{AFX_DATA(CEmergencyDlg)
	enum { IDD = IDD_DLG_EMERGENCY2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmergencyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmergencyDlg)
	afx_msg void OnOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	UINT		m_val;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMERGENCYDLG_H__CC601A30_963E_4648_A62F_50FB6854F38D__INCLUDED_)
