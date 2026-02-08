#pragma once
// CX_PricePopup.h : header file
//
#include <afxtempl.h>
#include <math.h>

#define CeilEX(x,dig) (floor((x)*pow(10,dig)+0.5)/pow(10,dig))

/////////////////////////////////////////////////////////////////////////////
// CPricePopup window

class CPricePopup : public CWnd
{
// Construction
public:
	CPricePopup();
	virtual ~CPricePopup();

// Attributes
public:
	int		m_nStatus;
	CWnd		*m_pParent, *m_pWizard;
	double		m_pDoubleTmp;
	double		m_dCurr, m_dMax, m_dMin, m_dGjga;
	bool		m_bKospi;
	int		m_nType;
	CString		m_sCode;
	bool		m_bNoMaxMin;
	BOOL		m_bNewUnit;		// 2023 호가단위개선

protected:
	bool		m_fDone, m_bDrag, m_bLhGd;
	CRect		m_selRc;
	double		m_fStart, m_fEnd, m_fSel;

	int		m_startPos;
	CArray	<double, double>	m_arPrice;
	CArray	<double, double>	m_arRate;
	CScrollBar*	m_pVScroll;

	COLORREF	m_upClr, m_dnClr;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPricePopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	void	setParent(CWnd *pWnd);
	void	setColor(COLORREF upClr, COLORREF dnClr);

private:
	CString LoadStringTb(UINT nIdx);
	CPen*	getAxPen(COLORREF clr, int nWidth = 1, int nStyle = 0);
	CFont*	getAxFont(CString fName, int point);
	void	setCurrPosition();
	void	drawRectangle(CRect rt, COLORREF clr);
	CRect	getPosition(CPoint point, double* val);
	CString SetComma(CString src);
	double	CalculateUnit(double price, bool bInc = true);
	double	CalculateRate(double price);
	void	makeList();
	void	drawData(CDC* pDC);
	void	drawCaption(CDC* pDC);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPricePopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};