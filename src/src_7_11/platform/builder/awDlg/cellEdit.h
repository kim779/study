#if !defined(AFX_CELLEDIT_H__EE007FC1_7D9B_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_CELLEDIT_H__EE007FC1_7D9B_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cellEdit.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CCellEdit dialog

class AFX_EXT_CLASS CCellEdit : public CDialog
{
// Construction
public:
	CCellEdit(CWnd* pParent, BYTE kind, BYTE type, BYTE isEdit, DWORD onEdit, char* editS,
			CString name, DWORD attr, int size);

// Dialog Data
	//{{AFX_DATA(CCellEdit)
	enum { IDD = IDD_CEDIT };
	CString	m_format;
	CString	m_title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	BYTE	m_isEdit;
	DWORD	m_onEdit;
	DWORD	m_attr;

protected:
	BYTE	m_kind;
	BYTE	m_type;
	int	m_size;
	bool	m_first;
	CString	m_name;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCellEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnColor();
	afx_msg void OnChangeEdit();
	afx_msg void OnNOP();
	afx_msg void OnFORMAT();
	afx_msg void OnCORGB();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLEDIT_H__EE007FC1_7D9B_11D4_A024_00001CD7F9BE__INCLUDED_)
