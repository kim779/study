#if !defined(AFX_PNCTRL_H__17D7DC44_9151_46C0_A61F_0B3F70941606__INCLUDED_)
#define AFX_PNCTRL_H__17D7DC44_9151_46C0_A61F_0B3F70941606__INCLUDED_

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

// Attributes
public:
	class CMainWnd *m_pMainWnd;

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
	virtual ~CPnCtrl();

	enum	{ autoPre = -1, autoStop, autoNext};
	// Generated message map functions
protected:
	//{{AFX_MSG(CPnCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg LRESULT	OnCtrlEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void	arrangeCtrl(CRect wRC);
	void	drawButton(CDC* pDC, struct _comp* comp);

	void	initScroll(int pos, int end, int day, int total);
	void	updateScroll(bool bGetData = true);
	void	setInfo();
	void	getScrInfo();
	void	setAutoScroll(int autoID);
	void	AutoShiftAction();
	void	SetTip(bool bSet);
	void	lbuttonup(CPoint point);

private:
	int			m_mKind;
	BOOL			m_bMouseIN;
	class CPnScrollBar	*m_pScroll;
	class CPnSlider		*m_pSlider;
	class CPnToolTip	*m_pToolTip;

	int			m_nSlider;

	int			m_totalDay;
	int			m_dispPos;
	int			m_dispEnd;
	int			m_dispDay;

	int			m_TimerID;
	int			m_AutoScroll;

	bool			m_bToolTip;

	CImageList		*m_pCtrlImage;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNCTRL_H__17D7DC44_9151_46C0_A61F_0B3F70941606__INCLUDED_)
