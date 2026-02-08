#pragma once

#include "../../h/axisfire.h"

class CFOMultiSel : public CWnd
{
// Construction
public:
	CFOMultiSel(CWnd *pParent, _param *param);

// Attributes
public:
	CWnd* m_pParent;
	_param m_Param;
	CString m_data;

// Operations
public:
	CString GetCtrlProperty( LPCSTR prop_name );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFOMultiSel)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFOMultiSel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFOMultiSel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFOMultiSel)
	afx_msg void SetProperties(LPCTSTR prop);
	afx_msg BSTR GetProperties();
	afx_msg BOOL DoSelect();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};