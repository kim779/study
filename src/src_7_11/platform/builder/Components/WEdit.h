#if !defined(AFX_WEDIT_H__DEB84082_1FA5_4B2D_AC52_65FAC7180F31__INCLUDED_)
#define AFX_WEDIT_H__DEB84082_1FA5_4B2D_AC52_65FAC7180F31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWEdit window

class CWEdit : public CEdit
{
// Construction
public:
	CWEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEDIT_H__DEB84082_1FA5_4B2D_AC52_65FAC7180F31__INCLUDED_)
