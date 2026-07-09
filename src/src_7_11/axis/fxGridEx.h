#if !defined(AFX_FXGRIDEX_H__D85D58AC_5CCD_4ABF_A35F_A1F9AFCFEE62__INCLUDED_)
#define AFX_FXGRIDEX_H__D85D58AC_5CCD_4ABF_A35F_A1F9AFCFEE62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxGridEx.h : header file
//

#include "../control/fx_gridEX/fxGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CfxGridEx window

class CfxGridEx : public CfxGrid
{
// Construction
public:
	CfxGridEx(LOGFONT* logfont = NULL, class CWnd* parent = NULL);

// Attributes
public:
	class	CWnd*	m_parent;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxGridEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxGridEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxGridEx)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXGRIDEX_H__D85D58AC_5CCD_4ABF_A35F_A1F9AFCFEE62__INCLUDED_)
