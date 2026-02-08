#if !defined(AFX_EXTABCTRL_H__574AD52E_3158_4183_A0D5_D06037D0860D__INCLUDED_)
#define AFX_EXTABCTRL_H__574AD52E_3158_4183_A0D5_D06037D0860D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExTabCtrl window
class CLoaderTabCtrl;
class CExTabCtrl : public CTabCtrl
{
// Construction
public:
	CExTabCtrl(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderTabCtrl*		m_pLoader;

// Operations
public:
	void	InsertItem(int nItem, LPSTR lpszItem, UINT nTab = EXTAB_NORMAL);
	void	InsertItem(int nItem, LPSTR lpszItem, int nImage, UINT nTab);
	void	InsertItem(int nItem, TCITEM* pTabCtrlItem, UINT nTab = EXTAB_NORMAL);
	void	AddChildControl(CWnd* pControl);
	void	RemoveChildControl(CWnd* pControl);
	void	SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive);
	void	SetSTTabColor(long lSTTabColor);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExTabCtrl)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExTabCtrl();

protected:
	//{{AFX_MSG(CExTabCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSelchange ( NMHDR * pNotifyStruct, LRESULT* result );
	afx_msg BOOL OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTABCTRL_H__574AD52E_3158_4183_A0D5_D06037D0860D__INCLUDED_)
