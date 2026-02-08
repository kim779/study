#if !defined(AFX_DLGSAVECANDLE_H__9CA668CD_047C_4A63_A881_E1D0E710C3AB__INCLUDED_)
#define AFX_DLGSAVECANDLE_H__9CA668CD_047C_4A63_A881_E1D0E710C3AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSaveCandle.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSaveCandle dialog
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

class CDlgSaveCandle : public CDialog
{
// Construction
public:
	CDlgSaveCandle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSaveCandle)
	enum { IDD = IDD_DLG_SAVECANDLE };
	CString	m_strCandleName;
	//}}AFX_DATA

	CfxImgButton m_btnOk;
	CfxImgButton m_btnCancel;
	CWnd*	m_pWndMainFrm;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSaveCandle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSaveCandle)
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

#endif // !defined(AFX_DLGSAVECANDLE_H__9CA668CD_047C_4A63_A881_E1D0E710C3AB__INCLUDED_)
