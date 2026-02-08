#if !defined(AFX_NOTTOGGLEBUTTON_H__B459F10F_4980_4D03_8484_C288ED353BE5__INCLUDED_)
#define AFX_NOTTOGGLEBUTTON_H__B459F10F_4980_4D03_8484_C288ED353BE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotToggleButton.h : header file
// 비토글버튼(토글되지 않는 버튼)들을 위한 클래스이다.

#include "ButtonEx.h"

/////////////////////////////////////////////////////////////////////////////
// CNotToggleButton window

class CNotToggleButton : public CButtonEx
{
// Construction
public:
	CNotToggleButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotToggleButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNotToggleButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNotToggleButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2006/11/16 - Seung-Won, Bae) Support Auto Repeat!
protected:
	int		m_nTimerID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTTOGGLEBUTTON_H__B459F10F_4980_4D03_8484_C288ED353BE5__INCLUDED_)
