// Gigan.cpp: implementation of the CGigan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gigan.h"
#include "../../h/axisgwin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGigan::CGigan(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_dispStr1 = m_dispStr2 = _T("");
	m_pt1 = m_pt2 = CPoint(0,0);
}

CGigan::~CGigan()
{

}

void CGigan::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen;
	CBrush	*cBrush;
	CPoint sp, ep;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);

		m_pointStart = sp;
		m_pointEnd = ep;

		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
		cBrush = GetBrush(m_pwndView, m_rgbText);
	}
	else
	{
		sp = m_pointStart;
		ep = m_pointEnd;

		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(ep, m_dPosEnd, m_dValueEnd);

		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		cBrush = GetBrush(m_pwndView, m_rgbMove);
	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen	*sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	bool bUp = CalculateDrawingPosition();

	pDC->MoveTo(m_ptGigan[0]);
	pDC->LineTo(m_ptGigan[1]);
	pDC->LineTo(m_ptGigan[2]);
	pDC->LineTo(m_ptGigan[3]);
	if (m_eToolKind == enumTK::tkGIGAN2)
	{
		pDC->MoveTo(m_ptGigan[2]);
		pDC->LineTo(m_ptGigan[4]);
		pDC->LineTo(m_ptGigan[5]);
	}

//	DrawArrow(pDC, m_ptGigan[3], cBrush, bUp);

	CString dispText;
	int nCandle = abs(int(m_dPosStart) - int (m_dPosEnd)) + 1;
	dispText.Format(_T("%d캔들(%s ∼ %s)"), nCandle, m_date1.GetString(), m_date2.GetString());

	CFont	*cFont;
	cFont = GetFont(m_pwndView, 9, "굴림");
	CFont	*sFont = pDC->SelectObject(cFont);
	int	sColor = pDC->SetTextColor(m_rgbMove ^ RGB(255,255,255));
	CSize	sz = pDC->GetOutputTextExtent(dispText);
	CRect	drawRC;

	CPoint pt = m_ptGigan[1].x < m_ptGigan[2].x ? m_ptGigan[1] : m_ptGigan[2];

	if (bUp)
		drawRC.SetRect(pt.x, pt.y + 2, pt.x + sz.cx, pt.y + sz.cy + 2);
	else
		drawRC.SetRect(pt.x, pt.y - 2, pt.x + sz.cx, pt.y - sz.cy - 2);

	drawRC.NormalizeRect();
	pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

	if (bUp)
		drawRC.SetRect(pt.x, drawRC.bottom + 2, pt.x + sz.cx, drawRC.bottom + sz.cy + 2);
	else
		drawRC.SetRect(pt.x, drawRC.top - 2, pt.x + sz.cx, drawRC.top - sz.cy - 2);
	
	if (m_dispDiff.Left(2) == "▲")
		pDC->SetTextColor(RGB(255,0,0));
	else if (m_dispDiff.Left(2) == "▼")
		pDC->SetTextColor(RGB(0,0,255));

	drawRC.NormalizeRect();
	pDC->DrawText(m_dispDiff, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

	pDC->SetTextColor(sColor);
	pDC->SelectObject(sFont);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)
		pDC->SelectClipRgn(NULL);
	
	InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);
}

void CGigan::DrawArrow(CDC *pDC, CPoint pt, CBrush *pBrush, bool bUp)
{
	CPoint rgnPt[5];
	
	if (bUp)
	{
		rgnPt[0] = pt;
		rgnPt[1].x = pt.x - 5;	rgnPt[1].y = pt.y;
		rgnPt[2].x = pt.x;	rgnPt[2].y = pt.y - 5;
		rgnPt[3].x = pt.x + 5;	rgnPt[3].y = pt.y;
		rgnPt[4] = pt;
	}
	else
	{
		rgnPt[0] = pt;
		rgnPt[1].x = pt.x - 5;	rgnPt[1].y = pt.y;
		rgnPt[2].x = pt.x;	rgnPt[2].y = pt.y + 5;
		rgnPt[3].x = pt.x + 5;	rgnPt[3].y = pt.y;
		rgnPt[4] = pt;
	}

	CRgn rgn;
	rgn.CreatePolygonRgn(rgnPt, 5, WINDING);
	pDC->FillRgn(&rgn, pBrush);
	rgn.DeleteObject();
}

void CGigan::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CGigan::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	InflateHolder();
}

void CGigan::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
//	DrawTool(&ClientDC);

	m_pointStart = pt;
	InflateHolder();
	
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
}

void CGigan::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
//	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
}

void CGigan::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
//	DrawTool(&ClientDC);

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

void CGigan::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);

//	DrawTool(&ClientDC);
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

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	InflateHolder();
}

void CGigan::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CGigan::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
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
//	DrawTool(&ClientDC);
	m_bMove = TRUE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CGigan::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);
//	DrawTool(&ClientDC);

	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	m_pointEnd.x += pt.x - m_pointCur.x;
	m_pointEnd.y += pt.y - m_pointCur.y;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);

	m_pointCur = pt;
	InflateHolder();
}

void CGigan::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CGigan::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	const	int arrayCOUNT = maxGigan +1;
	int	aIDX = 0;

	SetPointArray(aIDX++, m_pointStart, m_pointEnd);
	for (int ii = 0; ii < maxGigan; ii++)
		SetPointArray(aIDX++, m_ptGigan[ii], m_ptGigan[ii+1]);

	int count[arrayCOUNT]{ 0, };
	for (int ii = 0; ii < arrayCOUNT; ii++)
		count[ii] = 5;

	CRgn rgn;
	if (m_eToolKind == enumTK::tkGIGAN)
	{
		if (!rgn.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, arrayCOUNT - 2, WINDING))
			return false;
	}
	else
	{
		if (!rgn.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, arrayCOUNT, WINDING))
			return false;
	}

	BOOL bInRgn = rgn.PtInRegion(pt); 		
	rgn.DeleteObject();	

	return (bInRgn ? true : false);
}

bool CGigan::CalculateDrawingPosition()
{
	const int dLength = 20;
	bool	bUp = false;

	int	fpr1, fpr2, hpr1, hpr2, lpr1, lpr2, epr1, epr2;
	double	pos, value;
	CPoint	sp, ep, endPt;
//	CString date1, date2;

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
//		sp.x += (int)(fUnit /2);
		ep = m_pointEnd;
//		ep.x += (int)(fUnit /2);
	}


	CString date1 = m_pIndcBase->GetIndexValueStr(m_pointStart, fpr1, hpr1, lpr1, epr1);
	CString date2 = m_pIndcBase->GetIndexValueStr(m_pointEnd, fpr2, hpr2, lpr2, epr2);
	m_pIndcBase->GetPtToEndvalPt(m_pointEnd, endPt);
	
	
	if (epr1 == 0)	epr1 = 1;
	if (epr2== 0)	epr2 = 1;
	if (!date1.IsEmpty() && atoi(date1.Mid(1)) > atoi(date2.Mid(1)))
	{
		if (date1.GetAt(0) == 't')
		{
			m_date1.Format(_T("%s:%s:%s"), LPCTSTR(date2.Mid(9, 2)), LPCTSTR(date2.Mid(11, 2)), LPCTSTR(date2.Mid(13,2)));
			m_date2.Format(_T("%s:%s:%s"), LPCTSTR(date1.Mid(9, 2)), LPCTSTR(date1.Mid(11, 2)), LPCTSTR(date1.Mid(13,2)));
		}
		else if (date2.GetAt(0) == 'd')
		{
			m_date1.Format(_T("%s/%s/%s"), LPCTSTR(date2.Mid(1, 4)), LPCTSTR(date2.Mid(5, 2)), LPCTSTR(date2.Mid(7,2)));
			m_date2.Format(_T("%s/%s/%s"), LPCTSTR(date1.Mid(1, 4)), LPCTSTR(date1.Mid(5, 2)), LPCTSTR(date1.Mid(7,2)));
		}
		int diff = epr1 - epr2;

		if (m_pIndcBase->GetDataUnit() == GU_CODE)
		{
			if (diff > 0)
				m_dispDiff.Format("▲%d(%.2f%%)", diff, diff*100./epr2);
			else if (diff < 0)
				m_dispDiff.Format("▼%d(%.2f%%)", diff, diff*100./epr2);
			else
				m_dispDiff = _T("0(0%)");
		}
		else
		{
			if (diff > 0)
				m_dispDiff.Format("▲%.2f(%.2f%%)", diff/100., diff*100./epr2);
			else if (diff < 0)
				m_dispDiff.Format("▼%.2f(%.2f%%)", diff/100., diff*100./epr2);
			else
				m_dispDiff = _T("0(0%)");
		}
	}
	else if (!date1.IsEmpty() && atoi(date1.Mid(1)) <= atoi(date2.Mid(1)))
	{
		if (date1.GetAt(0) == 't')
		{
			m_date1.Format(_T("%s:%s:%s"), LPCTSTR(date1.Mid(9, 2)), LPCTSTR(date1.Mid(11, 2)), LPCTSTR(date1.Mid(13,2)));
			m_date2.Format(_T("%s:%s:%s"), LPCTSTR(date2.Mid(9, 2)), LPCTSTR(date2.Mid(11, 2)), LPCTSTR(date2.Mid(13,2)));
		}
		else if (date2.GetAt(0) == 'd')
		{
			m_date1.Format(_T("%s/%s/%s"), LPCTSTR(date1.Mid(1, 4)), LPCTSTR(date1.Mid(5, 2)), LPCTSTR(date1.Mid(7,2)));
			m_date2.Format(_T("%s/%s/%s"), LPCTSTR(date2.Mid(1, 4)), LPCTSTR(date2.Mid(5, 2)), LPCTSTR(date2.Mid(7,2)));
		}
		int diff = epr2 - epr1;

		if (m_pIndcBase->GetDataUnit() == GU_CODE)
		{
			if (diff > 0)
				m_dispDiff.Format("▲%d(%.2f%%)", diff, diff*100./epr1);
			else if (diff < 0)
				m_dispDiff.Format("▼%d(%.2f%%)", diff, diff*100./epr1);
			else
				m_dispDiff = _T("0(0%)");
		}
		else
		{
			if (diff > 0)
				m_dispDiff.Format("▲%.2f(%.2f%%)", diff/100., diff*100./epr1);
			else if (diff < 0)
				m_dispDiff.Format("▼%.2f(%.2f%%)", diff/100., diff*100./epr1);
			else
				m_dispDiff = _T("0(0%)");
		}
	}

	if (m_pointEnd.y < endPt.y)
	{
		m_ptGigan[0] = ep;
		m_ptGigan[1].x = ep.x;
		if (sp.y < ep.y)
			m_ptGigan[1].y = sp.y - dLength;
		else
			m_ptGigan[1].y = m_ptGigan[0].y - dLength;
		m_ptGigan[2].x = sp.x;
		m_ptGigan[2].y = m_ptGigan[1].y;
		m_ptGigan[3] = sp;

		if (m_ptGigan[2].x > m_ptGigan[1].x)
			m_ptGigan[4].x = m_ptGigan[2].x + (m_ptGigan[2].x - m_ptGigan[1].x);
		else
			m_ptGigan[4].x = m_ptGigan[2].x - (m_ptGigan[1].x - m_ptGigan[2].x);
		m_ptGigan[4].y = m_ptGigan[2].y;
		m_ptGigan[5].x = m_ptGigan[4].x;
		m_ptGigan[5].y = m_ptGigan[0].y;

		bUp = false;
	}
	else
	{
		m_ptGigan[0] = ep;
		m_ptGigan[1].x = ep.x;
		if (sp.y > (ep.y))
			m_ptGigan[1].y = sp.y + dLength;
		else
			m_ptGigan[1].y = m_ptGigan[0].y + dLength;
		m_ptGigan[2].x = sp.x;
		m_ptGigan[2].y = m_ptGigan[1].y;
		m_ptGigan[3] = sp;

		if (m_ptGigan[2].x > m_ptGigan[1].x)
			m_ptGigan[4].x = m_ptGigan[2].x + (m_ptGigan[2].x - m_ptGigan[1].x);
		else
			m_ptGigan[4].x = m_ptGigan[2].x - (m_ptGigan[1].x - m_ptGigan[2].x);
		m_ptGigan[4].y = m_ptGigan[2].y;
		m_ptGigan[5].x = m_ptGigan[4].x;
		m_ptGigan[5].y = m_ptGigan[0].y;

		bUp = true;
	}

	return bUp;
}

void CGigan::SetPointArray(int idx, CPoint pt1, CPoint pt2)
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

void CGigan::InflateHolder()
{
	CPoint holder;
	CPoint	dn;
	double	pos1, value;

	double fUnit;
	if(( fUnit = m_pIndcBase->GetOneDataSize()) == 0)//kangmc추세선변경
	{
		
		m_pIndcBase->GetPtToPosValue(m_pointStart, pos1, value);
		pos1 = (int)pos1 + 0.5;
		m_pIndcBase->GetValueToPoint(dn, pos1, value);
		holder = dn;

		m_ptRegion[0] = holder;
		m_rectDownHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);

		m_pIndcBase->GetPtToPosValue(m_pointEnd, pos1, value);
		pos1 = (int)pos1 + 0.5;
		m_pIndcBase->GetValueToPoint(dn, pos1, value);
		holder = dn;

		m_ptRegion[1] = holder;
		m_rectUpHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);
	}
	else
	{	
		dn = m_pointStart;
//		dn.x += (int)(fUnit /2);
		holder = dn;
		
		m_ptRegion[0] = holder;
		m_rectDownHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);
		
		dn = m_pointEnd;
//		dn.x += (int)(fUnit /2);
		holder = dn;
		
		m_ptRegion[1] = holder;
		m_rectUpHolder.SetRect(holder.x, holder.y, holder.x, holder.y);
		m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);
	
	}
}
