// QuardLine.cpp: implementation of the CQuardLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuardLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuardLine::CQuardLine(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{

}

CQuardLine::~CQuardLine()
{

}

void CQuardLine::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen, *dPen;
	CPoint sp, ep;

	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);

		m_pointStart = sp;
		m_pointEnd = ep;

		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
		dPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbText);
	}
	else
	{
		sp = m_pointStart;
		ep = m_pointEnd;

		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(ep, m_dPosEnd, m_dValueEnd);
//TRACE("===>>>S[%d:%d] [%.f][%.f]  E[%d:%d] [%.f][%.f]\n",
//	m_pointStart.x, m_pointStart.y,	m_dPosStart, m_dValueStart, 
//	m_pointEnd.x, m_pointEnd.y,	m_dPosEnd, m_dValueEnd);
		
		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		dPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbText);
	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen*	sPen = pDC->SelectObject(dPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	CalculateDrawingPosition();
	pDC->MoveTo(m_ptQuardLine[0].x, m_rectChart.top);
	pDC->LineTo(m_ptQuardLine[0].x, m_rectChart.bottom);
	pDC->MoveTo(m_ptQuardLine[1].x, m_rectChart.top);
	pDC->LineTo(m_ptQuardLine[1].x, m_rectChart.bottom);

	pDC->SelectObject(cPen);

	if (m_eToolKind != enumTK::tkTRILINELEVEL)
	{
		pDC->MoveTo(m_ptQuardLine[0]);
		pDC->LineTo(m_ptQuardLine[1]);
		pDC->MoveTo(m_ptQuardLine[2]);
		pDC->LineTo(m_ptQuardLine[3]);
	}

	pDC->MoveTo(m_ptQuardLine[4]);
	pDC->LineTo(m_ptQuardLine[5]);
	pDC->MoveTo(m_ptQuardLine[6]);
	pDC->LineTo(m_ptQuardLine[7]);

	if (m_eToolKind == enumTK::tkQUARDLINE)
	{
		pDC->MoveTo(m_ptQuardLine[8]);
		pDC->LineTo(m_ptQuardLine[9]);
	}
	else
	{
		pDC->MoveTo(m_ptQuardLine[8]);
		pDC->LineTo(m_ptQuardLine[9]);
	}

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)	
		pDC->SelectClipRgn(NULL);
	
	InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);
}

void CQuardLine::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CQuardLine::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	InflateHolder();
}

void CQuardLine::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	DrawTool(&ClientDC);
	m_pointStart = pt;
	InflateHolder();
	DrawTool(&ClientDC);
}

void CQuardLine::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
}

void CQuardLine::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
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

	InflateHolder();
}

void CQuardLine::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
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
	InflateHolder();
}

void CQuardLine::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CQuardLine::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CQuardLine::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
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
	InflateHolder();
}

void CQuardLine::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CQuardLine::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	const	int arrayCOUNT = maxQuardLine +1;
	int	aIDX = 0;

	SetPointArray(aIDX++, 
		CPoint(m_ptQuardLine[0].x, m_rectChart.top), CPoint(m_ptQuardLine[0].x, m_rectChart.bottom));
	SetPointArray(aIDX++, 
		CPoint(m_ptQuardLine[1].x, m_rectChart.top), CPoint(m_ptQuardLine[1].x, m_rectChart.bottom));

	SetPointArray(aIDX++, m_ptQuardLine[0], m_ptQuardLine[1]);
	SetPointArray(aIDX++, m_ptQuardLine[2], m_ptQuardLine[3]);
	SetPointArray(aIDX++, m_ptQuardLine[4], m_ptQuardLine[5]);
	SetPointArray(aIDX++, m_ptQuardLine[6], m_ptQuardLine[7]);
	if (m_eToolKind == enumTK::tkQUARDLINE)
		SetPointArray(aIDX++, m_ptQuardLine[8], m_ptQuardLine[9]);

	int count[arrayCOUNT];
	for (int ii = 0; ii < arrayCOUNT; ii++)
		count[ii] = 5;

	CRgn rgn;
	if (m_eToolKind == enumTK::tkTRILINE || m_eToolKind == enumTK::tkTRILINELEVEL)
	{
		if (!rgn.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, 6, WINDING))
			return false;
	}
	else
	{
		if (!rgn.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, 7, WINDING))
			return false;
	}

	BOOL bInRgn = rgn.PtInRegion(pt); 		
	rgn.DeleteObject();	

	return (bInRgn ? true : false);
}

bool CQuardLine::CalculateDrawingPosition()
{
	int	fpr, hpr, lpr, epr, low, high;
	int	posStart, posEnd;
	double	pos, value;
	CPoint	sp, ep, ptLow, ptHigh;
	double fUnit;
	if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
	{
		m_pIndcBase->GetPtToPosValue(m_pointStart, pos, value);
		pos = (int)pos + 0.5;
		m_pIndcBase->GetValueToPoint(sp, pos, value);
		
		m_pIndcBase->GetPtToPosValue(m_pointEnd, pos, value);
		pos = (int)pos + 0.5;
		m_pIndcBase->GetValueToPoint(ep, pos, value);
	}
	else
	{
		sp = m_pointStart;
		sp.x += (int)(fUnit /2);
		ep = m_pointEnd;
		ep.x += (int)(fUnit /2);
	}

	if(m_dPosStart != m_dPosEnd)
	{
		int jj = 0;
		jj += 2;
	}

	if(m_pIndcBase->IsNoTime())//kangmc추세선변경
	{
		if (m_dPosStart <= m_dPosEnd)
		{
			posStart = m_pIndcBase->GetValueToXIdx(m_dPosStart);
			posEnd = m_pIndcBase->GetValueToXIdx(m_dPosEnd);
		}
		else
		{
			posStart = m_pIndcBase->GetValueToXIdx(m_dPosEnd);
			posEnd = m_pIndcBase->GetValueToXIdx(m_dPosStart);
		}
	}
	else
	{
		if (m_dPosStart <= m_dPosEnd)
		{
			posStart = (int)m_dPosStart;
			posEnd = (int)m_dPosEnd;
		}
		else
		{
			posStart = (int)m_dPosEnd;
			posEnd = (int)m_dPosStart;
		}
	}

	
	m_pIndcBase->GetIndexStrTool(posStart, fpr, high, low, epr);

//TRACE(" posStart[%d] posEnd[%d]\n",posStart, posEnd);
	for (int ii = posStart; ii <= posEnd; ii++)
	{
		m_pIndcBase->GetIndexStrTool(ii, fpr, hpr, lpr, epr);
		if (hpr > high)
			high = hpr;
		if (lpr < low)
			low = lpr;
	}
	
	m_pIndcBase->GetValueToPoint(ptLow, 0, low);
	m_pIndcBase->GetValueToPoint(ptHigh, 0, high);

	if (m_eToolKind == enumTK::tkQUARDLINE)
	{
		m_ptQuardLine[0].x = sp.x;
		m_ptQuardLine[0].y = ptHigh.y;
		m_ptQuardLine[1].x = ep.x;
		m_ptQuardLine[1].y = ptHigh.y;

		m_ptQuardLine[2].x = sp.x;
		m_ptQuardLine[2].y = ptLow.y;
		m_ptQuardLine[3].x = ep.x;
		m_ptQuardLine[3].y = ptLow.y;

		m_ptQuardLine[4].x = sp.x;
		m_ptQuardLine[4].y = ptHigh.y + (ptLow.y - ptHigh.y)/2;
		m_ptQuardLine[5].x = ep.x;
		m_ptQuardLine[5].y = ptHigh.y + (ptLow.y - ptHigh.y)/2;

		m_ptQuardLine[6].x = sp.x;
		m_ptQuardLine[6].y = ptHigh.y + (m_ptQuardLine[4].y - ptHigh.y)/2;
		m_ptQuardLine[7].x = ep.x;
		m_ptQuardLine[7].y = ptHigh.y + (m_ptQuardLine[4].y - ptHigh.y)/2;

		m_ptQuardLine[8].x = sp.x;
		m_ptQuardLine[8].y = m_ptQuardLine[4].y + (m_ptQuardLine[4].y - ptHigh.y)/2;
		m_ptQuardLine[9].x = ep.x;
		m_ptQuardLine[9].y = m_ptQuardLine[4].y + (m_ptQuardLine[4].y - ptHigh.y)/2;
	}
	else if (m_eToolKind == enumTK::tkTRILINE)
	{
		m_ptQuardLine[0].x = sp.x;
		m_ptQuardLine[0].y = ptHigh.y;
		m_ptQuardLine[1].x = ep.x;
		m_ptQuardLine[1].y = ptHigh.y;

		m_ptQuardLine[2].x = sp.x;
		m_ptQuardLine[2].y = ptLow.y;
		m_ptQuardLine[3].x = ep.x;
		m_ptQuardLine[3].y = ptLow.y;

		m_ptQuardLine[4].x = sp.x;
		m_ptQuardLine[4].y = ptHigh.y + (ptLow.y - ptHigh.y)/3;
		m_ptQuardLine[5].x = ep.x;
		m_ptQuardLine[5].y = ptHigh.y + (ptLow.y - ptHigh.y)/3;

		m_ptQuardLine[6].x = sp.x;
		m_ptQuardLine[6].y = ptHigh.y + (ptLow.y - ptHigh.y)/3*2;
		m_ptQuardLine[7].x = ep.x;
		m_ptQuardLine[7].y = ptHigh.y + (ptLow.y - ptHigh.y)/3*2;

		m_ptQuardLine[8].x = 0;
		m_ptQuardLine[8].y = 0;
		m_ptQuardLine[9].x = 0;
		m_ptQuardLine[9].y = 0;
	}
	else if (m_eToolKind == enumTK::tkTRILINELEVEL)
	{
		m_ptQuardLine[0].x = sp.x;
		m_ptQuardLine[0].y = ptHigh.y;
		m_ptQuardLine[1].x = ep.x;
		m_ptQuardLine[1].y = ptHigh.y;

		m_ptQuardLine[2].x = sp.x;
		m_ptQuardLine[2].y = ptLow.y;
		m_ptQuardLine[3].x = ep.x;
		m_ptQuardLine[3].y = ptLow.y;

		m_ptQuardLine[4].x = sp.x;
		m_ptQuardLine[4].y = ptHigh.y + (ptLow.y - ptHigh.y)/3;
		m_ptQuardLine[5].x = ep.x;
		m_ptQuardLine[5].y = ptHigh.y + (ptLow.y - ptHigh.y)/3;

		m_ptQuardLine[6].x = sp.x;
		m_ptQuardLine[6].y = ptHigh.y + (ptLow.y - ptHigh.y)/3*2;
		m_ptQuardLine[7].x = ep.x;
		m_ptQuardLine[7].y = ptHigh.y + (ptLow.y - ptHigh.y)/3*2;

		m_ptQuardLine[8].x = sp.x;
		m_ptQuardLine[8].y = m_ptQuardLine[4].y + (m_ptQuardLine[6].y - m_ptQuardLine[4].y) / 2;
		m_ptQuardLine[9].x = ep.x;
		m_ptQuardLine[9].y = m_ptQuardLine[5].y + (m_ptQuardLine[7].y - m_ptQuardLine[5].y) / 2;
	}
	return true;
}

void CQuardLine::SetPointArray(int idx, CPoint pt1, CPoint pt2)
{
	if (pt1.x == pt2.x)
	{
		m_RgnPoint[idx][0].x = pt1.x - szREGION;
		m_RgnPoint[idx][0].y = pt1.y;
		m_RgnPoint[idx][1].x = pt1.x + szREGION;
		m_RgnPoint[idx][1].y = pt1.y;
		m_RgnPoint[idx][2].x = pt2.x + szREGION;
		m_RgnPoint[idx][2].y = pt2.y;
		m_RgnPoint[idx][3].x = pt2.x - szREGION;
		m_RgnPoint[idx][3].y = pt2.y;
		m_RgnPoint[idx][4].x = pt1.x - szREGION;
		m_RgnPoint[idx][4].y = pt1.y;
	}
	else
	{
		m_RgnPoint[idx][0].x = pt1.x;
		m_RgnPoint[idx][0].y = pt1.y - szREGION;
		m_RgnPoint[idx][1].x = pt2.x;
		m_RgnPoint[idx][1].y = pt2.y - szREGION;
		m_RgnPoint[idx][2].x = pt2.x;
		m_RgnPoint[idx][2].y = pt2.y + szREGION;
		m_RgnPoint[idx][3].x = pt1.x;
		m_RgnPoint[idx][3].y = pt1.y + szREGION;
		m_RgnPoint[idx][4].x = pt1.x;
		m_RgnPoint[idx][4].y = pt1.y - szREGION;
	}
}

void CQuardLine::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	double	pos1, value;

	double fUnit;
	if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
	{
		m_pIndcBase->GetPtToPosValue(m_ptQuardLine[0], pos1, value);
		pos1 = (int)pos1 + 0.5;
		m_pIndcBase->GetValueToPoint(dn, pos1, value);
		holder = dn;
		
		m_ptRegion[0] = holder;
		m_rectDownHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);
		
		m_pIndcBase->GetPtToPosValue(m_ptQuardLine[3], pos1, value);
		pos1 = (int)pos1 + 0.5;
		m_pIndcBase->GetValueToPoint(dn, pos1, value);
		holder = dn;
		
		m_ptRegion[1] = holder;
		m_rectUpHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);
	}
	else
	{
		dn = m_ptQuardLine[0];
		dn.x += (int)(fUnit /2);
		holder = dn;

		m_ptRegion[0] = holder;
		m_rectDownHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);
		
		dn = m_ptQuardLine[3];
		dn.x += (int)(fUnit /2);
		holder = dn;
		
		m_ptRegion[1] = holder;
		m_rectUpHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);
	}
}
