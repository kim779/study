#if !defined(AFX_DLGSOIKCHART_H__E772EDDB_7086_493E_827F_C8679DC2DD00__INCLUDED_)
#define AFX_DLGSOIKCHART_H__E772EDDB_7086_493E_827F_C8679DC2DD00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSoIkChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSoIkChart dialog
#include "resource.h"
class CDlgSoIkChart : public CDialog
{
// Construction
public:
	CDlgSoIkChart(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSoIkChart)
	enum { IDD = IDD_DLG_TAP_SONIKCHART };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSoIkChart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSoIkChart)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOIKCHART_H__E772EDDB_7086_493E_827F_C8679DC2DD00__INCLUDED_)
