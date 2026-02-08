#pragma once

#include "Common.h"

class CListBoxEx : public CListBox
{
// Construction
public:
	CListBoxEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxEx)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

