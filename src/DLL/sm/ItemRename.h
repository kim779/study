#if !defined(AFX_ITEMRENAME_H__5D7271E4_2021_4BCF_B6B3_67FD5E6F0DD3__INCLUDED_)
#define AFX_ITEMRENAME_H__5D7271E4_2021_4BCF_B6B3_67FD5E6F0DD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemRename.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CItemRename dialog

class CItemRename : public CDialog
{
// Construction
public:
	CItemRename(CWnd* pParent = NULL);   // standard constructor

	void	SetName(CString name);
// Dialog Data
	//{{AFX_DATA(CItemRename)
	enum { IDD = IDD_ITEMRENAME };
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemRename)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMRENAME_H__5D7271E4_2021_4BCF_B6B3_67FD5E6F0DD3__INCLUDED_)
