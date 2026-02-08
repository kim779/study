#if !defined(AFX_RENAMEDLG_H__1F84A5C9_5FE5_47FB_BC34_BF16BF346F23__INCLUDED_)
#define AFX_RENAMEDLG_H__1F84A5C9_5FE5_47FB_BC34_BF16BF346F23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CRenameDlg : public CDialog
{
public:
	CRenameDlg(CWnd* pParent = NULL, CString strName = _T(""));

	//{{AFX_DATA(CRenameDlg)
	enum { IDD = IDD_RENAME };
	CString	m_sName;
	//}}AFX_DATA

public:
	CPoint m_point;

	//{{AFX_VIRTUAL(CRenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRenameDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENAMEDLG_H__1F84A5C9_5FE5_47FB_BC34_BF16BF346F23__INCLUDED_)
