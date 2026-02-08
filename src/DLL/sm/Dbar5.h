#if !defined(AFX_DBAR5_H__B856BDE9_99B9_4A01_B6BB_A1D3D605A4A5__INCLUDED_)
#define AFX_DBAR5_H__B856BDE9_99B9_4A01_B6BB_A1D3D605A4A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dbar5.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDbar5 dialog

class CDbar5 : public CDialog
{
// Construction
public:
	CDbar5(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDbar5)
	enum { IDD = IDD_DBAR5 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbar5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDbar5)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBAR5_H__B856BDE9_99B9_4A01_B6BB_A1D3D605A4A5__INCLUDED_)
