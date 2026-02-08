// VLine.cpp: implementation of the CVLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVLine::CVLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{

}

CVLine::~CVLine()
{

}

void CVLine::DrawTool(CDC* pDC)
{
	CPen	*cPen;
	CPoint	drawUP, drawDN;

	CPoint sp;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pointStart = sp;
		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
	}
	else
	{
		sp = m_pointStart;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	}
	int	bkMode = pDC->SetBkMode(TRANSPARENT);
	CPen*	sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);

	CPoint	dn, up;
	double	pos1, pos2, value;
	m_pIndcBase->GetPtToPosValue(m_pointStart, pos1, value);

	drawUP.x = m_pointStart.x;
	drawUP.y = m_rectChart.top;
	drawDN.x = m_pointStart.x;
	drawDN.y = m_rectChart.bottom;

	pos2 = pos1;
	m_pIndcBase->GetValueToPoint(dn, pos2, value);
	drawUP.x = dn.x;
	drawDN.x = drawUP.x;
	if (drawUP.x < m_rectGrp.right)
	{
		pDC->MoveTo(drawUP);
		pDC->LineTo(drawDN);
	}

	pDC->SelectObject(sPen);
	pDC->SetROP2(sROP);
	pDC->SetBkMode(bkMode);

	InflateHolder();
	if (m_bShowHolder)
		DrawHolder(pDC, true);
}

void CVLine::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CVLine::DrawHolder(CDC *pDC, bool showHolder)
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

	if (m_cntRcHolder.right < m_rectGrp.right)
		pDC->Rectangle(&m_cntRcHolder);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sBrush);
}

void CVLine::HideHolder()
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CVLine::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	double	pos1, pos2, value;
	m_pIndcBase->GetPtToPosValue(m_pointStart, pos1, value);
//	pos1 = (int)pos1 + 0.5;

	pos2 = pos1;
	m_pIndcBase->GetValueToPoint(dn, pos2, value);
	holder.x = dn.x;
	holder.y = m_rectChart.top + (m_rectChart.Height() / 2);

	m_ptRegion = holder;
	m_cntRcHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
	m_cntRcHolder.InflateRect(szHOLDER, szHOLDER);
}

void CVLine::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_bMove = TRUE;
	bMouseCapture = TRUE;
	m_bClipRegion = TRUE;

	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	InflateHolder();
}

void CVLine::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	if (pt.x < m_rectGrp.left)
		return;
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	m_pointStart = pt;
	DrawTool(&ClientDC);
	InflateHolder();
}

void CVLine::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;
	m_bMove = FALSE;
	DrawTool(&ClientDC);
}

void CVLine::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CVLine::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CVLine::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CVLine::PtInToolHolderRect(CPoint pt)
{
	if (m_cntRcHolder.PtInRect(pt))
		return true;

	return false;
}

bool CVLine::PtInToolRegion(CPoint pt)
{
	if (!m_rectChart.PtInRect(pt))	return false;

	CPoint	point[maxVLINE][5];
	CPoint	drawUP, drawDN;
	for (int ii = 0; ii < maxVLINE; ii++)
	{
		drawUP.x = m_pointStart.x;
		drawUP.y = m_rectChart.top;
		drawDN.x = m_pointStart.x;
		drawDN.y = m_rectChart.bottom;
		drawUP.x = drawDN.x = m_ptRegion.x;

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

	int count[maxVLINE]{ 0, };
	for (int ii = 0; ii < maxVLINE; ii++)
		count[ii] = 5;

	CRgn VLineRGN;
	if (!VLineRGN.CreatePolyPolygonRgn(&point[0][0], count, maxVLINE, WINDING))
		return false;

	BOOL bInRgn = VLineRGN.PtInRegion(pt); 		
	VLineRGN.DeleteObject();	

	return (bInRgn ? true : false);
}
