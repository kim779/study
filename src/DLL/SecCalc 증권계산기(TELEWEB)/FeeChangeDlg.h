#if !defined(AFX_FEECHANGEDLG_H__83004E34_20C3_4ECB_BA1E_C628FF48F9FD__INCLUDED_)
#define AFX_FEECHANGEDLG_H__83004E34_20C3_4ECB_BA1E_C628FF48F9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeeChangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFeeChangeDlg dialog

class CFeeChangeDlg : public CDialog
{
// Construction
public:
	CFeeChangeDlg(CWnd* pParent = NULL);   // standard constructor
	CSecCalcApp	*m_pApp;

// Dialog Data
	//{{AFX_DATA(CFeeChangeDlg)
	enum { IDD = IDD_CHANGE_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFeeChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFeeChangeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEECHANGEDLG_H__83004E34_20C3_4ECB_BA1E_C628FF48F9FD__INCLUDED_)
