#pragma once
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

#include "tool.h"

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:
	BOOL	m_bComposition;	//2016.11.08 KSJ 조합중인지 

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

