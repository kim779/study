// Daedeung.cpp: implementation of the CDaedeung class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Daedeung.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDaedeung::CDaedeung(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{

}

CDaedeung::~CDaedeung()
{

}

void CDaedeung::InitValue(int lGap, int rGap)
{
	m_lGap = lGap;
	m_rGap = rGap;
}
///////////////////////////////////////////////////////////////////////////
// class member function

void CDaedeung::DrawTool(CDC* pDC)
{
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

	CPen*	sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	CalculateDrawingPosition();
	
	pDC->MoveTo(m_ptDaedeung[0].x, m_rectChart.top);
	pDC->LineTo(m_ptDaedeung[0].x, m_rectChart.bottom);
	pDC->MoveTo(m_ptDaedeung[1].x, m_rectChart.top);
	pDC->LineTo(m_ptDaedeung[1].x, m_rectChart.bottom);
	pDC->MoveTo(m_ptDaedeung[2].x, m_rectChart.top);
	pDC->LineTo(m_ptDaedeung[2].x, m_rectChart.bottom);

	if (!m_bMove)
	{
		CString dispText;
		dispText.Format("%d", m_lGap);
		
		CFont	*cFont = GetFont(m_pwndView, 9, "굴림");
		CFont	*sFont = pDC->SelectObject(cFont);
		int	sColor = pDC->SetTextColor(m_rgbMove ^ RGB(255,255,255));
		CSize	sz = pDC->GetOutputTextExtent(dispText);
		CRect	drawRC, orgRect;
		orgRect = m_rectGrp;
		drawRC.SetRect(m_ptDaedeung[1].x + 1, orgRect.bottom - sz.cy, 
			m_ptDaedeung[1].x + sz.cx + 1, orgRect.bottom);
		pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

		dispText.Format("%d", m_rGap);
		sz = pDC->GetOutputTextExtent(dispText);
		drawRC.SetRect(m_ptDaedeung[2].x + 1, orgRect.bottom - sz.cy, 
			m_ptDaedeung[2].x + sz.cx + 1, orgRect.bottom);
		pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

		pDC->SetTextColor(sColor);
		pDC->SelectObject(sFont);
	}

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
	
//	InflateHolder();
//	if (m_bShowHolder)
//		CToolBase::DrawHolder(pDC, true);
}

void CDaedeung::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CDaedeung::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);

//	InflateHolder();
}

void CDaedeung::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	DrawTool(&ClientDC);
	m_pointStart = pt;
//	InflateHolder();
	DrawTool(&ClientDC);
}

void CDaedeung::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
//	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
}

void CDaedeung::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
//	CToolBase::DrawHolder(&ClientDC, false);

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

//	InflateHolder();
}

void CDaedeung::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
//	if (m_bShowHolder)
//		CToolBase::DrawHolder(&ClientDC, false);

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
//	InflateHolder();
}

void CDaedeung::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CDaedeung::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointCur = pt;

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CClientDC ClientDC(m_pwndParent);
//	if (!m_bShowHolder)
//		CToolBase::DrawHolder(&ClientDC, true);
	m_bShowHolder = false;
	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);
	m_bMove = TRUE;
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CDaedeung::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
//	if (m_bShowHolder)
//		CToolBase::DrawHolder(&ClientDC, false);
	DrawTool(&ClientDC);

	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	m_pointEnd.x += pt.x - m_pointCur.x;
	m_pointEnd.y += pt.y - m_pointCur.y;
	DrawTool(&ClientDC);

	m_pointCur = pt;
//	InflateHolder();
}

void CDaedeung::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CDaedeung::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	const	int arrayCOUNT = maxDaedeung +1;
	int	aIDX = 0;

	SetPointArray(aIDX++, 
		CPoint(m_ptDaedeung[0].x, m_rectChart.top), CPoint(m_ptDaedeung[0].x, m_rectChart.bottom));
	SetPointArray(aIDX++, 
		CPoint(m_ptDaedeung[1].x, m_rectChart.top), CPoint(m_ptDaedeung[1].x, m_rectChart.bottom));
	SetPointArray(aIDX++, 
		CPoint(m_ptDaedeung[2].x, m_rectChart.top), CPoint(m_ptDaedeung[2].x, m_rectChart.bottom));

	int	count[arrayCOUNT];
	for (int ii = 0; ii < arrayCOUNT; ii++)
		count[ii] = 5;

	CRgn	rgn;
	if (!rgn.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, 3, WINDING))
		return false;

	BOOL	bInRgn = rgn.PtInRegion(pt); 		
	rgn.DeleteObject();	

	return (bInRgn ? true : false);
}

bool CDaedeung::CalculateDrawingPosition()
{
	double	pos, value;
	double fUnit;

	if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
	{
		m_pIndcBase->GetPtToPosValue(m_pointStart, pos, value);
		pos = (int)pos + 0.5;
		m_pIndcBase->GetValueToPoint(m_ptDaedeung[0], pos, value);
		m_pIndcBase->GetValueToPoint(m_ptDaedeung[1], pos - (m_lGap-1), value);
		m_pIndcBase->GetValueToPoint(m_ptDaedeung[2], pos + (m_rGap-1), value);
	}
	else
	{
		m_pIndcBase->GetPtToPosValue(m_pointStart, pos, value);
		
		m_ptDaedeung[0] = m_pointStart;
		m_ptDaedeung[0].x += (int)(fUnit /2);
		m_ptDaedeung[1] = m_ptDaedeung[0];
		m_ptDaedeung[1].x -= (int)(fUnit * (m_lGap-1));
		m_ptDaedeung[2] = m_ptDaedeung[0];
		m_ptDaedeung[2].x += (int)(fUnit * (m_lGap-1));
	}
	return true;
}

void CDaedeung::SetPointArray(int idx, CPoint pt1, CPoint pt2)
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

void CDaedeung::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	double	pos1, value;

	double fUnit;
	if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
	{
		
		m_pIndcBase->GetPtToPosValue(m_ptDaedeung[1], pos1, value);
		pos1 = (int)pos1 + 0.5;
		m_pIndcBase->GetValueToPoint(dn, pos1, value);
		holder = dn;

		m_ptRegion[0] = holder;
		m_rectDownHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);

		m_pIndcBase->GetPtToPosValue(m_ptDaedeung[2], pos1, value);
		pos1 = (int)pos1 + 0.5;
		m_pIndcBase->GetValueToPoint(dn, pos1, value);
		holder = dn;

		m_ptRegion[1] = holder;
		m_rectUpHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);
	}
	else
	{
		dn = m_ptDaedeung[1];
		dn.x += (int)(fUnit /2);
		holder = dn;
		
		m_ptRegion[0] = holder;
		m_rectDownHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);
		
		dn = m_ptDaedeung[2];
		dn.x += (int)(fUnit /2);
		holder = dn;
		
		m_ptRegion[1] = holder;
		m_rectUpHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);
		
	}
}
