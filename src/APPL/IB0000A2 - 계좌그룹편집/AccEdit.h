#pragma once
// AccEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccEdit window



class CAccEdit : public CEdit
{
// Construction
public:
	CAccEdit();

// Attributes
public:
	char	m_nChar{};
	CString	m_beforeStr;
	bool	m_bOnce;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetAccount();
	CString changeFormat(CString sAcc);
	virtual ~CAccEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAccEdit)
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

