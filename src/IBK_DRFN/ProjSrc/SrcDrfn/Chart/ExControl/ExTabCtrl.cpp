// ExTabCtrl.cpp: implementation of the CExTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExTabCtrl.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExTabCtrl

CExTabCtrl::CExTabCtrl(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderTabCtrl(this, bUseDefSkin);
}

CExTabCtrl::~CExTabCtrl()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CExTabCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGING, OnSelchanging)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave) 
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExTabCtrl message handlers

void CExTabCtrl::InsertItem(int nItem, LPSTR lpszItem, UINT nTab/* = EXTAB_NORMAL*/)
{
	m_pLoader->InsertItem(nItem, lpszItem, nTab);
}

void CExTabCtrl::InsertItem(int nItem, LPSTR lpszItem, int nImage, UINT nTab/* = EXTAB_NORMAL*/)
{
	m_pLoader->InsertItem(nItem, lpszItem, nItem, nTab);
}

void CExTabCtrl::InsertItem(int nItem, TCITEM* pTabCtrlItem, UINT nTab/* = EXTAB_NORMAL*/)
{
	m_pLoader->InsertItem(nItem, pTabCtrlItem, nTab);
}

void CExTabCtrl::PreSubclassWindow() 
{
	m_pLoader->PreSubclassWindow();
}

void CExTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	m_pLoader->DrawItem(lpDrawItemStruct);
}

void CExTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_pLoader->OnMouseMove(nFlags, point);
}

BOOL CExTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return m_pLoader->OnEraseBkgnd(pDC);
}

void CExTabCtrl::OnPaint() 
{
	CPaintDC dc(this);
	m_pLoader->OnPaint(&dc);
}

void CExTabCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_pLoader->OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CExTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	return m_pLoader->OnSelchange(pNMHDR, pResult);
}

BOOL CExTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	return m_pLoader->OnSelchanging(pNMHDR, pResult);
}

LRESULT CExTabCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	return m_pLoader->OnMouseLeave(wParam, lParam);
}

void CExTabCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	m_pLoader->OnKillFocus(pNewWnd);
}

void CExTabCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	m_pLoader->OnSetFocus(pOldWnd);
}

LRESULT CExTabCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return m_pLoader->WindowProc(message, wParam, lParam);
}

void CExTabCtrl::AddChildControl(CWnd* pControl)
{
	m_pLoader->AddChildControl(pControl);
}

void CExTabCtrl::RemoveChildControl(CWnd* pControl)
{
	m_pLoader->RemoveChildControl(pControl);
}

void CExTabCtrl::SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive)
{
	m_pLoader->SetTabColor(clrBkgrnd, clrActive, clrInactive);
}

void CExTabCtrl::SetSTTabColor(long lSTTabColor)
{
	m_pLoader->SetSTTabColor(lSTTabColor);
}

void CExTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	m_pLoader->OnSize(nType, cx, cy);
}