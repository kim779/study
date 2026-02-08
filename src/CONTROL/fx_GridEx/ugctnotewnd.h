/*************************************************************************
				Class Declaration : CUGNoteTypeEWnd
**************************************************************************
	Source file : UGCTNoteWnd.cpp
	Header file : UGCTNoteWnd.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

    Purpose
*************************************************************************/

#pragma once



//Class Declaration
class UG_CLASS_DECL CUGNoteTypeEWnd : public CEdit
{
// Construction
public:
	CUGNoteTypeEWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGNoteTypeEWnd)
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CUGNoteTypeEWnd();

protected:
	CBrush m_bkGndBrush;
	CFont  m_font;

	void CalcEditRect();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGNoteTypeEWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdate();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

