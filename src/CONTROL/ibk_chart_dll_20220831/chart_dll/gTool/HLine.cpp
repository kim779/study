// HLine.cpp: implementation of the CHLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HLine.h"
#include "../../h/axisgwin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHLine::CHLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_dwToolOption = TO_SHOW_POSI;
}

CHLine::~CHLine()
{

}

void CHLine::DrawTool(CDC* pDC)
{
	CPen* ppenCur;
	CPoint pointStart;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(pointStart, m_dPosStart, m_dValueStart);
		m_pointStart = pointStart;
		ppenCur = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
	}
	else
	{
		pointStart = m_pointStart;
		m_pIndcBase->GetPtToPosValue(pointStart, m_dPosStart, m_dValueStart);
		ppenCur = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	}
	
	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	CPen* ppenOld = pDC->SelectObject(ppenCur);
	int iOldRop = pDC->SetROP2(R2_XORPEN);

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	double	dPos1, dValue;
	m_pIndcBase->GetPtToPosValue(m_pointStart, dPos1, dValue);

	CPoint	pointDrawUp, pointDrawDown;
	pointDrawUp.x = m_pointStart.x;
	pointDrawUp.y = m_rectChart.top;
	pointDrawDown.x = m_pointStart.x;
	pointDrawDown.y = m_rectChart.bottom;

	pointDrawUp.x = m_rectChart.left;
	pointDrawUp.y = m_pointStart.y;
	pointDrawDown.x = m_rectChart.right;
	pointDrawDown.y = m_pointStart.y;

	double dPos2 = dPos1;
	CPoint	pointDown;
	m_pIndcBase->GetValueToPoint(pointDown, dPos2, dValue);
	pointDrawUp.y = pointDown.y;
	pointDrawDown.y = pointDrawUp.y;
	if (pointDrawUp.x < m_rectGrp.right)
	{
		pDC->MoveTo(pointDrawUp);
		pDC->LineTo(pointDrawDown);

		CString	str;
		m_pIndcBase->GetValueToStr(m_dValueStart, str);

		if (!str.IsEmpty() && (m_dwToolOption & TO_SHOW_POSI))
		{
			CFont* pfontCur = GetFont(m_pwndView, 9, "±¼¸²");
			CFont* pfontOld = pDC->SelectObject(pfontCur);
			COLORREF rgbOldText = pDC->SetTextColor(m_rgbMove);
			CSize sizeText = pDC->GetOutputTextExtent(str);

			CRect rc;
			rc.SetRect(m_rectGrp.left + 5, pointDrawUp.y - sizeText.cy, m_rectGrp.left + (sizeText.cx + 5), pointDrawUp.y);
			pDC->DrawText(str, &rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

			pDC->SetTextColor(rgbOldText);
			pDC->SelectObject(pfontOld);
		}
	}

	pDC->SelectObject(ppenOld);
	pDC->SetROP2(iOldRop);
	pDC->SetBkMode(iOldMode);

	InflateHolder();
	if (m_bShowHolder)
		DrawHolder(pDC, true);

	if (m_bClipRegion)
		pDC->SelectClipRgn(NULL);
}

void CHLine::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CHLine::DrawHolder(CDC *pDC, bool showHolder)
{
//	CClientDC ClientDC(m_pwndParent);

	CBrush	*cBrush = GetBrush(m_pwndView, m_rgbHolder);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	int	sROP;

	if (showHolder)
	{
		m_bShowHolder = true;
	}
	else
	{
		m_bShowHolder = false;
	}
	sROP = pDC->SetROP2(R2_XORPEN);

	if (m_cntRcHolder.right < m_rectGrp.right)
		pDC->Rectangle(&m_cntRcHolder);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sBrush);
}

void CHLine::HideHolder()
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CHLine::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	double	pos1, pos2, value;
	m_pIndcBase->GetPtToPosValue(m_pointStart, pos1, value);

	pos2 = pos1;
	m_pIndcBase->GetValueToPoint(dn, pos2, value);
	holder.x = m_rectChart.left + int(m_rectChart.Width() / 2);
	holder.y = dn.y;

	m_ptRegion = holder;
	m_cntRcHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
	m_cntRcHolder.InflateRect(szHOLDER, szHOLDER);
}

void CHLine::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	m_pointStart = pt;
	m_bMove = TRUE;
	bMouseCapture = TRUE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
	InflateHolder();
}

void CHLine::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	if (pt.x < m_rectGrp.left)
		return;
	CClientDC ClientDC(m_pwndParent);
//	DrawTool(&ClientDC);
	m_pointStart = pt;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	InflateHolder();
}

void CHLine::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	DrawHolder(&ClientDC, true);
//	DrawTool(&ClientDC);

	bMouseCapture = FALSE;
	m_bMove = FALSE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
}

void CHLine::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
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

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CHLine::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);

	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);

	m_pointCur = pt;
	InflateHolder();
}

void CHLine::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CHLine::PtInToolHolderRect(CPoint pt)
{
	if (m_cntRcHolder.PtInRect(pt))
		return true;

	return false;
}

bool CHLine::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	CPoint	point[maxHLINE][5];
	CPoint	drawUP, drawDN;
	for (int ii = 0; ii < maxHLINE; ii++)
	{
		drawUP.x = m_rectGrp.left;
		drawUP.y = m_pointStart.y;
		drawDN.x = m_rectGrp.right;
		drawDN.y = m_pointStart.y;
//		drawUP.x = drawDN.x = m_ptRegion.x;

		point[ii][0].x = drawUP.x;
		point[ii][0].y = drawUP.y - szREGION;
		point[ii][1].x = drawUP.x;
		point[ii][1].y = drawUP.y + szREGION;
		point[ii][2].x = drawDN.x;
		point[ii][2].y = drawDN.y + szREGION;
		point[ii][3].x = drawDN.x;
		point[ii][3].y = drawDN.y - szREGION;
		point[ii][4].x = drawUP.x;
		point[ii][4].y = drawUP.y - szREGION;
	}

	int count[maxHLINE]{ 0, };
	for (int ii = 0; ii < maxHLINE; ii++)
		count[ii] = 5;

	CRgn HLineRGN;
	if (!HLineRGN.CreatePolyPolygonRgn(&point[0][0], count, maxHLINE, WINDING))
		return false;

	BOOL bInRgn = HLineRGN.PtInRegion(pt); 		
	HLineRGN.DeleteObject();	

	return (bInRgn ? true : false);
}

