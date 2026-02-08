#if !defined(AFX_EDITUSR_H__A4EBD44E_27F3_4E90_B024_13947F301CF7__INCLUDED_)
#define AFX_EDITUSR_H__A4EBD44E_27F3_4E90_B024_13947F301CF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditUsr.h : header file
//

#include "../../../inc/ExEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CEditUsr window

class CEditUsr : public CExEdit
{
// Construction
public:
	CEditUsr();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditUsr)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditUsr();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditUsr)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITUSR_H__A4EBD44E_27F3_4E90_B024_13947F301CF7__INCLUDED_)
