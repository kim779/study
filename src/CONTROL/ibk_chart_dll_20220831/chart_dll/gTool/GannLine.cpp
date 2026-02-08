// GannLine.cpp: implementation of the CGannLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GannLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGannLine::CGannLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_basePOINT.x = m_basePOINT.y = 0;
}

CGannLine::~CGannLine()
{

}

void CGannLine::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen;
	CPoint sp, ep;
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

	CPen	*sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	if (CalculateDrawingPosition())
	{
		pDC->MoveTo(m_pointStart);
		pDC->LineTo(m_gannEXT);
	}

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)
		pDC->SelectClipRgn(NULL);
	
	CToolBase::InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);
}

void CGannLine::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CGannLine::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	int Width = m_rectGrp.Width() / 10;
	if (Width > 10)	Width = 10;

	m_pointEnd.Offset(Width, -Width);

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);

	CToolBase::InflateHolder();
}

void CGannLine::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);

	m_pointStart = pt;
	m_pointEnd = pt;
	int Width = m_rectGrp.Width() / 10;
	if (Width > 10)	Width = 10;
	m_pointEnd.Offset(Width, -Width);

	CToolBase::InflateHolder();
	DrawTool(&ClientDC);
}

void CGannLine::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	DrawTool(&ClientDC);
}

void CGannLine::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	if (m_rectDownHolder.PtInRect(pt))
	{
		m_eChgUpDn = enumUD::chgDN;
		m_pointStart = pt;
	}
	else if (m_rectUpHolder.PtInRect(pt))
	{
		m_eChgUpDn = enumUD::chgUP;
		m_pointEnd = pt;
	}

	m_bMove = TRUE;
	DrawTool(&ClientDC);

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CToolBase::InflateHolder();
}

void CGannLine::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	switch (m_eChgUpDn)
	{
	case enumUD::chgDN:
		m_pointStart = pt;
		break;
	case enumUD::chgUP:
		m_pointEnd = pt;
		break;
	case enumUD::chgNO:
	default:
		return;
	}
	DrawTool(&ClientDC);
	CToolBase::InflateHolder();
}

void CGannLine::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CGannLine::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointCur = pt;

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CClientDC ClientDC(m_pwndParent);
	if (!m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, true);
	m_bShowHolder = false;
	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	m_bMove = TRUE;
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CGannLine::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);
	DrawTool(&ClientDC);

	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	m_pointEnd.x += pt.x - m_pointCur.x;
	m_pointEnd.y += pt.y - m_pointCur.y;
	DrawTool(&ClientDC);

	m_pointCur = pt;
	CToolBase::InflateHolder();
}

void CGannLine::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CGannLine::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	CPoint	point[5];
	if (m_pointStart.x == m_gannEXT.x)	// 수직으로 일자인 경우 
	{
		point[0].x = m_pointStart.x - szREGION;
		point[0].y = m_pointStart.y;
		point[1].x = m_pointStart.x + szREGION;
		point[1].y = m_pointStart.y;
		point[2].x = m_gannEXT.x + szREGION;
		point[2].y = m_gannEXT.y;
		point[3].x = m_gannEXT.x - szREGION;
		point[3].y = m_gannEXT.y;
		point[4].x = m_pointStart.x - szREGION;
		point[4].y = m_pointStart.y;
	}
	else
	{
		point[0].x = m_pointStart.x;
		point[0].y = m_pointStart.y - szREGION;
		point[1].x = m_gannEXT.x;
		point[1].y = m_gannEXT.y - szREGION;
		point[2].x = m_gannEXT.x;
		point[2].y = m_gannEXT.y + szREGION;
		point[3].x = m_pointStart.x;
		point[3].y = m_pointStart.y + szREGION;
		point[4].x = m_pointStart.x;
		point[4].y = m_pointStart.y - szREGION;
	}

	CRgn GannRGN;
	GannRGN.CreatePolygonRgn(point, 5, WINDING);
	BOOL bInRgn = GannRGN.PtInRegion(pt); 		
	GannRGN.DeleteObject();	

	return (bInRgn ? true : false);
}

/////////////////////////////////////////////////////////////////////////////
// private member function

bool CGannLine::CalculateDrawingPosition()
{
	enum whichAREA {noneAREA = 0, firstAREA, secondAREA, thirdAREA, fourthAREA};

	whichAREA which = noneAREA;
	if (m_pointStart.x <= m_pointEnd.x)
	{
		if (m_pointStart.y > m_pointEnd.y)
		{
			m_basePOINT.x = m_rectChart.right;
			m_basePOINT.y = m_rectChart.top;
			which = firstAREA;
		}
		else if (m_pointStart.y < m_pointEnd.y) 
		{
			m_basePOINT.x = m_rectChart.right;
			m_basePOINT.y = m_rectChart.bottom;
			which = fourthAREA;
		}
	}
	else
	{
		if (m_pointStart.y > m_pointEnd.y)
		{
			m_basePOINT.x = m_rectChart.left;
			m_basePOINT.y = m_rectChart.top;
			which = secondAREA;
		}
		else if (m_pointStart.y < m_pointEnd.y)
		{
			m_basePOINT.x = m_rectChart.left;
			m_basePOINT.y = m_rectChart.bottom;
			which = thirdAREA;
		}
	}

	if (which == noneAREA)
		return false;

	int	Width = abs(m_pointStart.x - m_pointEnd.x);
	int	Height = abs(m_pointStart.y - m_pointEnd.y);
	if (Width < 1)	Width = 1;
	double	gannRATE = double(double(Height) / double(Width));

	Width  = abs(m_basePOINT.x - m_pointStart.x);
	Height = abs(m_basePOINT.y - m_pointStart.y);
	if (Width < 1)	Width = 1;
	double baseRATE = double(Height) / double(Width);

	if (gannRATE == 0)	gannRATE = 1;
	switch (which)				// gann line ext
	{
	case firstAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.top);
			m_gannEXT.x = m_pointStart.x + int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.top;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.right);
			m_gannEXT.x = m_rectChart.right;
			m_gannEXT.y = m_pointStart.y - int(Width * gannRATE);
		}
		break;
	case secondAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.top);
			m_gannEXT.x = m_pointStart.x - int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.top;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.left);
			m_gannEXT.x = m_rectChart.left;
			m_gannEXT.y = m_pointStart.y - int(Width * gannRATE);

		}
		break;
	case thirdAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.bottom);
			m_gannEXT.x = m_pointStart.x - int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.bottom;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.left);
			m_gannEXT.x = m_rectChart.left;
			m_gannEXT.y = m_pointStart.y + int(Width * gannRATE);

		}
		break;
	case fourthAREA:
		if (gannRATE > baseRATE)
		{
			Height = abs(m_pointStart.y - m_rectChart.bottom);
			m_gannEXT.x = m_pointStart.x + int(Height / gannRATE);
			m_gannEXT.y = m_rectChart.bottom;
		}
		else
		{
			Width = abs(m_pointStart.x - m_rectChart.right);
			m_gannEXT.x = m_rectChart.right;
			m_gannEXT.y = m_pointStart.y + int(Width * gannRATE);
		}
		break;
	}

	return true;
}
