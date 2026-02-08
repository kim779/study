// ColoredStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColoredStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredStatic

CColoredStatic::CColoredStatic()
{
	m_bCenter = FALSE;
	m_bHasBorder = FALSE;
	m_bHasBkColor = FALSE;
	m_clrBorder = RGB(0,0,0);
	m_clrBackbround = RGB(255,255,255);
}

CColoredStatic::~CColoredStatic()
{
}


BEGIN_MESSAGE_MAP(CColoredStatic, CStatic)
	//{{AFX_MSG_MAP(CColoredStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredStatic message handlers

void CColoredStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);

	if(m_bHasBkColor)
	{
		dc.FillSolidRect(rect,m_clrBackbround);
		dc.SetBkColor(m_clrBackbround);
	}

	if(m_bHasBorder)
	{
		dc.FrameRect(rect, &m_brBorder);
	}

	CFont *pFt = GetParent()->GetFont();
	HFONT hfontOld = (HFONT)SelectObject(dc.m_hDC,(HFONT)pFt->m_hObject);
	
	if(m_bCenter)
		::DrawText(dc, m_strText, -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	else
	{
		rect.left+=10;
		rect.top+=5;
		::DrawText(dc, m_strText, -1, &rect, DT_SINGLELINE|DT_LEFT);
	}	
	SelectObject(dc.m_hDC,hfontOld);
	// Do not call CStatic::OnPaint() for painting messages
}

void CColoredStatic::SetBorder(BOOL bUse, COLORREF clrBorder /*= RGB(0,0,0)*/)
{
	m_bHasBorder = bUse;
	m_clrBorder = clrBorder;
	m_brBorder.CreateSolidBrush(clrBorder);
}


void CColoredStatic::SetWindowText(LPCSTR lpszText)
{
	m_strText = lpszText;
	Invalidate(TRUE);
}


void CColoredStatic::SetBkColor(BOOL bUse, COLORREF clrBk/*= RGB(255,255,255)*/)
{
	m_bHasBkColor = bUse;
	m_clrBackbround = clrBk;
}

CString CColoredStatic::GetWindowText()
{
	return m_strText;
}

void CColoredStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_brBorder.GetSafeHandle())
	{
		m_brBorder.DeleteObject();
	}
}

