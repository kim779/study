#if !defined(AFX_XEDIT_H__CFDD0E39_BF54_4DB3_B8FA_0E2D840ABC8D__INCLUDED_)
#define AFX_XEDIT_H__CFDD0E39_BF54_4DB3_B8FA_0E2D840ABC8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XEdit.h : header file
//
extern UINT NEAR WM_XEDIT_VK_RETURN;
extern UINT NEAR WM_XEDIT_VK_ESCAPE;

/////////////////////////////////////////////////////////////////////////////
// CXEdit window

class CXEdit : public CEdit
{
// Construction
public:
	CXEdit();

// Attributes
public:
	enum XEDIT_TYPE { COMMON_TYPE=0, TIMER_TYPE=1};
	UINT m_uEditType;

// Operations
public:
	void SetEditType(UINT uEditType = COMMON_TYPE) { m_uEditType = uEditType; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XEDIT_H__CFDD0E39_BF54_4DB3_B8FA_0E2D840ABC8D__INCLUDED_)
