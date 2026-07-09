#if !defined(AFX_FXTREECTRL_H__D24610BE_C39E_4278_9990_31B7396FB920__INCLUDED_)
#define AFX_FXTREECTRL_H__D24610BE_C39E_4278_9990_31B7396FB920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxTreeCtrl window

class AFX_EXT_CLASS CfxTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CfxTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxTreeCtrl)
	afx_msg void OnCustomTreeDraw(NMHDR * pNMHDR, LRESULT * pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXTREECTRL_H__D24610BE_C39E_4278_9990_31B7396FB920__INCLUDED_)
