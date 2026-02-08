#if !defined(AFX_USESTOPPANE_H__7A8962F7_2BF7_4623_8E65_DC95D1966C37__INCLUDED_)
#define AFX_USESTOPPANE_H__7A8962F7_2BF7_4623_8E65_DC95D1966C37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UseStopPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUseStopPane window

class CUseStopPane : public CWnd
{
// Construction
public:
	CUseStopPane(bool (*axiscall)(int, WPARAM, LPARAM));

// Attributes
public:
	CFont	m_font;
// Operations
public:
	BOOL	m_bStockRunning;
	BOOL	m_bFORunning;
protected:
private:
	COLORREF	m_clrStock;
	COLORREF	m_clrFO;
	BOOL	m_bStockUsing;
	BOOL	m_bFOUsing;
	CRect	m_foRc;
	CRect	m_stockRc;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUseStopPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRunningFOStop(BOOL bRunning);
	void SetRunningStockStop(BOOL bRunning);
	void drawUsing(CRect rc, COLORREF rgb);
	void SetUseFOStop(BOOL bUse);
	void SetUseStockStop(BOOL bUse);
	virtual ~CUseStopPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUseStopPane)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USESTOPPANE_H__7A8962F7_2BF7_4623_8E65_DC95D1966C37__INCLUDED_)
