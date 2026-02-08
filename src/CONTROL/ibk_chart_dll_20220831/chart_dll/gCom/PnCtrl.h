#if !defined(AFX_PNCTRL_H__E2A80BAB_76EA_4A85_8971_A073F906D20E__INCLUDED_)
#define AFX_PNCTRL_H__E2A80BAB_76EA_4A85_8971_A073F906D20E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnCtrl window
#include "PnBaseWnd.h"

class CPnCtrl : public CPnBaseWnd
{
// Construction
public:
	CPnCtrl(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info);
	virtual ~CPnCtrl();

// Attributes
public:
	enum	{ autoPre = -1, autoStop, autoNext};

private:
	bool	m_bGrid;
	bool	m_bTable;
	BOOL	m_bMouseIN;

	int	m_iMainKind;
	int	m_iSlider;

	int	m_iTotalDay;
	int	m_iDispPos;
	int	m_iDispEnd;
	int	m_iDispDay;

	int	m_iTimerID;
	int	m_iAutoScroll;

	CImageList*	m_pimglstCtrl;

	class CPnScrollBar*	m_pScroll;
	class CPnSlider*	m_pSlider;
	class CPnToolTip*	m_pToolTip;
	
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	
private:
	void	ArrangeCtrl(CRect wRC);
	void	drawButton(CDC* pDC, struct _comp* comp);

	void	initScroll(int pos, int end, int day, int total);
	void	UpdateScroll(bool bGetData = true);
	void	setInfo();
	void	getScrInfo();
	void	setAutoScroll(int autoID);
	void	AutoShiftAction();
	void	SetGrid(bool bSet);
	void	SetTable(bool bSet);
	void	lbuttonup(CPoint point);
	void	mouseLeaveAction(UINT nFlags, CPoint point);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg LRESULT	OnCtrlEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNCTRL_H__E2A80BAB_76EA_4A85_8971_A073F906D20E__INCLUDED_)
