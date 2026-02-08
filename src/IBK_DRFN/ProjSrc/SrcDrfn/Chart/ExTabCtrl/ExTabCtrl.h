// ExTabCtrl.h : main header file for the EXTABCTRL DLL
//

#if !defined(AFX_EXTABCTRL_H__8E01BB58_BD76_4B23_B641_32AE6B62E455__INCLUDED_)
#define AFX_EXTABCTRL_H__8E01BB58_BD76_4B23_B641_32AE6B62E455__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExTabCtrlApp
// See ExTabCtrl.cpp for the implementation of this class
//

class CMember;
class CMemberDraw;
class CExTabCtrlApp : public CWinApp
{
public:
	CExTabCtrlApp();

	CMember*	m_pCurMember;

	long	ActionControl(LPVOID lpAction);
	void	ExitTabCtrl(LPVOID lpPointer);

	LPVOID	InitTabCtrlDraw(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void PreSubclassWindow();
	BOOL OnEraseBkgnd(CDC* pDC);
	void OnPaint(CDC* pDC);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void OnKillFocus(CWnd* pNewWnd);
	void OnSetFocus(CWnd* pOldWnd);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void	SetDefaultItemRect();
	void	OnSize(UINT nType, int cx, int cy);

	void	InsertItem(int nItem, LPSTR lpszItem, UINT nTab);
	void	InsertItem(int nItem, LPSTR lpszItem, int nImage, UINT nTab);
	void	InsertItem(int nItem, TCITEM* pTabCtrlItem, UINT nTab);

	void	OnMouseMove(UINT nFlags, CPoint point);

	void	DrawFocus(CDC &dc);
	void	DrawItemRect(CDC &dc, CRect &rcItem,int nHint=0);
	void	DrawItemInRect(CDC &dc, CRect &rcItem);
	UINT	FindLeftDownItem(CMemberDraw* pMember) const;
	bool	SetAutoRaising(bool bOn);

	void	AddChildControl(CWnd* pControl);
	void	RemoveChildControl(CWnd* pControl);
	void	SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive);
	void	SetSTTabColor(long lSTTabColor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExTabCtrlApp)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExTabCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTABCTRL_H__8E01BB58_BD76_4B23_B641_32AE6B62E455__INCLUDED_)
