// ExComboBox.cpp: implementation of the CExComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExComboBox.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CExComboBox, CComboBox)

/////////////////////////////////////////////////////////////////////////////
// CExComboBox

CExComboBox::CExComboBox(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderComboBox(this, bUseDefSkin);
}

CExComboBox::~CExComboBox()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExComboBox, CComboBox)
	//{{AFX_MSG_MAP(CExComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExComboBox message handlers

void CExComboBox::PreSubclassWindow() 
{
	m_pLoader->PreSubclassWindow();
}

void CExComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	m_pLoader->DrawItem(lpDrawItemStruct);
}

void CExComboBox::OnPaint() 
{
	Default();
	m_pLoader->OnPaint();
}

void CExComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_pLoader->OnMouseMove(nFlags, point);
}

void CExComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pLoader->OnLButtonDown(nFlags, point);
}

void CExComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pLoader->OnLButtonUp(nFlags, point);
}

void CExComboBox::OnTimer(UINT nIDEvent) 
{
	m_pLoader->OnTimer(nIDEvent);
}
