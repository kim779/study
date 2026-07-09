#if !defined(AFX_FORMAT_H__5CAD0A01_7CC6_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_FORMAT_H__5CAD0A01_7CC6_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// format.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CFormat dialog

class AFX_EXT_CLASS CFormat : public CDialog
{
// Construction
public:
	CFormat(CWnd* pParent, BYTE isEdit, char* onEdit, DWORD attr, char* name, int size);

// Dialog Data
	//{{AFX_DATA(CFormat)
	enum { IDD = IDD_FORMAT };
	CString	m_onEdit;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	BYTE	m_isEdit;
	DWORD	m_attr;
	int	m_size;
	CString	m_edit;

protected:
	bool	m_first;
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormat)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNoEdit();
	afx_msg void OnChangeFormat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMAT_H__5CAD0A01_7CC6_11D4_A024_00001CD7F9BE__INCLUDED_)
