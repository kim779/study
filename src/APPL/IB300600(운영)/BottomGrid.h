// BottomGrid.h: interface for the CBottomGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Common.h"
#include "AxisExt.h"
#include "grid/GridCtrl.h"

class BottomGrid : public CGridCtrl, public CAxisExt
{
// Construction
public:
	BottomGrid(CWnd* pParent);
	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE , DWORD dwExStyle = 0);
	CString m_sDragMicheMS;
	bool m_mouseDragmode;
	HCURSOR m_hHandCursor, m_hOriCursor;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BottomGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~BottomGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(BottomGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
