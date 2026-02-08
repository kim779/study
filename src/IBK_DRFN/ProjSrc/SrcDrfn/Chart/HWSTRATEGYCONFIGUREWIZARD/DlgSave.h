#if !defined(AFX_DLGSAVE_H__7B86C250_8340_4DCF_ADAD_B0D104B197B9__INCLUDED_)
#define AFX_DLGSAVE_H__7B86C250_8340_4DCF_ADAD_B0D104B197B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSave dialog
#include "Resource.h"
#include "Control/xSkinButton.h"

class CDlgSave  : public CDialog
{
// Construction
public:
	CDlgSave(CWnd* pParent = NULL);   // standard constructor
	void SetSTTitle(LPCTSTR lpSTTitle) { m_strSTTitle = lpSTTitle;}
	void SetUserPath(LPCTSTR lpUserPath) { m_strUserPath = lpUserPath;}
	CString GetSTTitle() { return m_strSTTitle;}

// Dialog Data
	//{{AFX_DATA(CDlgSave)
	enum { IDD = IDD_DLG_SAVE };
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CListBox	m_listSTFile;
	CEdit	m_editName;
	//}}AFX_DATA
//	CxSkinButton	m_btnOk;
//	CxSkinButton	m_btnCancel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strSTTitle;
	CString m_strUserPath;
	COLORREF m_crBk;
	CBrush	m_brBkColor;

	// Generated message map functions
	//{{AFX_MSG(CDlgSave)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeListStfile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAVE_H__7B86C250_8340_4DCF_ADAD_B0D104B197B9__INCLUDED_)
