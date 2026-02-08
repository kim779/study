#if !defined(AFX_DLGSETUP_H__E9294C0B_CC09_407E_BDE2_0FC13C635726__INCLUDED_)
#define AFX_DLGSETUP_H__E9294C0B_CC09_407E_BDE2_0FC13C635726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : header file
//

#include "RscDlg.h"
#include "control/XTabCtrl.h"
#include "PatternDBItems.h"			// Pattern DB Items
#include "DlgEnvSetup1.h"
#include "DlgEnvSetup2.h"
#include "DlgEnvSetup3.h"
#include "DlgEnvSetup4.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetup dialog

class CDlgSetup : public CRscDlg
{
// Construction
public:
	CDlgSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetup)
	enum { IDD = IDD_DLG_SETUP };
		CXTabCtrl	m_tabCtrl;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetup)
		// NOTE: the ClassWizard will add member functions here
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void			SetPatternEnvData(CPatternEnvData *pPatternEnvData);
	CArray<CRscDlg*,CRscDlg*> m_arrayTab;
	CDlgEnvSetup1* m_pEnvSetup1;
	CDlgEnvSetup2* m_pEnvSetup2;
	CDlgEnvSetup3* m_pEnvSetup3;
	CDlgEnvSetup4* m_pEnvSetup4;
	CPatternEnvData *m_pPatternEnvData;

protected:
	CBrush			m_brBack;				// Dialog BG Color
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_H__E9294C0B_CC09_407E_BDE2_0FC13C635726__INCLUDED_)
