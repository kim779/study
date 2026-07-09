#if !defined(AFX_MYEDIT_H__1AB9EC19_8ABE_426F_A515_E83FEC6209A9__INCLUDED_)
#define AFX_MYEDIT_H__1AB9EC19_8ABE_426F_A515_E83FEC6209A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window


class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnPaint();
	//}}AFX_MSG

	CTool* m_pTool;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__1AB9EC19_8ABE_426F_A515_E83FEC6209A9__INCLUDED_)
