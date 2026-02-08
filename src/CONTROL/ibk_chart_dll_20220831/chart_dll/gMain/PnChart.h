#if !defined(AFX_PNCHART_H__A40250A0_C149_4E1D_BD35_7463BCC37DC6__INCLUDED_)
#define AFX_PNCHART_H__A40250A0_C149_4E1D_BD35_7463BCC37DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnChart window
#include "../gData/DataMgr.h"
#include "../gIndc/GraphMgr.h"


class CPnChart : public CWnd
{
// Construction
public:
	CPnChart();

	CWnd *CreateCtrl(CWnd* pView, CWnd *pParent, char *info, CFont* pFont);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnChart)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnChart();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnChart)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnChartEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnViewEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInputEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSiseEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTableEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExtMessage(WPARAM, LPARAM);
	//** macho add
	afx_msg LRESULT OnAidEvent(WPARAM, LPARAM);
private:
	void	ExcelProc();
	BOOL	IsPtInGrp();
private:
	CWnd*		m_pwndView;
	CWnd*		m_pwndGView;
	CFont*		m_pFont;

	COLORREF	m_crBG;

	class CObjMgr*	m_pObjMgr;
	CString		m_strRtnData;	// 2006.11.20 lsh
protected:
	COLORREF m_stanLineColor;
	int		 m_stanLineWidth;
	CString	 m_stanLine;

	void	 InitStanLine();
	void	 SetStanLine(int lineWidth, COLORREF lineColor);
	void	 SetStanLine(LPCTSTR datetime);
	void	 DeleteStanLine();

	LRESULT  StanLineProc(UINT job, LPARAM lParam);
public:
	BOOL	 HasStanLine();
	void	 DrawStanLine(CDC* dc, CIndcBase* indc);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNCHART_H__A40250A0_C149_4E1D_BD35_7463BCC37DC6__INCLUDED_)
