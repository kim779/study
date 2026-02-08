#pragma once
// AmountPopup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAmountPopup window
#include "grid/GridCtrl.h"

#define CONFIG_INI "%s\\user\\%s\\IB190800.ini"

class CAmountPopup : public CWnd
{
// Construction
public:
	CAmountPopup();

// Attributes
public:
	int		m_nStatus;
	CWnd		*m_pParent, *m_pWizard;
	CGridCtrl *m_pJcntGrid;
	CFont* m_pFont;
	CString m_profile;
	COLORREF	m_crBlack,m_crWhite;
protected:
	bool	m_fDone, m_bDrag,m_bInit;
	CPoint	m_firstPnt;
	CRect	m_rc, m_selRt;
	CString m_sUserID, m_sRoot;

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
	CPen* getAxPen(COLORREF clr, int nWidth = 1, int nStyle = 0);
	CFont* getAxFont(CString fName, int point);
	void setParent(CWnd *pWnd);
	CRect getPosition(CPoint point, int &val);
	void drawRectangle(CRect rt, COLORREF clr);
	void drawData(CDC *pDC);
	void setPathInfo(CString sRoot, CString sUser);
	void drawCaption(CDC *pDC);
	LOGFONT setFont(CString fName, int size, bool isItalic, bool isBold);
	int Variant(int cmd, int data);
	void LoadConfig();
	void SetJcnt( int jcnt );
	LPCTSTR Variant( int cmd, LPCTSTR data );
	virtual ~CAmountPopup();

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
	afx_msg	LRESULT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
