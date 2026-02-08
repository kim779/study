#if !defined(AFX_NOSYSTEMMENUCOMBO_H__8B5A06CE_6690_4598_96A7_1BF7E2812374__INCLUDED_)
#define AFX_NOSYSTEMMENUCOMBO_H__8B5A06CE_6690_4598_96A7_1BF7E2812374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoSystemMenuCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoSystemMenuCombo window

class CNoSystemMenuCombo : public CComboBox
{
// Construction
public:
	CNoSystemMenuCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoSystemMenuCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNoSystemMenuCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNoSystemMenuCombo)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOSYSTEMMENUCOMBO_H__8B5A06CE_6690_4598_96A7_1BF7E2812374__INCLUDED_)
