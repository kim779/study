// CrossLine.cpp: implementation of the CCrossLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Tick.h"
#include "CrossLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossLine::CCrossLine(CWnd *pView, CWnd *pParent, COLORREF bg, COLORREF fg)
{
	m_pView = pView;
	m_pParent = pParent;
	m_fgColor = fg;
	m_bgColor = bg;
	m_DrawRect.SetRectEmpty();
	m_prePt = m_curPt = CPoint(0, 0);
	m_pApp = (CC_TickApp *)AfxGetApp();
}

CCrossLine::~CCrossLine()
{

}

void CCrossLine::Resize(CRect DrawRect)
{
	m_DrawRect = DrawRect;
}

void CCrossLine::DrawCrossLine(CDC *pDC)
{
	if (m_curPt.x == 0)
		return;

	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_fgColor^m_bgColor);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = m_pApp->GetBrush(m_pView, m_fgColor^m_bgColor);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	const int sROP = pDC->SetROP2(R2_XORPEN);
	const int sMode = pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(m_DrawRect.left, m_curPt.y);
	pDC->LineTo(m_DrawRect.right, m_curPt.y);
	pDC->MoveTo(m_curPt.x, m_DrawRect.top);
	pDC->LineTo(m_curPt.x, m_DrawRect.bottom);

	pDC->SetROP2(sROP);
	pDC->SetBkMode(sMode);
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
}

void CCrossLine::ShowCrossLine(CPoint pt)
{
	CClientDC ClientDC(m_pParent);

	if (m_prePt.x > 0)
	{
		m_curPt = m_prePt;
		DrawCrossLine(&ClientDC);
	}

	m_curPt = pt;
	DrawCrossLine(&ClientDC);

	m_prePt = m_curPt = pt;
}

void CCrossLine::HideCrossLine(CPoint pt)
{
	CClientDC ClientDC(m_pParent);

	if (m_curPt.x > 0)
		DrawCrossLine(&ClientDC);

	m_prePt = m_curPt = CPoint(0, 0);
}

void CCrossLine::SetColor(COLORREF bg, COLORREF fg)
{
	m_fgColor = fg;
	m_bgColor = bg;
}

