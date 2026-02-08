#if !defined(AFX_PASSEDIT_H__8508D2A8_2F37_4E23_9BA4_E9D54DBB62FC__INCLUDED_)
#define AFX_PASSEDIT_H__8508D2A8_2F37_4E23_9BA4_E9D54DBB62FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassEdit window

class CPassEdit : public CEdit
{
// Construction
public:
	CPassEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPassEdit();
	CString	GetPass() { return m_szOrg; }
	void SetPass(CString pswd);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPassEdit)
	afx_msg void OnChange();
	afx_msg void OnKillfocus();
	afx_msg void OnMaxtext();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void AllAsta();

	CString	m_szOrg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSEDIT_H__8508D2A8_2F37_4E23_9BA4_E9D54DBB62FC__INCLUDED_)
