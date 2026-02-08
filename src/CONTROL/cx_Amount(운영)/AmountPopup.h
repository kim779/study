#pragma once
// AmountPopup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAmountPopup window
#include "grid/GridCtrl.h"

#define CONFIG_INI "%s\\user\\%s\\Amount.ini"
#define AMT_GRIDROWS	3
#define AMT_GRIDCOLS	10
#define MON_GRIDROWS	2
#define MON_GRIDCOLS	5

class CAmountPopup : public CWnd
{
// Construction
public:
	CAmountPopup();
	virtual ~CAmountPopup();

// Attributes
public:
	int	m_nStatus;
	CWnd	*m_pParent, *m_pWizard;
	CFont*	m_pFont;
	CString m_profile;
	COLORREF m_crBlack,m_crWhite;

protected:
	bool	m_fDone;
	CRect	m_rcJcntGrid, m_rcJmonGrid;
	BOOL	m_bAlways;
	std::shared_ptr<CGridCtrl> m_pJcntGrid;
	std::shared_ptr<CGridCtrl> m_pJmonGrid;	//2013.10.08 KSJ

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmountPopup)
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString LoadStringTb(UINT nIdx);
	CPen*	getAxPen(COLORREF clr, int nWidth = 1, int nStyle = 0);
	CFont*	getAxFont(CString fName, int point);
	void	setParent(CWnd *pWnd);
	void	drawCaption(CDC *pDC);
	void	LoadConfig();
	void	addComma(CString& data);
	void	SetPopupAlways(BOOL bflag);
	int	Variant(int cmd, int data);
	LPCTSTR Variant(int cmd, LPCTSTR data);

	// Generated message map functions
protected:
	//{{AFX_MSG(CAmountPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};