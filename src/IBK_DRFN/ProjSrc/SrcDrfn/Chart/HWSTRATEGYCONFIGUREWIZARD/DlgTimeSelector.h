#if !defined(AFX_DLGTIMESELECTOR_H__70DC59F1_9AE7_499B_94D9_1D9E0DA7FB85__INCLUDED_)
#define AFX_DLGTIMESELECTOR_H__70DC59F1_9AE7_499B_94D9_1D9E0DA7FB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTimeSelector.h : header file
//
#include "Resource.h"
//#include "Control/xSkinButton.h"
#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExEdit.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgTimeSelector dialog

class CDlgTimeSelector : public CDialog
{
// Construction
public:
	CDlgTimeSelector(CWnd* pParent = NULL);   // standard constructor
	void SetDlgStrategyList(CWnd *pWnd) { m_pDlgStrategyList = pWnd;}
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE );

// Dialog Data
	//{{AFX_DATA(CDlgTimeSelector)
	enum { IDD = IDD_DLG_TIMESELECTOR };
	CExEdit	m_editTick;
	CExDrawButton	m_btnOK;
	CExEdit	m_editMinute;
	int m_nOption;
	int		m_nMinute;
	int		m_nTick;
	//}}AFX_DATA
	long m_lRow;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTimeSelector)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_crBk;
	CBrush	m_brBkColor;
	CWnd *m_pDlgStrategyList;

	// Generated message map functions
	//{{AFX_MSG(CDlgTimeSelector)
	afx_msg void OnRadioDay();
	afx_msg void OnRadioMinute();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnRadioTick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTIMESELECTOR_H__70DC59F1_9AE7_499B_94D9_1D9E0DA7FB85__INCLUDED_)
