// ArrowTool.cpp: implementation of the CArrowTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArrowTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	szArrow 3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArrowTool::CArrowTool(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_bClipRegion = true;
	m_bShowHolder = true;

	switch(m_eToolKind)
	{
	case enumTK::tkUARROW:
		m_arrowRGB = RGB(255,0,0) ^ RGB(255,255,255);
		break;
	case enumTK::tkDARROW:
		m_arrowRGB = RGB(0,0,255) ^ RGB(255,255,255);
		break;
	case enumTK::tkLARROW:
	case enumTK::tkRARROW:
		m_arrowRGB = RGB(1,242,1) ^ RGB(255,255,255);
		break;
	}
}

CArrowTool::~CArrowTool()
{

}

void CArrowTool::DrawTool(CDC* pDC)
{
	CPen	*cPen;
	CPoint	sp, ep;
	CBrush	*cBrush;
	cBrush = GetBrush(m_pwndView, m_arrowRGB);
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);
		m_pointStart = sp;
		m_pointEnd = ep;

		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
	}
	else
	{
		sp = m_pointStart;
		ep = m_pointEnd;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(ep, m_dPosEnd, m_dValueEnd);

		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen*	sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);

	if (calculateDrawingPosition())
	{
		CRgn rgn;
		rgn.CreatePolygonRgn(m_pt, 8, WINDING);
		pDC->FillRgn(&rgn, cBrush);
		rgn.DeleteObject();	
	}

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
}

void CArrowTool::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}
	
void CArrowTool::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{	
	m_pointStart = m_pointEnd = pt;
	m_bMove = TRUE;
	m_bShowHolder = false;
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CArrowTool::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	m_pointStart = m_pointEnd = pt;
	DrawTool(&ClientDC);
}

void CArrowTool::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	m_bMove = FALSE;
	m_bShowHolder = true;
	DrawTool(&ClientDC);
}

void CArrowTool::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;
	
	CClientDC ClientDC(m_pwndParent);
	m_bMove = TRUE;
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CArrowTool::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	m_pointStart = m_pointEnd = pt;
	DrawTool(&ClientDC);
}

void CArrowTool::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;

	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

bool CArrowTool::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	calculateDrawingPosition();
	CRgn rgn;
	rgn.CreatePolygonRgn(m_pt, 8, WINDING);

	BOOL bInRgn = rgn.PtInRegion(pt);
	rgn.DeleteObject();

	return (bInRgn ? true : false);
}

bool CArrowTool::calculateDrawingPosition()
{
	switch (m_eToolKind)
	{
	case enumTK::tkUARROW:		// ^ 화살표 상
		m_pt[0].x = m_pointStart.x;
		m_pt[0].y = m_pointStart.y - szArrow * 3;
		m_pt[1].x = m_pointStart.x + szArrow * 3;
		m_pt[1].y = m_pointStart.y;
		m_pt[2].x = m_pointStart.x + szArrow;
		m_pt[2].y = m_pointStart.y;
		m_pt[3].x = m_pointStart.x + szArrow;
		m_pt[3].y = m_pointStart.y + szArrow * 3;
		m_pt[4].x = m_pointStart.x - szArrow;
		m_pt[4].y = m_pointStart.y + szArrow * 3;
		m_pt[5].x = m_pointStart.x - szArrow;
		m_pt[5].y = m_pointStart.y;
		m_pt[6].x = m_pointStart.x - szArrow * 3;
		m_pt[6].y = m_pointStart.y;
		m_pt[7].x = m_pointStart.x;
		m_pt[7].y = m_pointStart.y - szArrow * 3;
		break;

	case enumTK::tkDARROW:			// 화살표 하
		m_pt[0].x = m_pointStart.x - szArrow;
		m_pt[0].y = m_pointStart.y - szArrow * 3;
		m_pt[1].x = m_pointStart.x + szArrow;
		m_pt[1].y = m_pointStart.y - szArrow * 3;
		m_pt[2].x = m_pointStart.x + szArrow;
		m_pt[2].y = m_pointStart.y;
		m_pt[3].x = m_pointStart.x + szArrow * 3;
		m_pt[3].y = m_pointStart.y;
		m_pt[4].x = m_pointStart.x;
		m_pt[4].y = m_pointStart.y + szArrow * 3;
		m_pt[5].x = m_pointStart.x - szArrow * 3;
		m_pt[5].y = m_pointStart.y;
		m_pt[6].x = m_pointStart.x - szArrow;
		m_pt[6].y = m_pointStart.y;
		m_pt[7].x = m_pointStart.x - szArrow;
		m_pt[7].y = m_pointStart.y - szArrow * 3;
		break;

	case enumTK::tkLARROW:			// <- 화살표 좌 
		m_pt[0].x = m_pointStart.x;
		m_pt[0].y = m_pointStart.y - szArrow * 3;
		m_pt[1].x = m_pointStart.x;
		m_pt[1].y = m_pointStart.y - szArrow;
		m_pt[2].x = m_pointStart.x + szArrow * 3;
		m_pt[2].y = m_pointStart.y - szArrow;
		m_pt[3].x = m_pointStart.x + szArrow * 3;
		m_pt[3].y = m_pointStart.y + szArrow;
		m_pt[4].x = m_pointStart.x;
		m_pt[4].y = m_pointStart.y + szArrow;
		m_pt[5].x = m_pointStart.x;
		m_pt[5].y = m_pointStart.y + szArrow * 3;
		m_pt[6].x = m_pointStart.x - szArrow * 3;
		m_pt[6].y = m_pointStart.y;
		m_pt[7].x = m_pointStart.x;
		m_pt[7].y = m_pointStart.y - szArrow * 3;
		break;

	case enumTK::tkRARROW:			// -> 화살표 우
		m_pt[0].x = m_pointStart.x;
		m_pt[0].y = m_pointStart.y - szArrow * 3;
		m_pt[1].x = m_pointStart.x + szArrow * 3;
		m_pt[1].y = m_pointStart.y;

		m_pt[2].x = m_pointStart.x;
		m_pt[2].y = m_pointStart.y + szArrow * 3;
		m_pt[3].x = m_pointStart.x;
		m_pt[3].y = m_pointStart.y + szArrow;

		m_pt[4].x = m_pointStart.x - szArrow * 3;
		m_pt[4].y = m_pointStart.y + szArrow;
		m_pt[5].x = m_pointStart.x - szArrow * 3;
		m_pt[5].y = m_pointStart.y - szArrow;

		m_pt[6].x = m_pointStart.x;
		m_pt[6].y = m_pointStart.y - szArrow;
		m_pt[7].x = m_pointStart.x;
		m_pt[7].y = m_pointStart.y - szArrow * 3;
		break;
	}

	return true;
}

void CArrowTool::DrawHolder(CDC *pDC, bool showHolder)
{
	CPen	*cPen;
	if (!m_bMove)
		cPen = GetPen(m_pwndView, PS_SOLID, 1, m_rgbText);
	else
		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	CPen	*sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	m_bShowHolder = showHolder;

	pDC->MoveTo(m_pointStart.x - szArrow * 3 - 1, m_pointStart.y - szArrow * 3 - 1);
	pDC->LineTo(m_pointStart.x + szArrow * 3 + 1, m_pointStart.y - szArrow * 3 - 1);
	pDC->LineTo(m_pointStart.x + szArrow * 3 + 1, m_pointStart.y + szArrow * 3 + 1);
	pDC->LineTo(m_pointStart.x - szArrow * 3 - 1, m_pointStart.y + szArrow * 3 + 1);
	pDC->LineTo(m_pointStart.x - szArrow * 3 - 1, m_pointStart.y - szArrow * 3 - 1);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);
}
