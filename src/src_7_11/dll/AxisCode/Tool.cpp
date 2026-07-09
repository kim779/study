// Draw.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTool::CTool()
{
//	m_Font.SetMyFont(12, "±¼¸²");
	m_Font.CreatePointFont(9*10, "±¼¸²");

}

CTool::~CTool()
{
	m_pDC = NULL;
	m_Font.DeleteObject();
}

BOOL CTool::ScrollDC( int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, CRgn* pRgnUpdate, LPRECT lpRectUpdate )
{
	return m_pDC->ScrollDC( dx, dy, lpRectScroll, lpRectClip, pRgnUpdate, lpRectUpdate );
}


void CTool::WriteText(CString szText, CRect rc, COLORREF fcolor, UINT nFormat)
{	
	CFont *pfont;

	pfont = (CFont*)m_pDC->SelectObject(&m_Font);
	
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetTextColor(fcolor);
	m_pDC->DrawText(szText, rc, nFormat);
	
	m_pDC->SelectObject(pfont);
}


void CTool::DrawCell(CRect rc, COLORREF lcolor)
{
	CPen pen(PS_SOLID, 1, lcolor), *poldpen;

	poldpen = m_pDC->SelectObject(&pen);

	m_pDC->MoveTo(rc.TopLeft());
	m_pDC->LineTo(rc.right, rc.top);

	m_pDC->MoveTo(rc.left, rc.top);
	m_pDC->LineTo(rc.left, rc.bottom);

	m_pDC->MoveTo(rc.right, rc.top);
	m_pDC->LineTo(rc.right, rc.bottom + 1);

	m_pDC->MoveTo(rc.left, rc.bottom);
	m_pDC->LineTo(rc.BottomRight());

	m_pDC->SelectObject(poldpen);
	pen.DeleteObject();
}

void CTool::FrameRect(CRect rc, COLORREF color, COLORREF frame, BOOL bline )
{
	m_pDC->FillSolidRect(rc, color);
	
	if(bline)
		m_pDC->Draw3dRect(rc, frame, frame);
}

void CTool::FrameRect2(CRect rc, COLORREF color, COLORREF frame, COLORREF frame2, BOOL bline )
{
	m_pDC->FillSolidRect(rc, color);
	
	if(bline)
		m_pDC->Draw3dRect(rc, frame, frame2);
}


void CTool::DeleteDC()
{
	m_pDC->DeleteDC();
}
