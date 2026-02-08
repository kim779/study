#pragma once
// fxGridEx.h : header file
//

#include "../../control/int_grid/fxGrid.h"

const int maxMEMO = 80;
/////////////////////////////////////////////////////////////////////////////
// CfxGridEx window

class CfxGridEx : public CfxGrid
{
// Construction
public:
	CfxGridEx(CWnd* pMainWnd, LOGFONT* logfont = nullptr);

// Attributes
public:
	CWnd*	m_pMainWnd;
	CArray	<bool, bool> m_memoAry;

	CRect	m_memoRC;
	class CMemo*	m_memoWnd;
	int	m_updateRow;
	CString	m_root;
	CString m_user;
	

// Operations
public:
	void	memoDraw(CDC* pDC);
	void	memoRefresh();
	bool	memoCheck();
	bool	memoCheck(int xrow);
	void	memoClear(int xrow);
	void	memoDropAction(int drag, int drop);
	void	memoWindow(CString code, CString name, CRect rect);
	void	memoWindowEdit(int row);

	void	drawIndicator1(CDC* dc, CRect rect, COLORREF clr);
	void	drawIndicator2(CDC* dc, CRect rect, COLORREF clr);
	void	Blink(int nRow, int nCol, COLORREF color, int interval = 500);
	BOOL	SetItemBkColor(int nRow, int nCol, COLORREF col = CLR_DEFAULT, bool force = false);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxGridEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxGridEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxGridEx)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LONG OnManage (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};