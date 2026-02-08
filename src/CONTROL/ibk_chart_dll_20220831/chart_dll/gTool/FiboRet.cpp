// FiboRet.cpp: implementation of the CFiboRet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiboRet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double	fiboRET[] = {0.0, 23.6, 38.2, 50.0, 61.8, 100.0, 
			161.8, 261.8, 423.6, 685.4, 1109.0};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFiboRet::CFiboRet(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{

}

CFiboRet::~CFiboRet()
{

}

void CFiboRet::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen	*cPen, *dPen, *yellowPen;
	CPoint sp, ep;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(ep, m_dPosEnd, m_dValueEnd);

		m_pointStart = sp;
		m_pointEnd = ep;

		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
		dPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbText);
		yellowPen = GetPen(m_pwndView, PS_SOLID, 1, _YELLOW ^ m_rgbBG);
	}
	else
	{
		sp = m_pointStart;
		ep = m_pointEnd;

		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(ep, m_dPosEnd, m_dValueEnd);

		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		dPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
		yellowPen = GetPen(m_pwndView, PS_DOT, 1, _YELLOW);
	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	CPen	*sPen = pDC->SelectObject(yellowPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	CalculateDrawingPosition();
	pDC->MoveTo(m_pointStart);
	pDC->LineTo(m_pointEnd);

	pDC->SelectObject(cPen);
	int ii = 0;
	for (ii = 0; ii < 6; ii++)
	{
		pDC->MoveTo(m_Retracement1[ii]);
		pDC->LineTo(m_Retracement2[ii]);
	}
	pDC->SelectObject(dPen);
	for (; ii < maxFIBORET; ii++)
	{
		pDC->MoveTo(m_Retracement1[ii]);
		pDC->LineTo(m_Retracement2[ii]);
	}

	if (!m_bMove)
	{
		CFont	*cFont = GetFont(m_pwndView, 9, "±¼¸²");
		CFont	*sFont = pDC->SelectObject(cFont);
		int	sColor = pDC->SetTextColor(m_rgbMove);
		char	*pStr[6] = {"0.00%", "23.6%", "38.2%", "50.0%", "61.8%", "100%"};
		char	*pStr2[5] = {"161.80%", "261.80%", "423.60%", "685.40%", "1109.00%"};
		CSize	sz = pDC->GetOutputTextExtent(pStr[0]);

		if (m_Retracement1[0].x < m_Retracement2[0].x)
		{
			for (ii = 0; ii < 6; ii++)
			{
				CRect	drawRC;
				CString	dispText = pStr[ii];
				drawRC.SetRect(m_Retracement1[ii].x - sz.cx, m_Retracement1[ii].y - sz.cy/2, 
					m_Retracement1[ii].x, m_Retracement1[ii].y + sz.cy/2);
				pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			}
			for (ii = 6; ii < maxFIBORET; ii++)
			{
				sz = pDC->GetOutputTextExtent(pStr2[ii-6]);
				CRect	drawRC;
				CString	dispText = pStr2[ii-6];
				drawRC.SetRect(m_Retracement1[ii].x - sz.cx, m_Retracement1[ii].y - sz.cy/2, 
					m_Retracement1[ii].x, m_Retracement1[ii].y + sz.cy/2);
				pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			}
		}
		else
		{
			for (ii = 0; ii < 6; ii++)
			{
				CRect	drawRC;
				CString	dispText = pStr[ii];
				drawRC.SetRect(m_Retracement2[ii].x - sz.cx, m_Retracement2[ii].y - sz.cy/2, 
					m_Retracement2[ii].x, m_Retracement2[ii].y + sz.cy/2);
				pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			}
			for (ii = 6; ii < maxFIBORET; ii++)
			{
				sz = pDC->GetOutputTextExtent(pStr2[ii-6]);
				CRect	drawRC;
				CString	dispText = pStr2[ii-6];
				drawRC.SetRect(m_Retracement2[ii].x - sz.cx, m_Retracement2[ii].y - sz.cy/2, 
					m_Retracement2[ii].x, m_Retracement2[ii].y + sz.cy/2);
				pDC->DrawText(dispText, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			}
		}

		pDC->SetTextColor(sColor);
		pDC->SelectObject(sFont);
	}

	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(bkMode);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
	
	CToolBase::InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);
}

void CFiboRet::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CFiboRet::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	CToolBase::InflateHolder();
}

void CFiboRet::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	DrawTool(&ClientDC);
	m_pointStart = pt;
	CToolBase::InflateHolder();
	DrawTool(&ClientDC);
}

void CFiboRet::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	DrawTool(&ClientDC);
}

void CFiboRet::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CFiboRet::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CFiboRet::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CFiboRet::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
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

void CFiboRet::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
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

void CFiboRet::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CFiboRet::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	const	int arrayCOUNT = maxFIBORET +1;
	int	aIDX = 0;

	SetPointArray(aIDX++, m_pointStart, m_pointEnd);
	for (int ii = 0; ii < maxFIBORET; ii++)
		SetPointArray(aIDX++, m_Retracement1[ii], m_Retracement2[ii]);

	int count[arrayCOUNT]{ 0, };
	for (int ii = 0; ii < arrayCOUNT; ii++)
		count[ii] = 5;

	CRgn FiboRGN;
	if (!FiboRGN.CreatePolyPolygonRgn(&m_RgnPoint[0][0], count, arrayCOUNT, WINDING))
		return false;

	BOOL bInRgn = FiboRGN.PtInRegion(pt); 		
	FiboRGN.DeleteObject();	

	return (bInRgn ? true : false);
}

///////////////////////////////////////////////////////////////////////
void CFiboRet::CalculateDrawingPosition()
{
	int ii;
	int Width  = m_pointStart.x - m_pointEnd.x;
	int Height = m_pointEnd.y - m_pointStart.y;

	int gap = m_pointStart.x - m_pointEnd.x;

	for (ii = 0; ii < maxFIBORET; ii++)
	{
		if (m_pointStart.x > m_pointEnd.x)
			m_Retracement1[ii].x = m_pointStart.x;
		else
			m_Retracement1[ii].x = m_pointStart.x - gap;

		m_Retracement1[ii].y = m_pointStart.y + int(Height * fiboRET[ii] / 100.);

		m_Retracement2[ii].x = m_rectGrp.right;//m_pointEnd.x;
		m_Retracement2[ii].y = m_Retracement1[ii].y;
	}
}

void CFiboRet::SetPointArray(int idx, CPoint pt1, CPoint pt2)
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
