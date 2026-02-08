#if !defined(AFX_TOGGLEBUTTON_H__2E87687E_DF3F_49AE_8DFD_86B70FBDC149__INCLUDED_)
#define AFX_TOGGLEBUTTON_H__2E87687E_DF3F_49AE_8DFD_86B70FBDC149__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToggleButton.h : header file
// 토글되는 버튼 클래스

#include "ButtonEx.h"

/////////////////////////////////////////////////////////////////////////////
// CToggleButton window

class CToggleButton : public CButtonEx
{
// Construction
public:
	CToggleButton();

// Attributes
private:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToggleButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToggleButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToggleButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOGGLEBUTTON_H__2E87687E_DF3F_49AE_8DFD_86B70FBDC149__INCLUDED_)
