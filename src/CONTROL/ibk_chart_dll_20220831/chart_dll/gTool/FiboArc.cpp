// FiboArc.cpp: implementation of the CFiboArc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboArc.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double	fiboARC[] = {38.2, 50.0, 61.8};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFiboArc::CFiboArc(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_bUPPER = false;
}

CFiboArc::~CFiboArc()
{

}

void CFiboArc::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen, *yellowPen;

	CPoint sp, ep;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);
		m_pointStart = sp;
		m_pointEnd = ep;

		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
		yellowPen = GetPen(m_pwndView, PS_SOLID, 1, _YELLOW ^ m_rgbBG);
	}
	else
	{
		sp = m_pointStart;
		ep = m_pointEnd;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(ep, m_dPosEnd, m_dValueEnd);

		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		yellowPen = GetPen(m_pwndView, PS_DOT, 1, _YELLOW);
	}	

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen*	sPen = pDC->SelectObject(yellowPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	CalculateDrawingPosition();

	pDC->MoveTo(m_pointStart);
	pDC->LineTo(m_pointEnd);

	int	Width;
	bool	bDraw2;
	double	radius, dbTemp;
	CPoint	ptStart1, ptEnd1, ptStart2, ptEnd2, ptCenter;

	pDC->SelectObject(cPen);
	for (int ii = 0; ii < maxFIBOARC; ii++)
	{
		bDraw2 = false;

		Width  = m_ArcRECT[ii].Width() /2;
		radius = double((Width * Width) + (Width * Width));
		radius = sqrt(radius);
		dbTemp = (radius * radius) - (Width * Width);
		dbTemp = sqrt(dbTemp);

		if (m_bUPPER)			// 상단
		{
			ptCenter.y = m_pointStart.y - int(dbTemp);
			ptCenter.x = m_pointStart.x;

			ptStart1.x = m_ArcRECT[ii].right;
			ptStart1.y = m_pointStart.y;
			ptEnd1.x = m_ArcRECT[ii].left;
			ptEnd1.y = m_pointStart.y;
			ptStart2 = ptStart1; ptEnd2 = ptEnd1;
		}
		else		// 하단
		{
			bDraw2 = false;

			Width  = m_ArcRECT[ii].Width() /2;
			radius = double((Width * Width) + (Width * Width));
			radius = sqrt(radius);
			dbTemp = (radius * radius) - (Width * Width);
			dbTemp = sqrt(dbTemp);

			ptCenter.y = m_pointStart.y + int(dbTemp);
			ptCenter.x = m_pointStart.x;

			ptStart1.x = m_ArcRECT[ii].left;
			ptStart1.y = m_pointStart.y;
			ptEnd1.x = m_ArcRECT[ii].right;
			ptEnd1.y = m_pointStart.y;
			ptStart2 = ptStart1; ptEnd2 = ptEnd1;
		}

		int available = abs(ptStart1.x - m_rectGrp.right);
		if (available > 4)
			pDC->Arc(&m_ArcRECT[ii], ptStart1, ptEnd1);

	}
	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
	
	CToolBase::InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);
}

void CFiboArc::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CFiboArc::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	CToolBase::InflateHolder();
}

void CFiboArc::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	DrawTool(&ClientDC);
	m_pointStart = pt;
	CToolBase::InflateHolder();
	DrawTool(&ClientDC);
}

void CFiboArc::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
}

void CFiboArc::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CFiboArc::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CFiboArc::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CFiboArc::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CFiboArc::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CFiboArc::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CFiboArc::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	CPoint	point[5];
	if (m_pointStart.x == m_pointEnd.x)	// 수직으로 일자인 경우 
	{
		point[0].x = m_pointStart.x - szREGION;
		point[0].y = m_pointStart.y;
		point[1].x = m_pointStart.x + szREGION;
		point[1].y = m_pointStart.y;
		point[2].x = m_pointEnd.x + szREGION;
		point[2].y = m_pointEnd.y;
		point[3].x = m_pointEnd.x - szREGION;
		point[3].y = m_pointEnd.y;
		point[4].x = m_pointStart.x - szREGION;
		point[4].y = m_pointStart.y;
	}
	else
	{
		point[0].x = m_pointStart.x;
		point[0].y = m_pointStart.y - szREGION;
		point[1].x = m_pointEnd.x;
		point[1].y = m_pointEnd.y - szREGION;
		point[2].x = m_pointEnd.x;
		point[2].y = m_pointEnd.y + szREGION;
		point[3].x = m_pointStart.x;
		point[3].y = m_pointStart.y + szREGION;
		point[4].x = m_pointStart.x;
		point[4].y = m_pointStart.y - szREGION;
	}

	CRgn ArcRGN;
	ArcRGN.CreatePolygonRgn(point, 5, WINDING);
	BOOL bInRgn = ArcRGN.PtInRegion(pt);
	ArcRGN.DeleteObject();
	if (bInRgn)
		return true;

	if (m_bUPPER && m_pointStart.y < pt.y)
		return false;
	if (!m_bUPPER && m_pointStart.y > pt.y)
		return false;

	int	response;
	CRgn	ArcRGN1,  ArcRGN2;
	CRect	arcRECT1, arcRECT2;
	for (int ii = 0; ii < maxFIBOARC; ii++)
	{
		arcRECT1 = m_ArcRECT[ii];
		arcRECT1.InflateRect(szREGION, szREGION);
		ArcRGN1.CreateEllipticRgnIndirect(&arcRECT1);

		arcRECT2 = m_ArcRECT[ii];
		arcRECT2.DeflateRect(szREGION, szREGION);
		ArcRGN2.CreateEllipticRgnIndirect(&arcRECT2);

		ArcRGN.CreateEllipticRgnIndirect(&m_ArcRECT[ii]);
		response = ArcRGN.CombineRgn(&ArcRGN1, &ArcRGN2, RGN_XOR);
		switch (response)
		{
		case COMPLEXREGION:
			bInRgn = ArcRGN.PtInRegion(pt);
			break;
		case ERROR:
			TRACE("CombineRgn error...[ERROR]\n");
			bInRgn = FALSE;	break;
		case SIMPLEREGION:
			TRACE("CombineRgn error...[SIMPLEREGION]\n");
			bInRgn = FALSE; break;
		default:
		case NULLREGION:
			TRACE("CombineRgn error...[NULLREGION]\n");
			bInRgn = FALSE;	break;
		}
		ArcRGN.DeleteObject();
		ArcRGN1.DeleteObject();
		ArcRGN2.DeleteObject();

		if (bInRgn)
			return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// private member function

void CFiboArc::CalculateDrawingPosition()
{
	if (m_pointStart.y >= m_pointEnd.y)
		m_bUPPER = true;
	else
		m_bUPPER = false;

	int	posRATE = 0;
	int	xLen = abs(m_pointStart.x - m_pointEnd.x);
	int	yLen = abs(m_pointStart.y - m_pointEnd.y);
	int	maxLEN = int(pow(xLen * xLen + yLen * yLen, 0.5));
//	int	maxLEN = max(abs(m_pointStart.x - m_pointEnd.x), abs(m_pointStart.y - m_pointEnd.y));
	for (int ii = 0; ii < maxFIBOARC; ii++)
	{
		posRATE = int(maxLEN * fiboARC[ii] / 100.);

		m_ArcRECT[ii].left  = m_pointStart.x - posRATE;
		m_ArcRECT[ii].right = m_pointStart.x + posRATE;
		m_ArcRECT[ii].top   = m_pointStart.y - posRATE;
		m_ArcRECT[ii].bottom = m_pointStart.y + posRATE;
	}
	return;
}
