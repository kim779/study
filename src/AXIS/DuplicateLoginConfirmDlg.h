#if !defined(AFX_DuplicateLOGINCONFIRMDLG_H__FB0C02A6_CDA1_4E0D_89DC_20BAFB692DB0__INCLUDED_)
#define AFX_DuplicateLOGINCONFIRMDLG_H__FB0C02A6_CDA1_4E0D_89DC_20BAFB692DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DuplicateLoginConfirmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDuplicateLoginConfirmDlg dialog

class CDuplicateLoginConfirmDlg : public CDialog
{
// Construction
public:
	CDuplicateLoginConfirmDlg(CWnd* pParent, const char* lpszMsg);   

// Dialog Data
	//{{AFX_DATA(CDuplicateLoginConfirmDlg)
	enum { IDD = IDD_DUPLOGIN_CONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDuplicateLoginConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDuplicateLoginConfirmDlg)
	afx_msg void OnForcelogin();
	afx_msg void OnSiseonly();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_msg;
	UINT	m_result;
public:
	UINT	GetResult();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DuplicateLOGINCONFIRMDLG_H__FB0C02A6_CDA1_4E0D_89DC_20BAFB692DB0__INCLUDED_)
