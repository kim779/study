#if !defined(AFX_FMGROUP_H__96E8A0B5_B256_4347_85DD_58A9A530548A__INCLUDED_)
#define AFX_FMGROUP_H__96E8A0B5_B256_4347_85DD_58A9A530548A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmGroup.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmGroup command target

class AFX_EXT_CLASS CfmGroup : public CfmBase
{
	DECLARE_DYNCREATE(CfmGroup)

public:
	CfmGroup();	           // protected constructor used by dynamic creation
	CfmGroup(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmGroup();

// Operations
public:
	void	Draw(CDC* dc);
	int	IsPointInRect(CPoint pt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmGroup)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CfmGroup)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmGroup)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg void _Refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMGROUP_H__96E8A0B5_B256_4347_85DD_58A9A530548A__INCLUDED_)
