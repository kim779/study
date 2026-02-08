#if !defined(AFX_SEDIT_H__298DF6DB_5337_492F_A44D_DFC9266BFB33__INCLUDED_)
#define AFX_SEDIT_H__298DF6DB_5337_492F_A44D_DFC9266BFB33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSEdit window

class CSEdit : public CEdit
{
// Construction
public:
	CSEdit(CDialog* parent);

// Attributes
protected:
	CDialog*	m_parent;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSEdit)
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEDIT_H__298DF6DB_5337_492F_A44D_DFC9266BFB33__INCLUDED_)
