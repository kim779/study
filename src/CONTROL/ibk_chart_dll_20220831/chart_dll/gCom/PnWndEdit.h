#if !defined(AFX_PNWNDEDIT_H__2CD7359A_CB95_4BBA_849C_BA06CC9E041B__INCLUDED_)
#define AFX_PNWNDEDIT_H__2CD7359A_CB95_4BBA_849C_BA06CC9E041B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnWndEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnWndEdit window

class CPnWndEdit : public CEdit
{
// Construction
public:
	CPnWndEdit(CWnd* pParent, CWnd* pView, int type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnWndEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnWndEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnWndEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CWnd	*m_pParent;
	CWnd	*m_pView;
	int	m_Type;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNWNDEDIT_H__2CD7359A_CB95_4BBA_849C_BA06CC9E041B__INCLUDED_)
