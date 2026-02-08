#if !defined(AFX_DLGSAVEPATTERN_H__E1FF6A90_449A_4956_ABCC_263EA4D45E9B__INCLUDED_)
#define AFX_DLGSAVEPATTERN_H__E1FF6A90_449A_4956_ABCC_263EA4D45E9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSavePattern.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSavePattern dialog
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

class CDlgSavePattern : public CDialog
{
// Construction
public:
	CDlgSavePattern(CWnd* pParent = NULL);   // standard constructor
	BOOL CheckFileName();

	CfxImgButton m_btnOk;
	CfxImgButton m_btnCancel;
	CWnd*	m_pWndMainFrm;
// Dialog Data
	//{{AFX_DATA(CDlgSavePattern)
	enum { IDD = IDD_DLG_SAVEPATTERN };
	CString	m_strPatternName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSavePattern)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSavePattern)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAVEPATTERN_H__E1FF6A90_449A_4956_ABCC_263EA4D45E9B__INCLUDED_)
