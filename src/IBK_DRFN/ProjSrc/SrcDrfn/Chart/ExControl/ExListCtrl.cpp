// ExListCtrl.cpp: implementation of the CExListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExListCtrl.h"
#include "include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl

CExListCtrl::CExListCtrl(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderListCtrl(this, bUseDefSkin);
}

CExListCtrl::~CExListCtrl()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExListCtrl)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExListCtrl message handlers
BOOL CExListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return m_pLoader->OnEraseBkgnd(pDC);
}

void CExListCtrl::OnDestroy()
{
	m_pLoader->OnDestroyPre();
}

void CExListCtrl::DisableColumnSizing(BOOL bDisable)
{
	m_pLoader->DisableColumnSizing(bDisable);
}

void CExListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	m_pLoader->DrawItem(lpDrawItemStruct);
}

void CExListCtrl::PreSubclassWindow() 
{
	m_pLoader->PreSubclassWindow();
}

BOOL CExListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return m_pLoader->OnNotify(wParam, lParam, pResult);
}

//void CExListCtrl::SetHeaderHeight(int nHeight)
//{
//	m_pLoader->SetHeaderHeight(nHeight);
//}

void CExListCtrl::SetRowHeight(int nHeight)
{
	m_pLoader->SetRowHeight(nHeight);
}

void CExListCtrl::SetAutoColSize(BOOL bAutoSize)
{
	m_pLoader->SetAutoColSize(bAutoSize);
}

void CExListCtrl::SetTwotoneRowColor(int nRow/* = 1*/)
{
	m_pLoader->SetTwotoneRowColor(nRow);
}
