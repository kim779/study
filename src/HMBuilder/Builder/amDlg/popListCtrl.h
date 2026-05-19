#pragma once


// CPopListCtrl

class CPopListCtrl : public CWnd
{
	DECLARE_DYNAMIC(CPopListCtrl)

public:
	CPopListCtrl();
	virtual ~CPopListCtrl();

	class	CEditListCtrl*	m_editListCtrl;

protected:
	BOOL	m_done;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


