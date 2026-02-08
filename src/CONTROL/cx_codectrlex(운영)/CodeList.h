#pragma once

#include "NButton.h"

// CodeList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeList window

class CCodeList : public CWnd
{
// Construction
public:
	CCodeList(CWnd* pWnd, CWnd* pWizard, CString home);
	virtual ~CCodeList();

	void	AddItem(CString str);
	void	AddSFItem(CString str);
	int	GetCount(){ return m_arrData.GetSize(); }
	BOOL	Create(CRect rc, CWnd* parent);
	void	Update();
	int	GetCurSel();
	void	SetTopIndex(int index);
	void	SetCurSel(int cur);
	int	GetTopIndex();
	void	RemoveAll();

	CString GetItem(int item);
	CString Parser(CString &srcstr, CString substr);
	CString GetRawItem(int item);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeList)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetItemFont(CFont* m_font);
	void Init();
	int m_RowHeight;
	std::unique_ptr<CNButton> m_pCtlBtnDeleteAll;

private:
	class CfxCodeCtrl* m_pParent;	
	CWnd*	m_pWizard;
	CString	m_home;
	
	CStringArray	m_arrData;
	long	m_Hscroll;
	long	m_Vscroll;
	int	m_iStart;
	int	m_iSelRow;
	BOOL    m_bFocus;
	CPoint  m_pointPrev;

	std::unique_ptr<CNButton> m_pCtlBtnDelete;
	CFont	m_Font;
	CRect	m_rcSelRow;

	bool	m_bTracking;
	bool	m_bWheel;

private:
	long	GetVirtualHeight();
	int	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	void	ResetScrollBars();
	void	DrawCell(CDC *pDC, CRect rc, int row);
	int	GetCurrentRow(CPoint pt, CRect* pRect);
	int	GetVisibleRow();


	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseHook(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDeleteBtn();
	afx_msg void OnAllDeleteBtn();
	DECLARE_MESSAGE_MAP()
};