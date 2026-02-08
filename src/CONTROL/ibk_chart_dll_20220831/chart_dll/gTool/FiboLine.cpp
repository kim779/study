// FiboLine.cpp: implementation of the CFiboLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int fiboLINE[] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFiboLine::CFiboLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_szOneDay = 0.0;
}

CFiboLine::~CFiboLine()
{

}

void CFiboLine::ResizeTool(CRect drawRect, CRect toolRect)
{
	m_rectGrp = drawRect;
	m_rectChart = toolRect;
}

void CFiboLine::DrawTool(CDC* pDC)
{
	CPen	*defaultPen, *yellowPen;
	CPoint	drawUP, drawDN;

	CPoint sp;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pointStart = sp;
		defaultPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
		yellowPen = GetPen(m_pwndView, PS_SOLID, 1, _YELLOW ^ m_rgbBG);
	}
	else
	{
		sp = m_pointStart;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		defaultPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		yellowPen = GetPen(m_pwndView, PS_DOT, 1, _YELLOW);
	}

	if (m_bClipRegion)	pDC->IntersectClipRect(&m_rectGrp);

	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);
	CPen*	sPen = pDC->SelectObject(yellowPen);
//	m_pointStart = CenterPointAdjust(m_pointStart);

	CPoint	dn, up;
	double	pos1, pos2, value;
	double fUnit;
	m_pIndcBase->GetPtToPosValue(m_pointStart, pos1, value);
	pos1 = (int)pos1 + 0.5;
	for (int ii = 0; ii < maxFIBOLINE; ii++)
	{
		drawUP.x = m_pointStart.x;
		drawUP.y = m_rectGrp.top;
		drawDN.x = m_pointStart.x;
		drawDN.y = m_rectGrp.bottom;

		if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
		{
			pos2 = pos1 + fiboLINE[ii];
			m_pIndcBase->GetValueToPoint(dn, pos2, value);
		}
		else
		{
			dn.x = (int) (m_pointStart.x + ((fiboLINE[ii] + 0.5) * fUnit));
		}
		drawUP.x = dn.x;
		drawDN.x = drawUP.x;
//		if (drawUP.x >= m_rectGrp.right)
//			continue;

		pDC->MoveTo(drawUP);
		pDC->LineTo(drawDN);
		if (ii == 0)
			pDC->SelectObject(defaultPen);
	}

	pDC->SelectObject(sPen);
	pDC->SetROP2(sROP);
	pDC->SetBkMode(bkMode);

	InflateHolder();
	if (m_bShowHolder)
		DrawHolder(pDC, true);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
}

void CFiboLine::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CFiboLine::DrawHolder(CDC *pDC, bool showHolder)
{
	CClientDC ClientDC(m_pwndParent);

	CBrush	*cBrush = GetBrush(m_pwndView, m_rgbHolder);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	int	sROP;

	if (showHolder)
	{
		m_bShowHolder = true;
		sROP = pDC->SetROP2(R2_XORPEN);
	}
	else
	{
		m_bShowHolder = false;
		sROP = pDC->SetROP2(R2_XORPEN);
	}

	for (int ii = 0; ii < maxFIBOLINE; ii++)
	{
		if (m_cntRcHolder[ii].right >= m_rectGrp.right)
			break;
		pDC->Rectangle(&m_cntRcHolder[ii]);
	}

	pDC->SetROP2(sROP);
	pDC->SelectObject(sBrush);
}

void CFiboLine::HideHolder()
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CFiboLine::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	double	pos1, pos2, value;
	m_pIndcBase->GetPtToPosValue(m_pointStart, pos1, value);
	pos1 = (int)pos1 + 0.5;
	double fUnit;
	for (int ii = 0; ii < maxFIBOLINE; ii++)
	{
		if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
		{
			pos2 = pos1 + fiboLINE[ii];
			m_pIndcBase->GetValueToPoint(dn, pos2, value);
		}
		else
		{
			dn = m_pointStart;
			dn.x += (int)((fUnit /2) + (fiboLINE[ii] * fUnit));
		}
	

		holder.x = dn.x;
		holder.y = m_rectGrp.top + (m_pIndcBase->GetDrawRect().Height() / 2);
	
		m_ptRegion[ii] = holder;
		m_cntRcHolder[ii].SetRect(holder.x, holder.y, holder.x, holder.y);
		m_cntRcHolder[ii].InflateRect(szHOLDER, szHOLDER);
	}
}

void CFiboLine::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	m_pointStart = pt;
	m_bMove = TRUE;
	bMouseCapture = TRUE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
	InflateHolder();
}

void CFiboLine::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
//	if (pt.x < m_rectGrp.left)
//		return;
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	m_pointStart = pt;
	DrawTool(&ClientDC);
	InflateHolder();
}

void CFiboLine::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;
	m_bMove = FALSE;
	DrawTool(&ClientDC);
}

void CFiboLine::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointCur = pt;
	
	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CClientDC ClientDC(m_pwndParent);
	if (!m_bShowHolder)
		DrawHolder(&ClientDC, false);
	m_bShowHolder = false;	
	m_bClipRegion = FALSE;
	m_bMove = TRUE;
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CFiboLine::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	DrawTool(&ClientDC);

	m_pointCur = pt;
	InflateHolder();
}

void CFiboLine::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CFiboLine::PtInToolHolderRect(CPoint pt)
{
	for (int ii = 0; ii < maxFIBOLINE; ii++)
	{
		if (m_cntRcHolder[ii].PtInRect(pt))
			return true;
	}
	return false;
}

bool CFiboLine::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	CPoint	point[maxFIBOLINE][5];
	CPoint	drawUP, drawDN;
	for (int ii = 0; ii < maxFIBOLINE; ii++)
	{
		drawUP.x = m_pointStart.x;
		drawUP.y = m_rectGrp.top;
		drawDN.x = m_pointStart.x;
		drawDN.y = m_rectGrp.bottom;
		drawUP.x = drawDN.x = m_ptRegion[ii].x;

		point[ii][0].x = drawUP.x - szREGION;
		point[ii][0].y = drawUP.y;
		point[ii][1].x = drawUP.x + szREGION;
		point[ii][1].y = drawUP.y;
		point[ii][2].x = drawDN.x + szREGION;
		point[ii][2].y = drawDN.y;
		point[ii][3].x = drawDN.x - szREGION;
		point[ii][3].y = drawDN.y;
		point[ii][4].x = drawUP.x - szREGION;
		point[ii][4].y = drawUP.y;
	}

	int count[maxFIBOLINE]{ 0, };
	for (int ii = 0; ii < maxFIBOLINE; ii++)
		count[ii] = 5;

	CRgn FiboRGN;
	if (!FiboRGN.CreatePolyPolygonRgn(&point[0][0], count, maxFIBOLINE, WINDING))
		return false;

	BOOL bInRgn = FiboRGN.PtInRegion(pt); 		
	FiboRGN.DeleteObject();	

	return (bInRgn ? true : false);
}

CPoint CFiboLine::CenterPointAdjust(CPoint pt)
{
	m_szOneDay = double(m_rectGrp.Width()) / double(m_pIndcBase->GetDisplayDay());

	if (m_szOneDay <= 3.5)
		return pt;

	int xPosition = 0;
	int displayDay = m_pIndcBase->GetDisplayDay();

	CPoint	adjustPT;
	for (int ii = 0; ii < displayDay; ii++)
	{
		xPosition = m_rectGrp.left + int(ii * m_szOneDay);
		if (xPosition >= pt.x)
		{
			xPosition -= int(m_szOneDay);
			xPosition += int((m_szOneDay -1) / 2);
			break;
		}
	}

	adjustPT = pt;
	adjustPT.x = xPosition;
	if (adjustPT.x <= m_rectGrp.left)
	{
		adjustPT.x += int(m_szOneDay);
	}
	else
	if (adjustPT.x >= m_rectGrp.right)
	{
		adjustPT.x -= int(m_szOneDay);
	}

	return adjustPT;
}